#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (select)
PARAMETER_ID (slew)

// tree volt:layer:val
PARAMETER_ID (volt)
PARAMETER_ID (layer)
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
        I_SIG_A,
        I_SIG_B,
        I_SIG_C,
        I_SIG_D,
        I_SIG_E,
        I_SIG_F,
        I_SIG_G,
        I_SIG_H,
        I_SIG_I,
        I_SIG_J,
        I_SIG_K,
        I_SIG_L,
        I_SIG_M,
        I_SIG_N,
        I_SIG_O,
        I_SIG_P,
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
        O_SIG_I,
        O_SIG_J,
        O_SIG_K,
        O_SIG_L,
        O_SIG_M,
        O_SIG_N,
        O_SIG_O,
        O_SIG_P,
        O_MAX
    };

    static constexpr unsigned MAX_SIG_IN = (I_SIG_P - I_SIG_A) + 1;
    static constexpr unsigned MAX_SIG_OUT = (O_SIG_P - O_SIG_A) + 1;

    static constexpr unsigned MAX_LAYERS = 16;

    struct VoltParam {
        using Parameter = juce::RangedAudioParameter;
        VoltParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned id);
        Parameter &val;
    };

    struct Layer {
        using Parameter = juce::RangedAudioParameter;
//        Layer(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned id);
        std::vector<std::unique_ptr<VoltParam>> volts_;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &slew;
        Parameter &select;

        std::vector<std::unique_ptr<Layer>> layers_;
    } params_;

    Layer& getLayer(unsigned layer) { assert(layer < params_.layers_.size()); return * (params_.layers_[layer]);}

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


    float getCurrentVolt(float layer, unsigned volt);

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

    float lastVolt_[MAX_SIG_OUT];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


