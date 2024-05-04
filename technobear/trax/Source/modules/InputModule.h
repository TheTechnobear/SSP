#pragma once


#include "SSPExApi.h"


namespace InputModule {

class PluginDescriptor : public SSPExtendedApi::PluginDescriptor {
public:
    explicit PluginDescriptor() {
        name = "IN";
        descriptiveName = "Input";
        manufacturerName = "TB";
        version = "1.0";
        fileOrIdentifier = "";
        uid = 0;
        inputChannelNames.clear();
        for (int i = 0; i < 8; i++) { outputChannelNames.push_back(std::string("In ") + std::to_string(i)); }
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

    unsigned numberOfParameters() override { return 0;}
    bool parameterDesc(unsigned idx, ParameterDesc& desc) override { return false;}
    float parameterValue(unsigned idx) override { return 0.f;}
    bool parameterValue(unsigned idx, float v) override { return false;}
    


    static SSPExtendedApi::PluginDescriptor* createDescriptor() { return new InputModule::PluginDescriptor(); }
};


}  // namespace InputModule
