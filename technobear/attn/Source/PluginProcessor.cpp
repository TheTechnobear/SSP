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
    for (int i = 0; i < MAX_SIG_OUT; i++) {
        lastCV_[i] = 0.0f;
    }
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::AttnParam::AttnParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    val(*apvt.getParameter(getPID(pre, sn, ID::val))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    slew(*apvt.getParameter(ID::slew)) {
    for (unsigned i = 0; i < MAX_SIG_IN; i++) {
        attnparams_.push_back(std::make_unique<AttnParam>(apvt, ID::attn, i));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::slew, "Slew", false));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::attn, "Attn", ID::separator);
    for (unsigned sn = 0; sn < MAX_SIG_IN; sn++) {
        char ar[2];
        ar[0]='A'+sn;ar[1]=0;
        String desc = "Attn " + String(ar);
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::attn, sn, ID::val), desc, 0.0f, 1.0f, 1.0f));
    }
    params.add(std::move(sg));

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


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    bool slew = params_.slew.getValue() > 0.5f;

    for (int i = 0; i < O_MAX; i++) {
        if (!isOutputEnabled(O_SIG_A + i)) continue;

        bool in = isInputEnabled(I_SIG_A + i);
        float attn = params_.attnparams_[i]->val.getValue();

        for (int smp = 0; smp < sz; smp++) {
            auto & lv=lastCV_[i];
            float sigv = in ? buffer.getSample(I_SIG_A + i, smp) : 1.0f; // normalise to 1
            float val = sigv * attn;
            if (slew) {
                static constexpr float slewRate = (1.0f / 128.0f);
                val =  lv + ( ( val - lv) * slewRate);
            }
            buffer.setSample(O_SIG_A + i, smp, val);
            lv = val;
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


