#pragma once

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>


using namespace juce;

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

constexpr const char *separator{":"};

PARAMETER_ID (oper)
PARAMETER_ID (triglevel)

PARAMETER_ID (gates)
PARAMETER_ID (inv)
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

    void getValues(float* inputs, bool* outputs);

    struct GateParam {
        using Parameter = juce::RangedAudioParameter;
        GateParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned id);
        Parameter &inv;
    };
    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &oper;
        Parameter &triglevel;
        std::vector<std::unique_ptr<GateParam>> gateparams_;
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

    enum {
        I_IN_A1,
        I_IN_B1,
        I_IN_A2,
        I_IN_B2,
        I_IN_A3,
        I_IN_B3,
        I_IN_A4,
        I_IN_B4,
        I_MAX
    };

    enum {
        O_OUT_ALL,
        O_OUT_1,
        O_OUT_2,
        O_OUT_3,
        O_OUT_4,
        O_MAX
    };
protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


private:
    enum OperType {
        OT_AND,
        OT_OR,
        OT_XOR,
        OT_NAND,
        OT_NOR,
        OT_NOT_A,
        OT_NOT_B,
        OT_GT,
        OT_LT,
        OT_MAX
    };
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    inline float normValue(RangedAudioParameter &p) {
        return p.convertFrom0to1(p.getValue());
    }
    float  defValue_[OT_MAX] = { 1.0f , 0.0f , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f ,0.0f, 0.0f };

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);
    float lastIn_[I_MAX] = {
        false, false, false, false,
        false, false, false, false
    };

    bool lastOut_[O_MAX] = {
        false, false, false, false, false
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




