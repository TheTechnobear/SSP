#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (source)
PARAMETER_ID (clkindiv)
PARAMETER_ID (bpm)
PARAMETER_ID (midippqn)

// tree div:val
PARAMETER_ID (div)
PARAMETER_ID (val)

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

    void prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) override;

    enum {
        I_CLK,
        I_RESET,
        I_MAX
    };
    enum {
        O_CLK_1,
        O_CLK_2,
        O_CLK_3,
        O_CLK_4,
        O_CLK_5,
        O_CLK_6,
        O_CLK_7,
        O_CLK_8,
        O_MIDI_CLK,
        O_MAX
    };

    static constexpr unsigned MAX_CLK_OUT = (O_CLK_8 - O_CLK_1) + 1;

    struct DivParam {
        using Parameter = juce::RangedAudioParameter;
        DivParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned id);
        Parameter &val;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        Parameter &source;
        Parameter &clkindiv;
        Parameter &bpm;
        Parameter &midippqn;

        std::vector<std::unique_ptr<DivParam>> divisions_;
    } params_;
    
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

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    enum MidiPPQN {
        MPPQN_24,
        MPPQN_48,
        MPPQN_96,
        MPPQN_192,
        MPPQN_MAX
    };

    enum ClkInDiv {
        CI_X1,
        CI_D2,
        CI_D4,
        CI_D8,
        CI_D16,
        CI_D32,
        CI_D64,
        CI_MAX
    };

    enum ClkOutDiv {
        CO_M64,
        CO_M32,
        CO_M16,
        CO_M8,
        CO_M4,
        CO_M2,
        CO_X1,
        CO_D2,
        CO_D4,
        CO_D8,
        CO_D16,
        CO_D32,
        CO_D64,
        CO_D128,
        CO_MAX
    };


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


