#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "daisysp.h"

//daisysp::AnalogBassDrum analogBassDrum_;
//daisysp::SyntheticSnareDrum syntheticBassDrum_;
//daisysp::AnalogSnareDrum analogSnareDrum_;
//daisysp::SyntheticSnareDrum syntheticSnareDrum_;
//daisysp::HiHat<daisysp::SquareNoise> hiHat1_;
//daisysp::HiHat<daisysp::RingModNoise> hiHat2_;



namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID(AB)
PARAMETER_ID(SB)
PARAMETER_ID(AS)
PARAMETER_ID(SS)
PARAMETER_ID(HH1)
PARAMETER_ID(HH2)


// common parameters

PARAMETER_ID(Sustain)
PARAMETER_ID(Accent)
PARAMETER_ID(Freq)
PARAMETER_ID(Tone) // not SS, SB
PARAMETER_ID(Decay)
PARAMETER_ID(Gain)


PARAMETER_ID(AB_AttackFM)
PARAMETER_ID(AB_SelfFM)

PARAMETER_ID(SB_Dirt)
PARAMETER_ID(SB_EnvFM)
PARAMETER_ID(SB_FMDecay)

PARAMETER_ID(AS_Snappy)


PARAMETER_ID(SS_FM)
PARAMETER_ID(SS_Snappy)

PARAMETER_ID(HH1_Noise)

PARAMETER_ID(HH2_Noise)

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
        I_AB_TRIG,
        I_AB_ACCENT,
        I_SB_TRIG,
        I_SB_ACCENT,
        I_AS_TRIG,
        I_AS_ACCENT,
        I_SS_TRIG,
        I_SS_ACCENT,
        I_HH1_TRIG,
        I_HH1_ACCENT,
        I_HH2_TRIG,
        I_HH2_ACCENT,
        I_MAX
    };
    enum {
        O_AB,
        O_SB,
        O_AS,
        O_SS,
        O_HH1,
        O_HH2,
        O_MAX
    };

    struct DrumBaseParam {
        using Parameter = juce::RangedAudioParameter;
        DrumBaseParam(AudioProcessorValueTreeState &apvt, StringRef pre);
        Parameter &Sustain;
        Parameter &Accent;
        Parameter &Freq;
        Parameter &Tone;
        Parameter &Decay;
        Parameter &Gain;
    };

    struct ABParam : public DrumBaseParam {
        ABParam(AudioProcessorValueTreeState &apvt, StringRef id);
        Parameter &AB_AttackFM;
        Parameter &AB_SelfFM;
    };

    struct SBParam : public DrumBaseParam {
        SBParam(AudioProcessorValueTreeState &apvt, StringRef id);
        Parameter &SB_Dirt;
        Parameter &SB_EnvFM;
        Parameter &SB_FMDecay;
    };

    struct ASParam : public DrumBaseParam {
        ASParam(AudioProcessorValueTreeState &apvt, StringRef id);
        Parameter &AS_Snappy;
    };


    struct SSParam : public DrumBaseParam {
        SSParam(AudioProcessorValueTreeState &apvt, StringRef id);
        Parameter &SS_FM;
        Parameter &SS_Snappy;
    };

    struct HH1Param : public DrumBaseParam {
        HH1Param(AudioProcessorValueTreeState &apvt, StringRef id);
        Parameter &HH1_Noise;
    };
    struct HH2Param : public DrumBaseParam {
        HH2Param(AudioProcessorValueTreeState &apvt, StringRef id);
        Parameter &HH2_Noise;
    };


    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        ABParam ab_;
        SBParam sb_;
        ASParam as_;
        SSParam ss_;
        HH1Param hh1_;
        HH2Param hh2_;
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

    float getCurrentVolt(float layer, unsigned volt, bool morph);

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    inline float normValue(RangedAudioParameter &p) {
        return p.convertFrom0to1(p.getValue());
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    float trig_[O_MAX];

    daisysp::AnalogBassDrum analogBassDrum_;
    daisysp::SyntheticBassDrum syntheticBassDrum_;
    daisysp::AnalogSnareDrum analogSnareDrum_;
    daisysp::SyntheticSnareDrum syntheticSnareDrum_;
    daisysp::HiHat<daisysp::SquareNoise> hiHat1_;
    daisysp::HiHat<daisysp::RingModNoise> hiHat2_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


