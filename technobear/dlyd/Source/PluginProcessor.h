#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <daisysp.h>

#include <atomic>
#include <algorithm>
#include "ssp/controls/RmsTrack.h"


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (size)
PARAMETER_ID (mix)
PARAMETER_ID (in_level)
PARAMETER_ID (out_level)
PARAMETER_ID (freeze)


// tree taps:1-4:params
PARAMETER_ID (taps)

PARAMETER_ID (time)
PARAMETER_ID (pan)
PARAMETER_ID (level)
PARAMETER_ID (feedback)
PARAMETER_ID (lpf)
PARAMETER_ID (hpf)
PARAMETER_ID (noise)


#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    void prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) override;

    enum {
        I_IN_1,
        I_IN_2,
        I_TIME_1_TA,
        I_TIME_1_TB,
        I_TIME_1_TC,
        I_TIME_1_TD,
        I_TIME_2_TA,
        I_TIME_2_TB,
        I_TIME_2_TC,
        I_TIME_2_TD,
        I_MAX
    };
    enum {
        O_OUT_1,
        O_OUT_2,
        O_OUT_1_TA,
        O_OUT_1_TB,
        O_OUT_1_TC,
        O_OUT_1_TD,
        O_OUT_2_TA,
        O_OUT_2_TB,
        O_OUT_2_TC,
        O_OUT_2_TD,
        O_MAX
    };


    static constexpr unsigned MAX_TAPS = 4;

    struct Tap {
        using Parameter = juce::RangedAudioParameter;
        Tap(AudioProcessorValueTreeState &apvt, unsigned id);
        unsigned id_;
        String pid_; // e.g taps:1
        Parameter &time;
        Parameter &level;
        Parameter &feedback;
        Parameter &pan;
        Parameter &lpf;
        Parameter &hpf;
        Parameter &noise;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &size;
        Parameter &mix;
        Parameter &in_level;
        Parameter &out_level;
        Parameter &freeze;

        std::vector<std::unique_ptr<Tap>> taps_;
    } params_;

    Tap &getTap(unsigned t) {
        jassert(t < params_.taps_.size());
        return *(params_.taps_[t]);
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

    void getRMS(float &lIn, float &rIn, float &lOut, float &rOut) {
        lIn = inRms_[0].lvl();
        rIn = inRms_[1].lvl();
        lOut = outRms_[0].lvl();
        rOut = outRms_[1].lvl();
    }


protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    inline float normValue(RangedAudioParameter &p) { return p.convertFrom0to1(p.getValue()); }


    // initial time with delay line from daisysp
    static constexpr unsigned MAX_DELAY = 48000 * 60; // 60 seconds
    static constexpr unsigned N_DLY_LINES = 2;
    struct DelayLine {
        daisysp::DelayLine<float, MAX_DELAY> line_;
        struct Taps {
            daisysp::Tone lpf_;
            daisysp::ATone hpf_;
            daisysp::Dust noise_;
        } taps_[MAX_TAPS];
    } delayLines_[N_DLY_LINES];

    ssp::RmsTrack inRms_[2];
    ssp::RmsTrack outRms_[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


