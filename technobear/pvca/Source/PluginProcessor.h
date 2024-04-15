#pragma once

#include <algorithm>
#include <atomic>

#include "ssp/BaseProcessor.h"

using namespace juce;

namespace ID {
#define PARAMETER_ID(str) constexpr const char *str{ #str };
constexpr const char *separator{ ":" };


// vca : 1-4
PARAMETER_ID(vca)

PARAMETER_ID(gain)
PARAMETER_ID(pan)

#undef PARAMETER_ID
}  // namespace ID


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                             AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    void prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) override;


    enum {
        I_VCA_1_IN_L,
        I_VCA_1_IN_R,
        I_VCA_1_GAIN,
        I_VCA_1_PAN,
        I_VCA_2_IN_L,
        I_VCA_2_IN_R,
        I_VCA_2_GAIN,
        I_VCA_2_PAN,
        I_VCA_3_IN_L,
        I_VCA_3_IN_R,
        I_VCA_3_GAIN,
        I_VCA_3_PAN,
        I_VCA_4_IN_L,
        I_VCA_4_IN_R,
        I_VCA_4_GAIN,
        I_VCA_4_PAN,
        I_MAX
    };
    enum {
        O_SUM_L,
        O_SUM_R,
        O_VCA_1_L,
        O_VCA_1_R,
        O_VCA_2_L,
        O_VCA_2_R,
        O_VCA_3_L,
        O_VCA_3_R,
        O_VCA_4_L,
        O_VCA_5_R,
        O_MAX
    };

    static constexpr unsigned MAX_VCA = ((O_VCA_4_L - O_VCA_1_L) / 2) + 1;
    static constexpr unsigned MAX_I_VCA = (I_VCA_1_PAN - I_VCA_1_IN_L) + 1;
    static constexpr unsigned MAX_O_VCA = (O_VCA_1_L - O_VCA_1_R) + 1;

    struct VcaParams {
        using Parameter = juce::RangedAudioParameter;
        VcaParams(AudioProcessorValueTreeState &apvt, unsigned id);
        unsigned id_;
        String pid_;  // e.g mods:1
        Parameter &gain;
        Parameter &pan;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        std::vector<std::unique_ptr<VcaParams>> vcaParams_;
    } params_;

    VcaParams &getVcaParam(unsigned i) {
        jassert(i < params_.vcaParams_.size());
        return *(params_.vcaParams_[i]);
    }

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) { props.addBus(true, getInputBusName(i), AudioChannelSet::mono()); }
        for (auto i = 0; i < O_MAX; i++) { props.addBus(false, getOutputBusName(i), AudioChannelSet::mono()); }
        return props;
    }

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override { return true; }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    inline float normValue(RangedAudioParameter &p) { return p.convertFrom0to1(p.getValue()); }


    struct {
        float gain_ = 1.0f;
        float pan_ = 0.0f;
    } vcas_[MAX_VCA];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
