#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "SSPExApi.h"

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};

PARAMETER_ID (main)

#undef PARAMETER_ID
}

class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }


    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) {
            props.addBus(true, getInputBusName(i), AudioChannelSet::mono());
        }
        for (auto i = 0; i < O_MAX; i++) {
            props.addBus(false, getOutputBusName(i), AudioChannelSet::mono());
        }
        return props;
    }

    static constexpr unsigned I_MAX = 16;
    static constexpr unsigned O_MAX = 4;
    static constexpr unsigned MAX_MODULES = 2;
    static constexpr unsigned MAX_IN = I_MAX / MAX_MODULES;
    static constexpr unsigned MAX_OUT = O_MAX / MAX_MODULES;

    const std::string &getPluginFile(unsigned m) { return modules_[m].pluginFile_; };

    SSPExtendedApi::PluginEditorInterface *getEditor(unsigned midx) {
        auto &m = modules_[midx];

        if (m.plugin_ != nullptr && m.editor_ == nullptr) {
            m.editor_ = (SSPExtendedApi::PluginEditorInterface *) m.plugin_->getEditor();
        }
        return m.editor_;
    };

    SSPExtendedApi::PluginInterface *getPlugin(unsigned m) { return modules_[m].plugin_; };

    SSPExtendedApi::PluginDescriptor *getDescriptor(unsigned m) { return modules_[m].descriptor_; };

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;

    const std::vector<std::string> &getSupportedModules() { return supportedModules_; }

    bool requestModuleChange(unsigned midx, const std::string &f);
    void scanPlugins();

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    static bool checkPlugin(const std::string &f);
private:

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    std::vector<std::string> supportedModules_;


    struct Module {
        void alloc(
            const std::string &f,
            SSPExtendedApi::PluginInterface *p,
            SSPExtendedApi::PluginDescriptor *d,
            void *h);
        void free();

        std::string pluginFile_;
        std::string requestedModule_;
        std::atomic_flag lockModule_ = ATOMIC_FLAG_INIT;
        SSPExtendedApi::PluginInterface *plugin_ = nullptr;
        SSPExtendedApi::PluginEditorInterface *editor_ = nullptr;
        SSPExtendedApi::PluginDescriptor *descriptor_ = nullptr;
        void *dlHandle_ = nullptr;
        AudioSampleBuffer audioSampleBuffer_;
    };

    Module modules_[MAX_MODULES];
    bool loadModule(std::string, Module &m);

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


