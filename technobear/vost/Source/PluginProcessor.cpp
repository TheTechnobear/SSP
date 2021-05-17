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
    float select = 0.0f;
    for (int i = 0; i < MAX_SIG_OUT; i++) {

        lastVolt_[i] = getCurrentVolt(select, i);
    }
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::VoltParam::VoltParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    val(*apvt.getParameter(getPID(pre, sn, ID::val))) {

}


//TODO
//PluginProcessor::Layer::Layer(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) {
//
//}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    slew(*apvt.getParameter(ID::slew)),
    select(*apvt.getParameter(ID::select)) {
    for (unsigned x = 0; x < MAX_LAYERS; x++) {
        //TODO
        auto layer = std::make_unique<Layer>();
//        auto layer = std::make_unique<Layer>(apvt, ID:layer, x);

        //TODO : id stuff ;) 
        for (unsigned y = 0; y < MAX_SIG_IN; y++) {
            layer->volts_.push_back(std::make_unique<VoltParam>(apvt, ID::volt, y));
        }

        //TODO :: CHECK
        layers_.push_back(std::move(layer));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::slew, "Slew", false));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::select, "Select", 0, float(MAX_LAYERS), 0.0f));


    auto lg = std::make_unique<AudioProcessorParameterGroup>(ID::volt, "Layer", ID::separator);
    for (unsigned x = 0; x < MAX_LAYERS; x++) {
        char la[2];
        la[0] = 'A' + x;
        la[1] = 0;
        String desc = "Layer " + String(la);

        //TODO : more id stuff ;)
        auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::volt, "Volt", ID::separator);
        for (unsigned sn = 0; sn < MAX_SIG_IN; sn++) {
            char ar[2];
            ar[0] = 'A' + sn;
            ar[1] = 0;
            String desc = "Volt " + String(ar);
            sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::volt, sn, ID::val), desc, -1.0f, 1.0f, 0.0f));
        }
        //TODO CHECK
        lg->addChild(std::move(sg));
    }
    params.add(std::move(lg));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In A",
        "In B",
        "In C",
        "In D",
        "In E",
        "In F",
        "In G",
        "In H",
        "In I",
        "In J",
        "In K",
        "In L",
        "In M",
        "In N",
        "In O",
        "In P"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out A",
        "Out B",
        "Out C",
        "Out D",
        "Out E",
        "Out F",
        "Out G",
        "Out H",
        "Out I",
        "Out J",
        "Out K",
        "Out L",
        "Out M",
        "Out N",
        "Out O",
        "Out P"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


float PluginProcessor::getCurrentVolt(float layer, unsigned volt) {
    //TODO :
    unsigned l1 = layer;
    unsigned l2 = layer + 1;
    float m1 = 1.0f;
    float m2 = 0.0f;
    auto &layer1 = params_.layers_[l1];
    auto &layer2 = params_.layers_[l2];
    float vout = ((layer1->volts_[volt]->val.getValue() * 2.0f) - 1.0f) * m1;
    return vout;
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    for (int i = 0; i < MAX_SIG_OUT; i++) {
        float select = 0.0f; // TODO
        lastVolt_[i] = getCurrentVolt(select, i);
    }
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    bool slew = params_.slew.getValue() > 0.5f;

    for (int i = 0; i < O_MAX; i++) {
        if (!isOutputEnabled(O_SIG_A + i)) continue;

        bool in = isInputEnabled(I_SIG_A + i);

        float select = 0.0f; // TODO

        float attnP = getCurrentVolt(select, i);

        for (int smp = 0; smp < sz; smp++) {
            auto &lP = lastVolt_[i];
            float sigv = in ? buffer.getSample(I_SIG_A + i, smp) : 1.0f; // normalise to 1
            float attn = attnP;
            if (slew) {
                static constexpr float slewRate = (1.0f / 128.0f);
                attn = lP + ((attnP - lP) * slewRate);
            }
            buffer.setSample(O_SIG_A + i, smp, sigv * attn);
            lP = attn;
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


