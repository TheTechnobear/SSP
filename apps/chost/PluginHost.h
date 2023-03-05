#pragma once

#include "SSPExApi.h"

#include "../JuceLibraryCode/JuceHeader.h"

class PluginHost : public AudioIODeviceCallback {
public:

    explicit PluginHost(int argc, char *argv[]);
    virtual ~PluginHost();

    // AudioIODeviceCallback
    void audioDeviceIOCallback(const float **inputChannelData,
                               int numInputChannels,
                               float **outputChannelData,
                               int numOutputChannels,
                               int numSamples) override;
    void audioDeviceAboutToStart(AudioIODevice *device) override;
    void audioDeviceStopped() override;
    void audioDeviceError(const String &errorMessage) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(AudioBuffer<const float> &in, AudioBuffer<float>& out);

protected:
    void scanPlugins();
    void loadPlugin(unsigned m, const std::string &f);
    static bool checkPlugin(const std::string &f);

    std::vector<std::string> supportedModules;

    struct Module {
        void alloc(
            const std::string &f,
            SSPExtendedApi::PluginInterface *p,
            SSPExtendedApi::PluginEditorInterface *e,
            SSPExtendedApi::PluginDescriptor *d,
            void *h);
        void free();

        std::string pluginFile_;
        SSPExtendedApi::PluginInterface *plugin_ = nullptr;
        SSPExtendedApi::PluginEditorInterface *editor_ = nullptr;
        SSPExtendedApi::PluginDescriptor *descriptor_ = nullptr;
        void *dlHandle_ = nullptr;
        AudioSampleBuffer audioSampleBuffer_;
    };

    void dumpDeviceInfo();

    static constexpr unsigned MAX_MODULES = 1;

    Module modules_[MAX_MODULES];
    bool loadModule(const std::string &f, Module &m);

    unsigned MAX_IN = 8;
    unsigned MAX_OUT = 2;

    float getSampleRate() { return sampleRate_; }

    float getBlockSize() { return blockSize_; }

    float sampleRate_ = 48000.f;
    float blockSize_ = 128.f;


    AudioSampleBuffer inputBuffer_;
    AudioSampleBuffer outputBuffer_;
    AudioDeviceManager deviceManager_;

};
