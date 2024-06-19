#pragma once

#include <algorithm>
#include <atomic>

#include "SSPExApi.h"
#include "ssp/BaseProcessor.h"
#include "Module.h"

using namespace juce;


namespace ID {
#define PARAMETER_ID(str) constexpr const char *str{ #str };
constexpr const char *separator{ ":" };

PARAMETER_ID(main)

#undef PARAMETER_ID
}  // namespace ID

class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                             AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }


    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) { props.addBus(true, getInputBusName(i), AudioChannelSet::mono()); }
        for (auto i = 0; i < O_MAX; i++) { props.addBus(false, getOutputBusName(i), AudioChannelSet::mono()); }
        return props;
    }


    enum { M_LEFT, M_RIGHT, M_MAX };


    static constexpr unsigned I_MAX = 16;
    static constexpr unsigned O_MAX = 4;
    static constexpr unsigned MAX_IN = I_MAX / M_MAX;
    static constexpr unsigned MAX_OUT = O_MAX / M_MAX;

    const std::string &getLoadedPlugin(unsigned /*t*/, unsigned m) { return modules_[m].pluginName_; };

    SSPExtendedApi::PluginEditorInterface *getEditor(unsigned /*tidx*/, unsigned midx) {
        auto &m = modules_[midx];

        if (m.plugin_ != nullptr && m.editor_ == nullptr) {
            m.editor_ = (SSPExtendedApi::PluginEditorInterface *)m.plugin_->getEditor();
        }
        return m.editor_;
    };

    SSPExtendedApi::PluginInterface *getPlugin(unsigned /*tidx*/, unsigned m) { return modules_[m].plugin_; };

    SSPExtendedApi::PluginDescriptor *getDescriptor(unsigned /*tidx*/, unsigned m) { return modules_[m].descriptor_; };

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;

    const std::vector<ModuleDesc> &getSupportedModules() { return supportedModules_; }

    bool requestModuleChange(unsigned t, unsigned m, const std::string &mn);
    void loadSupportedModules(bool forceScan = false);

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override { return true; }

    std::vector<ModuleDesc> supportedModules_;

    Module modules_[M_MAX];
    bool loadModule(std::string, Module &m);

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    static bool checkPlugin(const std::string &f);
    static std::string getPluginFile(const std::string &m);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
