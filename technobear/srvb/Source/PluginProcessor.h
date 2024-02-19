#pragma once

#include "ssp/BaseProcessor.h"
#include "ssp/controls/RmsTrack.h"

#include <atomic>
#include <algorithm>

#include <daisysp.h>

using namespace juce;


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (feedback)
PARAMETER_ID (lpfreq)
PARAMETER_ID (mix)
PARAMETER_ID (freeze)

#undef PARAMETER_ID
}

class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) override;
    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        Parameter &feedback;
        Parameter &lpfreq;
        Parameter &mix;
        Parameter &freeze;
    } params_;

    void getRMS(float &lIn, float &rIn, float &lOut, float &rOut) {
        lIn = inRms_[0].lvl();
        rIn = inRms_[1].lvl();
        lOut = outRms_[0].lvl();
        rOut = outRms_[1].lvl();
    }

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

    float ioActivity(bool input, int bus) {
        jassert(
            (input == true && bus < I_MAX)
            ||
            (input == false && bus < O_MAX)
        );
        return input ? inActivity_[bus] : outActivity_[bus];
    }

protected:

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    enum {
        I_LEFT,
        I_RIGHT,
        I_MAX
    };

    enum {
        O_LEFT,
        O_RIGHT,
        O_MAX
    };

    inline float normValue(RangedAudioParameter &p) {
        return p.convertFrom0to1(p.getValue());
    }

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    daisysp::ReverbSc reverbSc_;
    ssp::RmsTrack inRms_[2];
    ssp::RmsTrack outRms_[2];

    float inActivity_[I_MAX];
    float outActivity_[O_MAX];
    unsigned activityCount_ = 0;
    static constexpr unsigned ACTIVITY_PERIOD = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


