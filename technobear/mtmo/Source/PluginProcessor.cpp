
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
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

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
//    if (!midiMessages.isEmpty()) {
//        Logger::writeToLog("processBlock :: midi msg " + String(midiMessages.getNumEvents()));
//    }
}


void PluginProcessor::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message){
    BaseProcessor::handleIncomingMidiMessage(source,message);
    if (!messageQueue_.try_enqueue(message)) { ; } // queue full
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


