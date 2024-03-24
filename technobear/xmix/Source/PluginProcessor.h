#pragma once

#include <algorithm>
#include <atomic>

#include "Module.h"
#include "SSPExApi.h"
#include "Track.h"
#include "ssp/BaseProcessor.h"

#include "ssp/controls/RmsTrack.h"

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

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    void onInputChanged(unsigned i, bool b) override;
    void onOutputChanged(unsigned i, bool b) override;

    const std::vector<std::string> &getSupportedModules() { return supportedModules_; }

    bool requestModuleChange(unsigned t, unsigned m, const std::string &mn);
    void scanPlugins();

    void rmsLevels(unsigned t, float& lLevel, float& rLevel) { lLevel = rmsData_[t][0].lvl();rLevel = rmsData_[t][1].lvl();}

    static constexpr unsigned MAX_TRACKS = 4;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    void prepareTrack(Track &track, double sampleRate, int samplesPerBlock);
    void processTrack(Track &track, AudioSampleBuffer &ioBuffer);
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override { return true; }

    std::vector<std::string> supportedModules_;


    Track tracks_[MAX_TRACKS];

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    ssp::RmsTrack rmsData_[MAX_TRACKS][O_MAX];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
