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

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::DivParam::DivParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    val(*apvt.getParameter(getPID(pre, sn, ID::val))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    source(*apvt.getParameter(ID::source)),
    clkindiv(*apvt.getParameter(ID::clkindiv)),
    bpm(*apvt.getParameter(ID::bpm)),
    midippqn(*apvt.getParameter(ID::midippqn)) {
    for (unsigned i = 0; i < MAX_CLK_OUT; i++) {
        divisions_.push_back(std::make_unique<DivParam>(apvt, ID::div, i));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    StringArray source;
    source.add("Internal");
    source.add("Clk In");
    source.add("Midi");

    StringArray clkInDivs;
    clkInDivs.add("1");
    clkInDivs.add("1/2");
    clkInDivs.add("1/4");
    clkInDivs.add("1/8");
    clkInDivs.add("1/16");
    clkInDivs.add("1/32");
    clkInDivs.add("1/64");

    jassert(clkInDivs.size() == CI_MAX);

    StringArray midippqn;
    midippqn.add("24");
    midippqn.add("48");
    midippqn.add("96");
    midippqn.add("192");
    jassert(midippqn.size() == MPPQN_MAX);

    StringArray clkOutDivs;
    clkOutDivs.add("x 64");
    clkOutDivs.add("x 32");
    clkOutDivs.add("x 16");
    clkOutDivs.add("x 8");
    clkOutDivs.add("x 4");
    clkOutDivs.add("x 2");
    clkOutDivs.add("1");
    clkOutDivs.add("1/2");
    clkOutDivs.add("1/4");
    clkOutDivs.add("1/8");
    clkOutDivs.add("1/16");
    clkOutDivs.add("1/32");
    clkOutDivs.add("1/64");
    clkOutDivs.add("1/128");
    jassert(clkOutDivs.size() == CO_MAX);


    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::source, "Clock", source, 0));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::clkindiv, "Clk Div", clkInDivs, CI_D4));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::bpm, "Int BPM", 1.0f, 360, 120.0f));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::midippqn, "Midi PPQN", midippqn, MPPQN_24));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::div, "Divisions", ID::separator);
    for (unsigned sn = 0; sn < MAX_CLK_OUT; sn++) {
        char ar[2];
        ar[0] = 'A' + sn;
        ar[1] = 0;
        String desc = "Div " + String(ar);
        sg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getPID(ID::div, sn, ID::val), desc, clkOutDivs, (CO_X1 + sn) % clkOutDivs.size() ));
    }
    params.add(std::move(sg));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "Clk In"
        "Reset"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Clk A",
        "Clk B",
        "Clk C",
        "Clk D",
        "Clk E",
        "Clk F",
        "Clk G",
        "Clk H",
        "Midi Clk"
    };

    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

