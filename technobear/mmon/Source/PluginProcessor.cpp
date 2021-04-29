
#include "PluginProcessor.h"
#include "PluginEditor.h"


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
#if __APPLE__
    for(unsigned i=0;i<I_MAX;i++) inputEnabled[i]=false;
    for(unsigned i=0;i<O_MAX;i++) outputEnabled[i]=false;
#endif

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    // params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_1a, "VCA 1A", -4.0f, 4.0f, 0.0f));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    if(!midiMessages.isEmpty()) {
        Logger::writeToLog("processBlock :: midi msg " + String(midiMessages.getNumEvents()));
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


