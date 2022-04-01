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

    enum ModMode {
        MOD_MODE_NONE,
        MOD_MODE_RESET,
        MOD_MODE_CLK,
        MOD_MODE_RUNSTP,
        MOD_MODE_DIR,
        MOD_MODE_MAX
    };

    enum CvMode {
        CV_MODE_NONE,
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

    typedef std::vector<std::unique_ptr<LayerStep>> Steps;

    struct Layer {
        using Parameter = juce::RangedAudioParameter;
        explicit Layer(AudioProcessorValueTreeState &apvt, unsigned ln);
        Steps steps_;
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
        // state
        unsigned seqStep_ = 0;

        // params
        struct {
            // snake
            unsigned snake_ = 0;

            // fun
            bool fun_op_trig_ = false;
            bool fun_op_sleep_ = false;
            ModMode fun_mod_mode_ = MOD_MODE_NONE;
            CvMode fun_cv_mode_ = CV_MODE_NONE;

            // quant
            unsigned scale_ = 0;
            unsigned root_ = 0;
        } p_;

        // current values
        unsigned pos_ = 0;
        float cv_ = 0.0f;
        bool gate_ = false;

        bool clkTrig_ = false; // did last smp trick this clock?
        float targetCv_ = 0.0f;
        bool reset_ = false;
        bool run_ = true;
        bool reverse_ = false;

        unsigned snakeOffset_ = 0; // mod_cv_snake
        unsigned seqOffset_ = 0; // mod_cv_loc
        float cvOffset_ = 0.0f; // mod_cv_add/sh

        // last values
        float lastClkIn_ = 0.0f;
        float lastModIn_ = 0.0f;
        float lastCvIn_ = 0.0f;

        //glide
        int lastClkSmpCnt_ = 0;
        float lastClkSmp_ = 0;

        unsigned gateTime_ = 0; // smps of gate left
    } layerData_[MAX_LAYERS];

    inline float normValue(RangedAudioParameter &p) { return p.convertFrom0to1(p.getValue()); }

    // control rate prep
    void prepLayer(Layer &layerParam, LayerData &ld);
    void prepCartLayer(Layer &layerParam, LayerData &ld);

    // sample rate
    void processLayer(float clkIn, float modIn, float cvIn, Steps &steps, LayerData &ld, float &cv, bool &gate);
    void processCartLayer(Steps &steps, LayerData &ld, LayerData &xld, LayerData &yld, float &cv, bool &gate);
    float quantizeCv(unsigned scale, unsigned root, float voctIn);

    unsigned advanceLayer(Steps &steps, unsigned seqstep, LayerData &ld);
    unsigned setLayerStep(Steps &steps, unsigned seqstep, LayerData &ld);


    unsigned setCartLayerX(Steps &steps, unsigned x, unsigned xOffset, LayerData &ld);
    unsigned setCartLayerY(Steps &steps, unsigned y, unsigned yOffset, LayerData &ld);
    void advanceCartLayer(Steps &steps, LayerData &ld, bool xTrig, bool yTrig);

    static Snakes snakes_;
    Quantizer quantizer_;

    static constexpr unsigned trigGateTime = 64; // samples
    static constexpr float trigLevel = 0.2f; // 1v
    static constexpr float clockLevel = trigLevel; // 1v
    static constexpr float glideRatio = 10.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


