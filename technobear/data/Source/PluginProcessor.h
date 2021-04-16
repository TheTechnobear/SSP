#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include <readerwriterqueue.h>

// t_scale
// a_y_scale
// a_y_offset
// b_y_scale
// b_y_offset
// c_y_scale
// c_y_offset
// d_y_scale
// d_y_offset
// a_show
// b_show
// d_show
// d_show
// ab_xy
// cd_xy



namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (t_scale)
PARAMETER_ID (ab_xy)
PARAMETER_ID (cd_xy)
PARAMETER_ID(sig)

// sig tree
PARAMETER_ID (y_scale)
PARAMETER_ID (y_offset)
PARAMETER_ID (show)

#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override {}

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    enum {
        I_SIG_A,
        I_SIG_B,
        I_SIG_C,
        I_SIG_D,
        I_TRIG,
        I_MAX
    };
    enum {
        O_SIG_A,
        O_SIG_B,
        O_SIG_C,
        O_SIG_D,
        O_MAX
    };

    static constexpr unsigned MAX_SIG_IN = (I_SIG_D - I_SIG_A) + 1;
    static constexpr unsigned MAX_SIG_OUT = (O_SIG_D - O_SIG_A) + 1;

    struct SigParams {
        using Parameter = juce::RangedAudioParameter;
        SigParams(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn);
        Parameter &y_scale;
        Parameter &y_offset;
        Parameter &show;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &t_scale;

        Parameter &ab_xy;
        Parameter &cd_xy;

        std::vector<std::unique_ptr<SigParams>> sigparams_;
    } params_;


    static constexpr float  MAX_FREQ = 24000.f;
    static constexpr float  UI_FREQ = 50.f ; // ms
    static constexpr float  OVERHEAD = 2.f ; // factor
    static constexpr unsigned MAX_MSGS = unsigned((MAX_FREQ / 1000.f * UI_FREQ) * OVERHEAD) ;
    struct DataMsg {
        unsigned count_=0;
        float sample_[4];
    };

    moodycamel::ReaderWriterQueue<DataMsg>& messageQueue() { return messageQueue_;}

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;


private:
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

    // processor updates
    // std::atomic<unsigned> lastInIdx_;
    // std::atomic<unsigned> lastOutIdx_;

    // unsigned inCount_;
    // unsigned outCount_;

    // AudioSampleBuffer lastBuffer_;
    // AudioSampleBuffer inputBuffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)

    moodycamel::ReaderWriterQueue<DataMsg> messageQueue_;

};


