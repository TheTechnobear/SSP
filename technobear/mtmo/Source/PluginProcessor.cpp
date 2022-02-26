
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


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    dummy(*apvt.getParameter(ID::dummy)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::dummy, "dummy", false));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    switch (channelIndex) {
        case O_DUMMY_L:
            return "DummyL";
        case O_DUMMY_R:
            return "DummyR";
        default:;
    }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
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
            auto diff = msg.getTimeStamp() - clockTs_;
            if (diff > 0.025f) { // 25 millis (twice rate of ui)
                if (messageQueue_.try_enqueue(msg)) {
                    clockTs_ = msg.getTimeStamp();
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

