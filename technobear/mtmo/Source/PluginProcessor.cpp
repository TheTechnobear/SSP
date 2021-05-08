
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
    blockCounter_ = 0;
    sendClock_ = false;
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
    int clockFreq = getSampleRate() * (50 / 1000);
    blockCounter_ += buffer.getNumSamples();
    if (blockCounter_ > clockFreq) {
        blockCounter_ = 0;
        sendClock_ = true;
    }
}


void PluginProcessor::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &msg) {
    BaseProcessor::handleIncomingMidiMessage(source, msg);
    if (midiChannel_ == 0 || msg.getChannel() == midiChannel_) {
//        Logger::writeToLog("handleIncomingMidiMessage -> " + msg.getDescription());
        if (msg.isNoteOnOrOff() || msg.isController() || msg.isPitchWheel()
            || msg.isAftertouch() || msg.isChannelPressure()
            ) {
            if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
        } else if (msg.isMidiClock()) {
            if (sendClock_) {
                if (messageQueue_.try_enqueue(msg)) {
                    sendClock_ = true;
                }
            }
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


