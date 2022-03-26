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
PARAMETER_ID (glide)
PARAMETER_ID (snake)
PARAMETER_ID (fun)
PARAMETER_ID (quant)

// layer step params
PARAMETER_ID (cv)
PARAMETER_ID (access)
PARAMETER_ID (gate)


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

    static Percussa::SSP::PluginDescriptor *createDescriptor();

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
//        I_Z_MOD,
//        I_Z_CV,
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
        explicit LayerStep(AudioProcessorValueTreeState &apvt, unsigned ln, unsigned sn);
        Parameter &cv;
        Parameter &access;
        Parameter &gate;
    };


    struct Layer {
        using Parameter = juce::RangedAudioParameter;
        explicit Layer(AudioProcessorValueTreeState &apvt, unsigned ln);
        std::vector<std::unique_ptr<LayerStep>> steps_;
        Parameter &glide;
        Parameter &snake;
        Parameter &fun;
        Parameter &quant;
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


    static constexpr unsigned MAX_LAYERS = PluginParams::MAX_LAYER;


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
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    struct LayerData {
        bool lastClk_ = false;
        unsigned gateTime_ = 0; // smps of gate left
        float cv_ = 0.0f;
        bool gate_ = false;
        unsigned pos_ = 0;
    } x_, y_, c_;

    unsigned findNextStep(unsigned cpos, Layer &params);
    void advanceLayer(LayerData &ld, Layer &params);
    void advanceCartLayer(LayerData &ld, Layer &params, unsigned xPos, unsigned yPos);
    void initSnakes();

public:
    class SnakeAlgo {
    public:
        explicit SnakeAlgo() { ; }

        virtual ~SnakeAlgo() { ; }

        virtual const String &name() = 0;
        virtual unsigned findNext(unsigned cpos) = 0;
    };

private:

    std::vector<std::unique_ptr<SnakeAlgo>> snakes_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)


};


