#pragma once

#include "plaits/dsp/voice.h"

#include "ssp/BaseProcessor.h"
#include "ssp/controls/RmsTrack.h"

#include <atomic>
#include <algorithm>

using namespace juce;


//pitch
//harmonics
//timbre
//morph
//model
//freq_mod
//timbre_mod
//morph_mod
//lpg
//vca


namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (pitch)
PARAMETER_ID (harmonics)
PARAMETER_ID (timbre)
PARAMETER_ID (morph)
PARAMETER_ID (model)
PARAMETER_ID (freq_mod)
PARAMETER_ID (timbre_mod)
PARAMETER_ID (morph_mod)
PARAMETER_ID (lpg)
PARAMETER_ID (vca)

PARAMETER_ID (enable_trig)
PARAMETER_ID (enable_level)

#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor() override = default;

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter& pitch;
        Parameter& harmonics;
        Parameter& timbre;
        Parameter& morph;
        Parameter& model;
        Parameter& freq_mod;
        Parameter& timbre_mod;
        Parameter& morph_mod;
        Parameter& lpg;
        Parameter& vca;
        Parameter& enable_trig;
        Parameter& enable_level;
    } params_;

    void getRMS(float &lOut, float &rOut) {
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
    void midiNoteInput(unsigned note, unsigned velocity) override { if (velocity > 0) noteInputTranspose_ = float(note) - 60.f; }

private:
    enum {
        I_VOCT,
        I_TRIG,
        I_LEVEL,
        I_HARMONICS,
        I_TIMBRE,
        I_MORPH,
        I_FM,
        I_MODEL,

        I_MAX
    };
    enum {
        O_OUT,
        O_AUX,
        O_MAX
    };

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    static constexpr unsigned PltsBlock = 16;
    static constexpr float PltsMaxEngine = 24.0f -1.0f;
    plaits::Voice voice_;
    plaits::Patch patch_{};
    char shared_buffer_[16384]{};

    ssp::RmsTrack outRms_[2];

    float trig_ =0.0f;
    float noteInputTranspose_ = 0.0f;

    float inActivity_[I_MAX];
    float outActivity_[O_MAX];
    unsigned activityCount_ = 0;
    static constexpr unsigned ACTIVITY_PERIOD = 10;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




