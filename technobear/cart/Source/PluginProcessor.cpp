#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned ln, StringRef id, unsigned n) {
    static const char *layerID[] = {
        "x", "y", "c"
    };
    String pid = pre + String(ID::separator) + layerID[ln] +String(ID::separator) + id + String(ID::separator) + String(n);
    return pid;
}


PluginProcessor::LayerStep::LayerStep(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned ln, unsigned sn) :
    cv(*apvt.getParameter(getPID(pre, ln, ID::cv, sn))),
    access(*apvt.getParameter(getPID(pre, ln, ID::access, sn))),
    gate(*apvt.getParameter(getPID(pre, ln, ID::gate, sn))),
    glide(*apvt.getParameter(getPID(pre, ln, ID::glide, sn))),
    snake(*apvt.getParameter(getPID(pre, ln, ID::snake, sn))),
    fun(*apvt.getParameter(getPID(pre, ln, ID::fun, sn))),
    quant(*apvt.getParameter(getPID(pre, ln, ID::quant, sn))) {
    ;
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) {
    for (unsigned i = 0; i < 3; i++) {
        auto layer = std::make_unique<Layer>();
        for (unsigned n = 0; n < 16; n++) {
            layer->steps_.push_back(std::make_unique<LayerStep>(apvt, ID::layer, i, n));
        }
        layers_.push_back(std::move(layer));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    auto lgs = std::make_unique<AudioProcessorParameterGroup>(ID::layer, "Layers", ID::separator);
    for (unsigned ln = 0; ln < 3; ln++) {
        static const char *layerID[] = {
            "x", "y", "c"
        };
        auto lg = std::make_unique<AudioProcessorParameterGroup>(layerID[ln], layerID[ln], ID::separator);
        for (unsigned n = 0; n < 16; n++) {
            lg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::layer, ln, ID::cv, n), "CV", -5.0f, 5.0f, 0.0f));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::layer, ln, ID::access, n), "access", true));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::layer, ln, ID::gate, n), "gate", false));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::layer, ln, ID::glide, n), "glide", false));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::layer, ln, ID::fun, n), "fun", false));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::layer, ln, ID::quant, n), "quant", true));
        }
        lgs->addChild(std::move(lg));
    }
    params.add(std::move(lgs));

    return params;
}

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


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "X CLK",
        "X MOD",
        "X CV",
        "Y CLK",
        "Y MOD",
        "Y CV",
        "Z MOD",
        "Z CV"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "X CV",
        "X GATE",
        "Y CV",
        "Y GATE",
        "C CV",
        "C GATE"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

#if 0
    if (params_.freeze.getValue() > 0.5f) return;


    unsigned n = buffer.getNumSamples();
    unsigned tidx = params_.t_scale.convertFrom0to1((params_.t_scale.getValue()));
    float t = timeSpecs[tidx].v_;

    float tS = t * getSampleRate() / DIV_RES;

    float nextSample = lastSample_ + tS;
    float endSample = sampleCounter_ + n;


    if (nextSample - sampleCounter_ < -1) {
        // happens when we change timebase from slow to fast
        lastSample_ = sampleCounter_;
        nextSample = sampleCounter_;
    }

    while (nextSample < endSample) {
        float s = nextSample - sampleCounter_;

        if (s >= 0 && s < n - 1) {
            unsigned i0 = s, i1 = i0 + 1;
            float f1 = s - i0, f0 = 1.0f - f1;

            jassert(f0 >= 0.0f && f0 <= 1.0f);
            jassert(f1 >= 0.0f && f1 <= 1.0f);
            DataMsg msg;
            msg.sample_[0] = buffer.getSample(I_SIG_A, i0) * f0 + buffer.getSample(I_SIG_A, i1) * f1;
            msg.sample_[1] = buffer.getSample(I_SIG_B, i0) * f0 + buffer.getSample(I_SIG_B, i1) * f1;
            msg.sample_[2] = buffer.getSample(I_SIG_C, i0) * f0 + buffer.getSample(I_SIG_C, i1) * f1;
            msg.sample_[3] = buffer.getSample(I_SIG_D, i0) * f0 + buffer.getSample(I_SIG_D, i1) * f1;
            msg.trig_ = buffer.getSample(I_TRIG, i0) * f0 + buffer.getSample(I_TRIG, i1) * f1;
            if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
            lastSample_ = nextSample;
        } else if (s < 0) {
            unsigned i1 = 0;
            float f1 = s + 1.0f, f0 = 1.0f - f1;
            jassert(f0 >= 0.0f && f0 <= 1.0f);
            jassert(f1 >= 0.0f && f1 <= 1.0f);
            DataMsg msg;
            msg.sample_[0] = lastS_[0] * f0 + buffer.getSample(I_SIG_A, i1) * f1;
            msg.sample_[1] = lastS_[1] * f0 + buffer.getSample(I_SIG_B, i1) * f1;
            msg.sample_[2] = lastS_[2] * f0 + buffer.getSample(I_SIG_C, i1) * f1;
            msg.sample_[3] = lastS_[3] * f0 + buffer.getSample(I_SIG_D, i1) * f1;
            msg.trig_ = lastS_[4] * f0 + buffer.getSample(I_TRIG, i1) * f1;
            if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
            lastSample_ = nextSample;
        } else { // i1==n
            if (s + tS < n) {
                unsigned i1 = n - 1;
                DataMsg msg;
                msg.sample_[0] = buffer.getSample(I_SIG_A, i1);
                msg.sample_[1] = buffer.getSample(I_SIG_B, i1);
                msg.sample_[2] = buffer.getSample(I_SIG_C, i1);
                msg.sample_[3] = buffer.getSample(I_SIG_D, i1);
                msg.trig_ = buffer.getSample(I_TRIG, i1);
                if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
                lastSample_ = nextSample;
            } else {
                unsigned i0 = n - 1;
                lastS_[0] = buffer.getSample(I_SIG_A, i0);
                lastS_[1] = buffer.getSample(I_SIG_B, i0);
                lastS_[2] = buffer.getSample(I_SIG_C, i0);
                lastS_[3] = buffer.getSample(I_SIG_D, i0);
                lastS_[4] = buffer.getSample(I_TRIG, i0);
            }
        }
        nextSample += tS;
    }

    sampleCounter_ = endSample;
#endif
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


