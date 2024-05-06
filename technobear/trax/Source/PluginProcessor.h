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
    static constexpr unsigned IO_MAX = std::max(I_MAX, O_MAX);

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

    const std::vector<ModuleDesc> &getSupportedModules() { return supportedModules_; }

    bool requestModuleChange(unsigned t, unsigned m, const std::string &mn);
    void loadSupportedModules(bool forceScan = false);

    Track &track(unsigned t) { return tracks_[t]; }

    void rmsLevels(unsigned t, float &lLevel, float &rLevel) {
        lLevel = rmsData_[t][0].lvl();
        rLevel = rmsData_[t][1].lvl();
    }

    static constexpr unsigned MAX_TRACKS = 4;

    void presetName(const String &n) { presetName_ = n; }
    String presetName() const { return presetName_; }


    void presetLoadRequest(const String &n) {
        requestedPreset_ = n;
        requestPresetLoad_ = true;
        loadPreset();
    }

    void presetSaveRequest(const String &n) {
        requestedPreset_ = n;
        requestPresetSave_ = true;
        savePreset();
    }

    void savePreset();
    void loadPreset();

    void muteTrack(unsigned t, bool mute) {
        if (t >= MAX_TRACKS) return;
        tracks_[t].mute(mute);
    }

    bool muteTrack(unsigned t) {
        if (t >= MAX_TRACKS) return false;
        return tracks_[t].mute();
    }

    void gainTrack(unsigned t, float gain) {
        if (t >= MAX_TRACKS) return;
        tracks_[t].level(gain);
    }

    float gainTrack(unsigned t) {
        if (t >= MAX_TRACKS) return 0.f;
        return tracks_[t].level();
    }

    class PerformanceParam {
    public:
        PerformanceParam(unsigned t, unsigned m, unsigned p, 
                        const std::string &pluginName,
                        const std::string &paramName,
                        float min = 0.f, float max = 1.f, float def = 0.5f, int numSteps = 127, bool isDescrete = false)
            : trackIdx_(t), moduleIdx_(m), paramIdx_(p), pluginName_(pluginName), paramName_(paramName),
              min_(min), max_(max), def_(def), numSteps_(numSteps), isDescrete_(isDescrete), value_(def) {}


        unsigned trackIdx() const { return trackIdx_; }
        unsigned moduleIdx() const { return moduleIdx_; }
        unsigned paramIdx() const { return paramIdx_; }

        const std::string &pluginName() const { return pluginName_; }
        const std::string &paramName() const { return paramName_; }
        float min() const { return min_; }
        float max() const { return max_; }
        float def() const { return def_; }
        bool isDescrete() const { return isDescrete_; }
        int numSteps() const { return numSteps_; }

        void value(float v) { value_ = v; }
        float value() const { return value_; }


    private:
        unsigned trackIdx_;
        unsigned moduleIdx_;
        unsigned paramIdx_;

        std::string pluginName_;
        std::string paramName_;
        float min_ = 0.f;
        float max_ = 1.f;
        float def_ = 0.5f;
        int  numSteps_ = 127;
        bool isDescrete_ = false;

        float value_;
    };

    bool addPerformanceParam(const PerformanceParam &p);
    bool removePerformanceParam(const PerformanceParam &p);
    bool removePerformanceParam(unsigned t);
    bool removePerformanceParam(unsigned t, unsigned m);

    std::vector<PerformanceParam> &performanceParams() { return performanceParams_; }


protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    void prepareTrack(Track &track, double sampleRate, int samplesPerBlock);
    void processTrack(Track &track, AudioSampleBuffer &ioBuffer);
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override { return true; }

    std::vector<ModuleDesc> supportedModules_;

    bool shouldExit();
    bool shouldExit_ = false;

    Track tracks_[MAX_TRACKS];
    float lastGain_[MAX_TRACKS] = { 0.f, 0.f, 0.f, 0.f };
    // processing thread for each track
    std::vector<std::thread> trackThreads_;

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    AudioSampleBuffer threadBuffers_[MAX_TRACKS];

    ssp::RmsTrack rmsData_[MAX_TRACKS][O_MAX];

    String presetName_;
    String requestedPreset_;
    bool requestPresetLoad_ = false;
    bool requestPresetSave_ = false;

    std::vector<PerformanceParam> performanceParams_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
