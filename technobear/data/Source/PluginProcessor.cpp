#include "PluginProcessor.h"
#include "PluginEditor.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()), messageQueue_(MAX_MSGS) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::SigParams::SigParams(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    y_offset(*apvt.getParameter(getPID(pre, sn, ID::y_offset))),
    y_scale(*apvt.getParameter(getPID(pre, sn, ID::y_scale))),
    show(*apvt.getParameter(getPID(pre, sn, ID::show))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    t_scale(*apvt.getParameter(ID::t_scale)),
    freeze(*apvt.getParameter(ID::freeze)),
    ab_xy(*apvt.getParameter(ID::ab_xy)),
    cd_xy(*apvt.getParameter(ID::cd_xy)) {
    for (unsigned i = 0; i < MAX_SIG_IN; i++) {
        sigparams_.push_back(std::make_unique<SigParams>(apvt, ID::sig, i));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);


    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::t_scale, "Time", 1.0f, 1000.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::freeze, "Freeze", false));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::ab_xy, "AB XY", false));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::cd_xy, "CD XY", false));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::sig, "Signal", ID::separator);
    for (unsigned sn = 0; sn < MAX_SIG_IN; sn++) {
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::sig, sn, ID::y_scale), "Scale", -5.0f, 5.0f, 1.0f));
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::sig, sn, ID::y_offset), "Offset", -5.0f, 5.0f, 0.0f));
        sg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::sig, sn, ID::show), "Visible", true));
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
        "Trig"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out A",
        "Out B",
        "Out C",
        "Out D"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if (params_.freeze.getValue() > 0.5f) return;

    unsigned n = buffer.getNumSamples();
    unsigned long tS = params_.t_scale.convertFrom0to1((params_.t_scale.getValue()));

    for (unsigned i = 0; i < n; i++) {

        if ( tS==1 || (sampleCounter_ + i) % tS == 0) {
            DataMsg msg;
            msg.sample_[0] = buffer.getSample(I_SIG_A, i);
            msg.sample_[1] = buffer.getSample(I_SIG_B, i);
            msg.sample_[2] = buffer.getSample(I_SIG_C, i);
            msg.sample_[3] = buffer.getSample(I_SIG_D, i);
            if (!messageQueue_.try_enqueue(msg)) {
                // queue is full, perhaps editor is not consuming....\
                // or is not consuming fast enough
            }
        }
    }

    sampleCounter_ = sampleCounter_ + buffer.getNumSamples();
    static constexpr unsigned long MAX_SC = 480000 * 60 * 60; // 1 hour  @ 48k
    if(sampleCounter_ > MAX_SC ) sampleCounter_ = sampleCounter_ % MAX_SC;
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


