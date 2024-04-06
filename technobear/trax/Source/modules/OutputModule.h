#pragma once

#include "SSPExApi.h"

namespace OutputModule {

class PluginDescriptor : public SSPExtendedApi::PluginDescriptor {
public:
    explicit PluginDescriptor() {
        name = "OUT";
        descriptiveName = "Output";
        manufacturerName = "TB";
        version = "1.0";
        fileOrIdentifier = "";
        uid = 0;
        for (int i = 0; i < 2; i++) { inputChannelNames.push_back(std::string("Out ") + std::to_string(i)); }
        outputChannelNames.clear();
        colour = 0;

        supportCompactUI_ = false;
    }
};

class PluginInterface : public SSPExtendedApi::PluginInterface {
public:
    explicit PluginInterface() = default;

    SSPExtendedApi::PluginEditorInterface* getEditor() override { return nullptr; }
    void buttonPressed(int n, bool val) override {}
    void encoderPressed(int n, bool val) override {}
    void encoderTurned(int n, int val) override {}
    void inputEnabled(int n, bool val) override {}
    void outputEnabled(int n, bool val) override {}
    void getState(void** buffer, size_t* size) override {*buffer = nullptr, *size = 0;}
    void setState(void* buffer, size_t size) override {}
    void prepare(double sampleRate, int samplesPerBlock) override {};
    void process(float** channelData, int numChannels, int numSamples) override {}
    void useCompactUI(bool b) override {};


    static SSPExtendedApi::PluginDescriptor* createDescriptor() { return new OutputModule::PluginDescriptor(); }
};


}  // namespace OutputModule
