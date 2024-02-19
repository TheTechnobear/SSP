#pragma once


#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "daisysp.h"

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};



PARAMETER_ID (pitch)
PARAMETER_ID (first)
PARAMETER_ID (centre)
PARAMETER_ID (spread)
PARAMETER_ID (amount)

PARAMETER_ID(harmonics)
// harmonics 1-16
PARAMETER_ID (amp)
#undef PARAMETER_ID
}

using namespace juce;


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
        I_VOCT,
        I_AMP,
        I_CENTRE,
        I_SPREAD,
        I_AMOUNT,
        I_MAX
    };
    enum {
        O_MAIN,
        O_MAX
    };

    static constexpr unsigned MAX_HARMONICS = 16;


    struct Harmonic {
        using Parameter = juce::RangedAudioParameter;
        Harmonic(AudioProcessorValueTreeState &apvt, unsigned id);
        unsigned id_ = 0;
        String pid_;
        Parameter &amp;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        Parameter &pitch;
        Parameter &first;
        Parameter &centre;
        Parameter &spread;
        Parameter &amount;
        std::vector<std::unique_ptr<Harmonic>> harmonics_;
    } params_;

    Harmonic &getHarmonic(unsigned n) {
        jassert(n < params_.harmonics_.size());
        return *(params_.harmonics_[n]);
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
    static inline float normValue(RangedAudioParameter &p) {
        return p.convertFrom0to1(p.getValue());
    }


    daisysp::HarmonicOscillator<MAX_HARMONICS> oscillator_;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }
    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);
    float inActivity_[I_MAX];
    float outActivity_[O_MAX];
    unsigned activityCount_ = 0;
    static constexpr unsigned ACTIVITY_PERIOD = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


