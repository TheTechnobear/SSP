#pragma once

#include "Quantizer.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

//root
//scale
//quant

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (root)
PARAMETER_ID (scale)
PARAMETER_ID (quant)
#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor() override = default;

    const String getName() const override { return JucePlugin_Name; }

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &root;
        Parameter &scale;
        Parameter &quant;
    } params_;

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

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    float processCV(float value, unsigned scale, unsigned root);

    enum {
        I_SIG_1,
        I_TRIG_1,
        I_SIG_2,
        I_TRIG_2,
        I_SIG_3,
        I_TRIG_3,
        I_SIG_4,
        I_TRIG_4,
        I_SCALE,
        I_ROOT,
        I_MAX
    };

    enum {
        O_SIG_1,
        O_TRIG_1,
        O_SIG_2,
        O_TRIG_2,
        O_SIG_3,
        O_TRIG_3,
        O_SIG_4,
        O_TRIG_4,
        O_SCALE,
        O_ROOT,
        O_MAX
    };

    static constexpr unsigned MAX_SIG = 4;
    std::atomic<float> lastSig_[MAX_SIG];
    std::atomic<float> lastTrig_[MAX_SIG];

    Random randomGen_;
    Quantizer quantizer_;


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};

