#pragma once
#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

using namespace juce;

//inSel
//outSel
//active
//soft

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };


PARAMETER_ID (inSel)
PARAMETER_ID (outSel)
PARAMETER_ID (active)
PARAMETER_ID (soft)
#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &inSel;
        Parameter &outSel;
        Parameter &active;
        Parameter &soft;
    } params_;

    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

public:
    enum {
        I_IN_SEL,
        I_OUT_SEL,
        I_SIG_1,
        I_SIG_2,
        I_SIG_3,
        I_SIG_4,
        I_SIG_5,
        I_SIG_6,
        I_SIG_7,
        I_SIG_8,
        I_MAX
    };
    enum {
        O_SIG_A,
        O_SIG_B,
        O_SIG_C,
        O_SIG_D,
        O_SIG_E,
        O_SIG_F,
        O_SIG_G,
        O_SIG_H,
        O_MAX
    };

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
private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    // processor updates
    std::atomic<unsigned> lastInIdx_;
    std::atomic<unsigned> lastOutIdx_;

    unsigned inCount_;
    unsigned outCount_;

    AudioSampleBuffer lastBuffer_;
    AudioSampleBuffer inputBuffer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)

public:
    void getLastSel(unsigned &in, unsigned &out) { in = lastInIdx_, out = lastOutIdx_; }

    static constexpr unsigned MAX_SIG_IN = (I_SIG_8 - I_SIG_1) + 1;
    static constexpr unsigned MAX_SIG_OUT = (O_SIG_H - O_SIG_A) + 1;

};


