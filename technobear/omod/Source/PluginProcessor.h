#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <daisysp.h>

#include <atomic>
#include <algorithm>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (wave)
PARAMETER_ID (freq)
PARAMETER_ID (amp)
PARAMETER_ID (phase)


// slaveosc : 1-7
PARAMETER_ID (slaveosc)

//PARAMETER_ID (wave)
PARAMETER_ID (ratio)
//PARAMETER_ID (amp)
//PARAMETER_ID (phase)

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

    void requestReset() { requestReset_=true;}

    enum {
        I_FREQ,
        I_RESET,
        I_CLOCK,
        I_MAX
    };
    enum {
        O_OUT_MAIN,
        O_OUT_A,
        O_OUT_B,
        O_OUT_C,
        O_OUT_D,
        O_OUT_E,
        O_OUT_F,
        O_OUT_G,
        O_EOC_MAIN,
        O_EOC_A,
        O_EOC_B,
        O_EOC_C,
        O_EOC_D,
        O_EOC_E,
        O_EOC_F,
        O_EOC_G,
        O_MAX
    };


    static constexpr unsigned MAX_S_OSC = (O_OUT_G - O_OUT_A) + 1;

    struct SlaveOscParams {
        using Parameter = juce::RangedAudioParameter;
        SlaveOscParams(AudioProcessorValueTreeState &apvt, unsigned id);
        unsigned id_;
        String pid_; // e.g mods:1
        Parameter &wave;
        Parameter &ratio;
        Parameter &amp;
        Parameter &phase;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &wave;
        Parameter &freq;
        Parameter &amp;
        Parameter &phase;

        std::vector<std::unique_ptr<SlaveOscParams>> slaveOscsParams_;
    } params_;

    SlaveOscParams &getSlaveOscParam(unsigned t) {
        jassert(t < params_.slaveOscsParams_.size());
        return *(params_.slaveOscsParams_[t]);
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

    daisysp::Oscillator mainOsc_;
    float mainPhase_=0.0f;
    float mainFreq_=0.0f;

    struct {
        daisysp::Oscillator osc_;
        float phase_=0.0f;
        float freq_=0.0f;

    } slaveOscs_[MAX_S_OSC];

    int clockSampleCnt_=0;
    float lastClock_ = 0.0f;
    float clockFreq_ = 0.0f;

    float lastReset_ = 0.0f;
    bool requestReset_ = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


