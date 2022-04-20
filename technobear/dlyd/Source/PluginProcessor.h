#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <daisysp.h>

#include <atomic>
#include <algorithm>


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (size)
PARAMETER_ID (mix)
PARAMETER_ID (in_level)
PARAMETER_ID (out_level)


// tree taps:1-4:params
PARAMETER_ID (taps)

PARAMETER_ID (time)
PARAMETER_ID (repeats)
PARAMETER_ID (pan)
PARAMETER_ID (level)
PARAMETER_ID (feedback)

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
        I_MAX
    };
    enum {
        O_OUT_1,
        O_OUT_2,
        O_OUT_TA_1,
        O_OUT_TB_1,
        O_OUT_TC_1,
        O_OUT_TD_1,
        O_OUT_TA_2,
        O_OUT_TB_2,
        O_OUT_TC_2,
        O_OUT_TD_2,
        O_MAX
    };


    static constexpr unsigned MAX_TAPS = 4;

    struct Tap {
        using Parameter = juce::RangedAudioParameter;
        Tap(AudioProcessorValueTreeState &apvt, unsigned id);
        unsigned id_;
        String pid_; // e.g taps:1
        Parameter &time;
        Parameter &repeats;
        Parameter &pan;
        Parameter &level;
        Parameter &feedback;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &size;
        Parameter &mix;
        Parameter &in_level;
        Parameter &out_level;

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
    static constexpr unsigned MAX_DELAY = 48000 * 15; // 15 seconds
    static constexpr unsigned N_DLY_LINES = I_MAX;
    daisysp::DelayLine<float, MAX_DELAY> delayLine_[N_DLY_LINES];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


