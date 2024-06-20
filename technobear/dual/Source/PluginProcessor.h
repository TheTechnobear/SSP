#pragma once

#include <algorithm>
#include <atomic>

#include "SSPExApi.h"
#include "ssp/BaseProcessor.h"
#include "Track.h"
// #include "Module.h"

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

    static constexpr unsigned I_MAX = 16;
    static constexpr unsigned O_MAX = 4;
    static constexpr unsigned MAX_IN = I_MAX / Track::M_MAX;
    static constexpr unsigned MAX_OUT = O_MAX / Track::M_MAX;

    std::string getLoadedPlugin(unsigned t, unsigned m) {
        if (t >= MAX_TRACKS || m >= Track::M_MAX) return "";
        auto &track = tracks_[t];
        return track.modules_[m].pluginName_;
    };

    SSPExtendedApi::PluginEditorInterface *getEditor(unsigned t, unsigned m) {
        if (t >= MAX_TRACKS || m >= Track::M_MAX) return nullptr;
        auto &track = tracks_[t];
        auto &module = track.modules_[m];

        if (module.plugin_ != nullptr && module.editor_ == nullptr) {
            module.editor_ = (SSPExtendedApi::PluginEditorInterface *)module.plugin_->getEditor();
        }
        return module.editor_;
    };

    SSPExtendedApi::PluginInterface *getPlugin(unsigned t, unsigned m) {
        if (t >= MAX_TRACKS || m >= Track::M_MAX) return nullptr;
        auto &track = tracks_[t];
        return track.modules_[m].plugin_;
    };

    SSPExtendedApi::PluginDescriptor *getDescriptor(unsigned t, unsigned m) {
        if (t >= MAX_TRACKS || m >= Track::M_MAX) return nullptr;
        auto &track = tracks_[t];
        return track.modules_[m].descriptor_;
    };

    const std::vector<ModuleDesc> &getSupportedModules() { return supportedModules_; }

    bool requestModuleChange(unsigned t, unsigned m, const std::string &mn);
    void loadSupportedModules(bool forceScan = false);

    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;
    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    static constexpr unsigned MAX_TRACKS = 1;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override { return true; }

    std::vector<ModuleDesc> supportedModules_;

    Track tracks_[MAX_TRACKS];

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
