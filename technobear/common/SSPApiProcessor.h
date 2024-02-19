#pragma once

#include "../../ssp-sdk/Percussa.h"

#include "SSPApiEditor.h"

class SSP_PluginInterface : public SSPExtendedApi::PluginInterface {
public:
    explicit SSP_PluginInterface(PluginProcessor *p);
    ~SSP_PluginInterface() override;

    Percussa::SSP::PluginEditorInterface *getEditor() override;

    void buttonPressed(int n, bool val) override;
    void encoderPressed(int n, bool val) override;
    void encoderTurned(int n, int val) override;
    void inputEnabled(int n, bool val) override;
    void outputEnabled(int n, bool val) override;
    void getState(void **buffer, size_t *size) override;
    void setState(void *buffer, size_t size) override;

    void prepare(double sampleRate, int samplesPerBlock) override;
    void process(float **channelData, int numChannels, int numSamples) override;

    // extended API
    void useCompactUI(bool b) override;

private:
    SSP_PluginEditorInterface *editor_ = nullptr;
    PluginProcessor *processor_ = nullptr;

    ScopedJuceInitialiser_GUI* juceInitialiser_ = nullptr;
};

#include "SSPApiProcessor.cpp"

