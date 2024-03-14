#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "SSPExApi.h"

struct Module {
    void alloc(const std::string &f, SSPExtendedApi::PluginInterface *p, SSPExtendedApi::PluginDescriptor *d, void *h);
    void free();

    void prepare(int sampleRate, int blockSize);

    bool loadModule(std::string);
    static std::string getPluginFile(const std::string& m);
    static bool checkPlugin(const std::string &f);
    static void scanPlugins(std::vector<std::string>& supportedModules);

    int sampleRate_ = 48000;
    int blockSize_ = 256;

    std::string pluginName_;
    std::string requestedModule_;
    std::atomic_flag lockModule_ = ATOMIC_FLAG_INIT;
    SSPExtendedApi::PluginInterface *plugin_ = nullptr;
    SSPExtendedApi::PluginEditorInterface *editor_ = nullptr;
    SSPExtendedApi::PluginDescriptor *descriptor_ = nullptr;
    void *dlHandle_ = nullptr;
    juce::AudioSampleBuffer audioSampleBuffer_;
};
