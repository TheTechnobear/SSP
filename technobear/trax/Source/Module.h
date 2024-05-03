#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "SSPExApi.h"


struct ModuleDesc {
    std::string name;
    std::string description;
    std::vector<std::string> categories;
};

struct Module {
    explicit Module();
    void alloc(const std::string &f, SSPExtendedApi::PluginInterface *p, SSPExtendedApi::PluginDescriptor *d, void *h);
    void free();

    void prepare(int sampleRate, int blockSize);
    void process(juce::AudioSampleBuffer& );

    bool loadModule(std::string);
    static std::string getPluginFile(const std::string& m);
    static bool checkPlugin(const std::string &f,ModuleDesc &md);
    static void scanPlugins(std::vector<ModuleDesc>& supportedModules);

    juce::AudioSampleBuffer audioBuffer_;

    std::string pluginName_;
    std::string requestedModule_;
    std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
    SSPExtendedApi::PluginInterface *plugin_ = nullptr;
    SSPExtendedApi::PluginEditorInterface *editor_ = nullptr;
    SSPExtendedApi::PluginDescriptor *descriptor_ = nullptr;
    void *dlHandle_ = nullptr;
};
