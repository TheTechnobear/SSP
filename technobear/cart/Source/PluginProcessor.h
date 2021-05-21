#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


// layers e.g layer.x.param
PARAMETER_ID (layer)

// layer names
PARAMETER_ID (x)
PARAMETER_ID (y)
PARAMETER_ID (c)

// layer params
PARAMETER_ID (cv)
PARAMETER_ID (access)
PARAMETER_ID (gate)
PARAMETER_ID (glide)
PARAMETER_ID (snake)
PARAMETER_ID (fun)
PARAMETER_ID (quant)

#undef PARAMETER_ID
}


//layer
//x
//y
//c
//cv
//access
//gate
//glide
//snake
//fun
//quant



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
        I_X_CLK,
        I_X_MOD,
        I_X_CV,
        I_Y_CLK,
        I_Y_MOD,
        I_Y_CV,
        I_Z_MOD,
        I_Z_CV,
        I_MAX
    };
    enum {
        O_X_CV,
        O_X_GATE,
        O_Y_CV,
        O_Y_GATE,
        O_C_CV,
        O_C_GATE,
        O_MAX
    };


    struct LayerStep {
        using Parameter = juce::RangedAudioParameter;
        LayerStep(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned ln, unsigned sn);
        Parameter &cv;
        Parameter &access;
        Parameter &gate;
        Parameter &glide;
        Parameter &snake;
        Parameter &fun;
        Parameter &quant;
    };



    struct Layer {
        std::vector<std::unique_ptr<LayerStep>> steps_;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        std::vector<std::unique_ptr<Layer>> layers_;

        enum LAYER {
            X,
            Y,
            C,
            MAX_LAYER
        };
    } params_;


    static constexpr unsigned MAX_LAYERS=PluginParams::MAX_LAYER;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)


};


