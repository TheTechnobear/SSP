#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (tr_a)
PARAMETER_ID (tr_b)
PARAMETER_ID (tr_c)
PARAMETER_ID (tr_d)
PARAMETER_ID (tr_e)
PARAMETER_ID (tr_f)
PARAMETER_ID (tr_g)
PARAMETER_ID (tr_h)

PARAMETER_ID (slew)
PARAMETER_ID (pb_range)

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

    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override;


    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &tr_a;
        Parameter &tr_b;
        Parameter &tr_c;
        Parameter &tr_d;
        Parameter &tr_e;
        Parameter &tr_f;
        Parameter &tr_g;
        Parameter &tr_h;
    } params_;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    enum {
//        I_DUMMY,
        I_MAX
    };

    enum {
        O_TR_A,
        O_VEL_A,
        O_TR_B,
        O_VEL_B,
        O_TR_C,
        O_VEL_C,
        O_TR_D,
        O_VEL_D,
        O_TR_E,
        O_VEL_E,
        O_TR_F,
        O_VEL_F,
        O_TR_G,
        O_VEL_G,
        O_TR_H,
        O_VEL_H,
        O_MAX
    };


private:


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

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

    bool  nextTR_[O_MAX];
    float nextVel_[O_MAX];


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




