#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include <readerwriterqueue.h>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (t_scale)
PARAMETER_ID (trig_src)
PARAMETER_ID (trig_lvl)
PARAMETER_ID (freeze)
PARAMETER_ID (ab_xy)
PARAMETER_ID (cd_xy)

// sig tree sig.y_scale etc
PARAMETER_ID(sig)
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
        Parameter &trig_src;
        Parameter &trig_lvl;

        Parameter &freeze;
        Parameter &ab_xy;
        Parameter &cd_xy;

        std::vector<std::unique_ptr<SigParams>> sigparams_;
    } params_;


    struct DataMsg {
        float sample_[MAX_SIG_IN];
        float trig_;
    };

    static constexpr float DIV_RES = 50.f; // resolution per division
    static constexpr unsigned DIV_COUNT = 10; // number of divisions
    static constexpr unsigned MAX_ENTRY = DIV_RES * DIV_COUNT;
    static constexpr float OVERHEAD = 6.f; // factor  , need to allow for 50ms data for ui
    static constexpr unsigned MAX_MSGS = unsigned(MAX_ENTRY * OVERHEAD);

    moodycamel::ReaderWriterQueue<DataMsg> &messageQueue() { return messageQueue_; }

    void prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) override;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

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

    float sampleCounter_ = 0.0f;
    float timeSpec_ = 0.0f;
    float lastS_[MAX_SIG_IN + 1] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // inc trig

    float backoffTs_=0.0f;

    moodycamel::ReaderWriterQueue<DataMsg> messageQueue_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


