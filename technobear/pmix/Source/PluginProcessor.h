#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"
#include "ssp/RmsTrack.h"

#include <atomic>
#include <algorithm>


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (pitch)
PARAMETER_ID (vca)
#undef PARAMETER_ID
}

struct TrackData {
    TrackData() {
        init();
    }

    void makeFollow(unsigned f) {
        dummy_ = true;
        follows_ = f;
    }

    void init() {
        level_[MASTER] = 1.0f;
        level_[CUE] = 1.0f;
        for (unsigned i = CUE + 1; i < OUT_TRACKS; i++) level_[i] = 0.0f;
        pan_ = 0.0f;
        gain_ = 1.0f;
        mute_ = false;
        solo_ = false;
        cue_ = false;
        ac_ = false;
        dummy_ = false;
        follows_ = 0;
    }

    static constexpr unsigned OUT_TRACKS = 4;
    static constexpr unsigned MASTER = 0;
    static constexpr unsigned CUE = 1;

    std::atomic<float> level_[OUT_TRACKS];  // 0==master, 1==cue , 2/3 == aux1/2
    std::atomic<float> pan_;    // -1 to 1
    std::atomic<float> gain_;
    std::atomic<bool> mute_;
    std::atomic<bool> solo_;
    std::atomic<bool> cue_;
    std::atomic<bool> ac_;

    // currently cannot be changed in ui
    bool dummy_;
    unsigned follows_; // dummy
    // hpf/dc block
    float dcX1_ = 0.0f, dcY1_ = 0.0f;
    ssp::RmsTrack rms_;
};


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor() override = default;

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override {}

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &pitch;
        Parameter &vca;
    } params_;

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    TrackData &inputTrack(unsigned t) { return t < IN_T_MAX ? inTracks_[t] : inTracks_[IN_T_MAX - 1]; }

    TrackData &outputTrack(unsigned t) { return t < OUT_T_MAX ? outTracks_[t] : outTracks_[OUT_T_MAX - 1]; }

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    enum {
        I_IN_1,
        I_IN_2,
        I_IN_3,
        I_IN_4,
        I_IN_5,
        I_IN_6,
        I_IN_7,
        I_IN_8,
        I_MAX
    };

    enum {
        O_MAIN_L,
        O_MAIN_R,
        O_CUE_L,
        O_CUE_R,
        O_AUX_1_L,
        O_AUX_1_R,
        O_AUX_2_L,
        O_AUX_2_R,
        O_MAX
    };

public:
    static constexpr unsigned IN_T_MAX = I_MAX;
    static constexpr unsigned OUT_T_MAX = O_MAX;
private:

    unsigned numInTracks() { return IN_T_MAX; }

    unsigned numOutTracks() { return OUT_T_MAX; }


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) {
            props.addBus(true, getInputBusName(i), AudioChannelSet::mono());
        }
        for (auto i = 0; i < O_MAX; i++) {
            props.addBus(false, getOutputBusName(i), AudioChannelSet::mono());
        }
        return props;
    }

    void writeToXml(juce::XmlElement &xml);
    void readFromXml(juce::XmlElement &xml);

    void writeTrackXml(TrackData &t, juce::XmlElement &xml);
    void readTrackXml(TrackData &t, juce::XmlElement &xml);


    inline void dcBlock(float x, float &x1, float &y, float &y1) {
        // y[n] = x[n] - x[n-1] + a * y[n-1]
        // example usage
        // dcBlock(curSample, lastIn, curOut, lastOut)
        static constexpr float a = 0.995f;
        y = x - x1 + a * y1;
        y1 = y;
        x1 = x;
    }


    TrackData inTracks_[IN_T_MAX];
    TrackData outTracks_[OUT_T_MAX];
    void initTracks();
    AudioSampleBuffer inputBuffers_;
    AudioSampleBuffer outputBuffers_;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


