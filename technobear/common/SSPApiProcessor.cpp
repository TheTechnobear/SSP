//#include "ssp/EditorHost.h"

// This has to be inline since it uses PluginProcessor which is part of the Plugin Project
SSP_PluginInterface::SSP_PluginInterface(PluginProcessor *p) : processor_(p), editor_(nullptr) {
#ifdef JUCE_DEBUG
   juceInitialiser_ = new  ScopedJuceInitialiser_GUI();
#endif
}

SSP_PluginInterface::~SSP_PluginInterface() {
    if (editor_) delete editor_;
    if (processor_) delete processor_;
    if(juceInitialiser_) delete juceInitialiser_;
}

// editor interaction
Percussa::SSP::PluginEditorInterface *SSP_PluginInterface::getEditor() {
    if (editor_ == nullptr) {
        ssp::EditorHost *pluginEditor = static_cast<ssp::EditorHost *>( processor_->createEditor());
        editor_ = new SSP_PluginEditorInterface(pluginEditor);
    }
    return editor_;
}

void SSP_PluginInterface::buttonPressed(int n, bool val) {
    if (editor_) editor_->buttonPressed(n, val);
}

void SSP_PluginInterface::encoderPressed(int n, bool val) {
    if (editor_) editor_->encoderPressed(n, val);
}

void SSP_PluginInterface::encoderTurned(int n, int val) {
    if (editor_) editor_->encoderTurned(n, val);
}

void SSP_PluginInterface::inputEnabled(int n, bool val) {
    processor_->onInputChanged(n, val);
}


// processor interaction
void SSP_PluginInterface::outputEnabled(int n, bool val) {
    processor_->onOutputChanged(n, val);
}

void SSP_PluginInterface::getState(void **buffer, size_t *size) {
    MemoryBlock state;
    processor_->getStateInformation(state);
    *size = state.getSize();
    *buffer = new char[*size];
    state.copyTo(*buffer, 0, *size);
}

void SSP_PluginInterface::setState(void *buffer, size_t size) {
    processor_->setStateInformation(buffer, size);
}

void SSP_PluginInterface::prepare(double sampleRate, int samplesPerBlock) {
    unsigned numIn = processor_->getBusCount(true);
    unsigned numOut = processor_->getBusCount(false);
    processor_->setRateAndBufferSizeDetails(sampleRate,samplesPerBlock);
//    processor_->setPlayConfigDetails(
//        numIn,
//        numOut,
//        sampleRate,
//        samplesPerBlock);

    processor_->prepareToPlay(sampleRate, samplesPerBlock);
}

void SSP_PluginInterface::process(float **channelData, int numChannels, int numSamples) {
    MidiBuffer midiBuffer;
    AudioSampleBuffer buffer(channelData, numChannels, numSamples);
    processor_->processBlock(buffer, midiBuffer);
}

void SSP_PluginInterface::useCompactUI(bool b) {
    return processor_->useCompactUI(b);
}
