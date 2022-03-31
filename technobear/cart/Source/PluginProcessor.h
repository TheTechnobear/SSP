#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>
#include "Quantizer.h"


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
PARAMETER_ID (snake)
PARAMETER_ID (scale)
PARAMETER_ID (root)
PARAMETER_ID (fun_op_trig)
PARAMETER_ID (fun_op_sleep)
PARAMETER_ID (fun_mod_mode)
PARAMETER_ID (fun_cv_mode)

// layer step params
PARAMETER_ID (cv)
PARAMETER_ID (access)
PARAMETER_ID (gate)
PARAMETER_ID (glide)


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

#include "Snakes.h"

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

    enum {
        MOD_MODE_RESET,
        MOD_MODE_CLK,
        MOD_MODE_RUNSTP,
        MOD_MODE_DIR,
        MOD_MODE_MAX
    };

    enum {
        CV_MODE_ADD,
        CV_MODE_LOC,
        CV_MODE_SNAKE,
        CV_MODE_SH,
        CV_MODE_MAX
    };

    struct LayerStep {
        using Parameter = juce::RangedAudioParameter;
        explicit LayerStep(AudioProcessorValueTreeState &apvt, unsigned ln, unsigned sn);
        Parameter &cv;
        Parameter &access;
        Parameter &gate;
        Parameter &glide;
    };


    struct Layer {
        using Parameter = juce::RangedAudioParameter;
        explicit Layer(AudioProcessorValueTreeState &apvt, unsigned ln);
        std::vector<std::unique_ptr<LayerStep>> steps_;
        Parameter &snake;
        Parameter &scale;
        Parameter &root;
        Parameter &fun_op_trig;
        Parameter &fun_op_sleep;
        Parameter &fun_mod_mode;
        Parameter &fun_cv_mode;
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
    static constexpr unsigned MAX_STEPS = 16;

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

    void getActiveData(unsigned &xp, unsigned &yp, unsigned &cp,
                       float &xCv, float &yCv, float &cCv,
                       bool &xGate, bool &yGate, bool &cGate) const;


protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    struct LayerData {
        unsigned pos_ = 0;
        float cv_ = 0.0f;
        bool gate_ = false;
        float glide_ = 0.0f;

        bool  clkTrig_ =false; // did last smp trick this clock?
        float cvOffset_=0.0f; // mod_cv_add/sh
        float snakeOffset_=0.0f; // mod_cv_snake
        float posOffset_=0.0f; // mod_cv_loc

        bool reset_=false;
        bool run_=true;
        bool reverse_=false;

        float lastCV_ = 0.0f;

        float lastClkIn_ = 0.0f;
        float lastModIn_ = 0.0f;
        float lastCvIn_ = 0.0f;

        // use for glide on x/y
        int lastClkSmpCnt_= 0;
        int lastClkSmp_= 0;
        int glideTime_=0;

        unsigned gateTime_ = 0; // smps of gate left
    } layerData_[MAX_LAYERS];

    unsigned findNextStep(unsigned cpos, Layer &params, bool sleep, bool rev);
    void setLayerStep(unsigned pos,LayerData &ld, Layer &params, bool sleep);
    void advanceLayer(LayerData &ld, Layer &params, bool sleep, bool rev);

    void setCartLayerX(LayerData &ld, Layer &params,unsigned pos);
    void setCartLayerY(LayerData &ld, Layer &params,unsigned pos);
    void advanceCartLayer(LayerData &ld, Layer &params, bool xTrig, bool yTrig);

    static Snakes snakes_;
    Quantizer quantizer_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


