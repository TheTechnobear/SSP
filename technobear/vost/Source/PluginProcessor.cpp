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
        lastVolt_[i] = getCurrentVolt(select, i, params_.morph.getValue() > 0.5f);
    }
}

PluginProcessor::~PluginProcessor() {
}

String getLayerPid(unsigned lid) {
    return String(ID::layers) + String(ID::separator) + String(lid);
}

String getVoltPid(unsigned lid, unsigned vid) {
    String pid = getLayerPid(lid)
                 + String(ID::separator) + String(ID::volts)
                 + String(ID::separator) + String(vid)
                 + String(ID::separator) + String(ID::val);
    return pid;
}


PluginProcessor::VoltParam::VoltParam(AudioProcessorValueTreeState &apvt, unsigned lid, unsigned id) :
    val(*apvt.getParameter(getVoltPid(lid, id))),
    id_(id), lid_(lid), pid_(getVoltPid(lid, id)) {

}


PluginProcessor::Layer::Layer(AudioProcessorValueTreeState &, unsigned id) :
    id_(id), pid_(getLayerPid(id)) {

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    slew(*apvt.getParameter(ID::slew)),
    select(*apvt.getParameter(ID::select)),
    morph(*apvt.getParameter(ID::morph)) {
    for (unsigned lid = 0; lid < MAX_LAYERS; lid++) {
        auto layer = std::make_unique<Layer>(apvt, lid);
        for (unsigned vid = 0; vid < MAX_SIG_IN; vid++) {
            layer->volts_.push_back(std::make_unique<VoltParam>(apvt, lid, vid));
        }
        layers_.push_back(std::move(layer));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::slew, "Slew", false));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::morph, "Morph", false));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::select, "Select", 0, float(MAX_LAYERS), 0.0f));


    auto layers = std::make_unique<AudioProcessorParameterGroup>(ID::layers, String(ID::layers), ID::separator);
    for (unsigned lid = 0; lid < MAX_LAYERS; lid++) {
        char la[2];
        la[0] = 'A' + lid;
        la[1] = 0;
        String desc = "Layer " + String(la);

        auto layer = std::make_unique<AudioProcessorParameterGroup>(getLayerPid(lid), getLayerPid(lid), ID::separator);
        for (unsigned vid = 0; vid < MAX_SIG_IN; vid++) {
            char ar[2];
            ar[0] = 'A' + vid;
            ar[1] = 0;
            String desc = "Volt " + String(ar);
            layer->addChild(std::make_unique<ssp::BaseFloatParameter>(getVoltPid(lid, vid), desc, -1.0f, 1.0f, 0.0f));
        }

        layers->addChild(std::move(layer));
    }
    params.add(std::move(layers));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "Select",
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


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    for (int i = 0; i < MAX_SIG_OUT; i++) {
        float select = (params_.select.getValue() * MAX_LAYERS);
        lastVolt_[i] = getCurrentVolt(select, i, params_.morph.getValue() > 0.5f);
    }
}


float PluginProcessor::getCurrentVolt(float layer, unsigned volt, bool morph) {
    float vout = 0.0f;
    if (morph) {
        float l1 = std::min(floorf(layer), float(MAX_LAYERS) - 1.0f);
        float l2 = std::min(l1 + 1.0f, float(MAX_LAYERS) - 1.0f);

        float m = layer - l1;
        auto &layer1 = params_.layers_[l1];
        auto &layer2 = params_.layers_[l2];

        float l1v = ((layer1->volts_[volt]->val.getValue() * 2.0f) - 1.0f) * (1.0f - m);
        float l2v = ((layer2->volts_[volt]->val.getValue() * 2.0f) - 1.0f) * m;
        vout = l1v + l2v;
    } else {
        // take nearest layer
        unsigned l1 = floorf(layer + 0.5f);
        l1 = std::max(std::min(l1, MAX_LAYERS - 1), unsigned(0));
        assert(l1 < params_.layers_.size());
        auto &layer1 = params_.layers_[l1];
        vout = ((layer1->volts_[volt]->val.getValue() * 2.0f) - 1.0f);
    }

    return vout;
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();
    bool morph = params_.morph.getValue() > 0.5f;
    bool slew = params_.slew.getValue() > 0.5f;

    float selectOffset = buffer.getSample(I_SELECT,0);
    float select = std::min(std::max(((params_.select.getValue() + selectOffset ) * MAX_LAYERS), 0.0f), float(MAX_LAYERS));

    for (int i = 0; i < O_MAX; i++) {
        if (!isOutputEnabled(O_SIG_A + i)) continue;

        bool in = isInputEnabled(I_SIG_A + i);

        float attnP = getCurrentVolt(select, i, morph);

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


