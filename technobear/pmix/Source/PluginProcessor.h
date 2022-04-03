#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"
#include "ssp/RmsTrack.h"

#include <atomic>
#include <algorithm>

namespace ID {
constexpr const char *separator{":"};

#define PARAMETER_ID(str) constexpr const char* str { #str };
PARAMETER_ID (in)
PARAMETER_ID (out)

PARAMETER_ID (level)
PARAMETER_ID (pan)
PARAMETER_ID (gain)
PARAMETER_ID (mute)
PARAMETER_ID (solo)
PARAMETER_ID (cue)
PARAMETER_ID (ac)
#undef PARAMETER_ID
}


//level
//pan
//gain
//mute
//solo
//cue
//ac


struct TrackData {
    using Parameter = juce::RangedAudioParameter;
    explicit TrackData(juce::AudioProcessorValueTreeState &, StringRef io, unsigned tn);
//    TrackData() {
//        init();
//    }

    void makeFollow(unsigned f) {
        dummy_ = true;
        follows_ = f;
    }

    void init() {
        dummy_ = false;
        follows_ = 0;
        rms_.clear();
    }

    static constexpr unsigned OUT_TRACKS = 4;
    static constexpr unsigned MASTER = 0;
    static constexpr unsigned CUE = 1;


    std::reference_wrapper<Parameter> level[OUT_TRACKS];
    Parameter &pan;
    Parameter &gain;
    Parameter &mute;
    Parameter &solo;
    Parameter &cue;
    Parameter &ac;

    // currently cannot be changed in ui
    bool dummy_=false;
    unsigned follows_=0; // dummy
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

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    TrackData &inputTrack(unsigned t) { return t < IN_T_MAX ? *inTracks_[t] : *inTracks_[IN_T_MAX - 1]; }

    TrackData &outputTrack(unsigned t) { return t < OUT_T_MAX ? *outTracks_[t] : *outTracks_[OUT_T_MAX - 1]; }

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

    //TODO - clear up  OUT_T_MAX/2 = number of stereo channels vs O_MAX audio channels
    static constexpr unsigned IN_T_MAX = I_MAX;
    static constexpr unsigned OUT_T_MAX = O_MAX;

    unsigned numInTracks() { return IN_T_MAX; }

    unsigned numOutTracks() { return OUT_T_MAX; }

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

    static Colour getIconColour() { return juce::Colours::yellow;}

private:

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    inline void dcBlock(float x, float &x1, float &y, float &y1) {
        // y[n] = x[n] - x[n-1] + a * y[n-1]
        // example usage
        // dcBlock(curSample, lastIn, curOut, lastOut)
        static constexpr float a = 0.995f;
        y = x - x1 + a * y1;
        y1 = y;
        x1 = x;
    }


    std::vector<std::unique_ptr<TrackData>> inTracks_;
    std::vector<std::unique_ptr<TrackData>> outTracks_;
    void initTracks();
    AudioSampleBuffer inputBuffers_;
    AudioSampleBuffer outputBuffers_;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


