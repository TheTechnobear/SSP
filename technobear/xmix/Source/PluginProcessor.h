#pragma once

#include <algorithm>
#include <atomic>

#include "Module.h"
#include "SSPExApi.h"
#include "Track.h"
#include "ssp/BaseProcessor.h"


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

    static constexpr unsigned I_MAX = 8;
    static constexpr unsigned O_MAX = 2;

    const std::string &getLoadedPlugin(unsigned m) {
        auto &track = tracks_[0];
        return track.modules_[m].pluginName_;
    };

    SSPExtendedApi::PluginEditorInterface *getEditor(unsigned midx) {
        auto &track = tracks_[0];
        auto &m = track.modules_[midx];

        if (m.plugin_ != nullptr && m.editor_ == nullptr) {
            m.editor_ = (SSPExtendedApi::PluginEditorInterface *)m.plugin_->getEditor();
        }
        return m.editor_;
    };

    SSPExtendedApi::PluginInterface *getPlugin(unsigned m) {
        auto &track = tracks_[0];
        return track.modules_[m].plugin_;
    };

    SSPExtendedApi::PluginDescriptor *getDescriptor(unsigned m) {
        auto &track = tracks_[0];
        return track.modules_[m].descriptor_;
    };

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;

    const std::vector<std::string> &getSupportedModules() { return supportedModules_; }

    bool requestModuleChange(unsigned midx, const std::string &mn);
    void scanPlugins();

    enum ModuleIdx {
        M_MOD = Track::M_MOD,
        M_PRE = Track::M_PRE,
        M_MAIN = Track::M_MAIN,
        M_POST = Track::M_POST,
        M_MAX = Track::M_MAX
    };

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override { return true; }

    std::vector<std::string> supportedModules_;

    static constexpr unsigned MAX_TRACKS = 4;

    Track tracks_[MAX_TRACKS];

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
