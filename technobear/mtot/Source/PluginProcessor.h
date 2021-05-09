#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (cv_a)
PARAMETER_ID (cv_b)
PARAMETER_ID (cv_c)
PARAMETER_ID (cv_d)
PARAMETER_ID (cv_e)
PARAMETER_ID (cv_f)
PARAMETER_ID (cv_g)
PARAMETER_ID (cv_h)

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

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &cv_a;
        Parameter &cv_b;
        Parameter &cv_c;
        Parameter &cv_d;
        Parameter &cv_e;
        Parameter &cv_f;
        Parameter &cv_g;
        Parameter &cv_h;

        Parameter &pb_range;
    } params_;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    enum {
        I_CV_A,
        I_CV_B,
        I_CV_C,
        I_CV_D,
        I_CV_E,
        I_CV_F,
        I_CV_G,
        I_CV_H,
        I_VOCT,
        I_GATE,
        I_VEL,
        I_MAX
    };

    enum {
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

    int getCCNum(int idx);

    int lastMidi_[I_MAX];
    int pitchbend_=8192;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




