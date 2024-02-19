#pragma once


#include "rings/dsp/part.h"
#include "rings/dsp/patch.h"
#include "rings/dsp/strummer.h"
#include "rings/dsp/string_synth_part.h"

#include "ssp/BaseProcessor.h"
#include "ssp/controls/RmsTrack.h"

#include <atomic>
#include <algorithm>

using namespace juce;

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (pitch)
PARAMETER_ID (structure)
PARAMETER_ID (brightness)
PARAMETER_ID (damping)
PARAMETER_ID (position)
PARAMETER_ID (polyphony)
PARAMETER_ID (model)
//PARAMETER_ID (bypass)
//PARAMETER_ID (easter_egg)
PARAMETER_ID (in_gain)

PARAMETER_ID (enable_in)
PARAMETER_ID (enable_strum)
PARAMETER_ID (enable_voct)

#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        Parameter &pitch;
        Parameter &structure;
        Parameter &brightness;
        Parameter &damping;
        Parameter &position;
        Parameter &polyphony;
        Parameter &model;
        //Parameter&  bypass;
        //Parameter&  easter_egg;
        Parameter &in_gain;

        Parameter &enableIn;
        Parameter &enableStrum;
        Parameter &enableVoct;
    } params_;

    void getRMS(float &in, float &lOut, float &rOut) {
        in = inRms_.lvl();
        lOut = outRms_[0].lvl();
        rOut = outRms_[1].lvl();
    }

    void setStateInformation(const void *data, int sizeInBytes) override;

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
        I_IN,
        I_STRUM,
        I_VOCT,
        I_FM,
        I_STRUCTURE,
        I_BRIGHTNESS,
        I_DAMPING,
        I_POSITION,
        I_MAX
    };
    enum {
        O_ODD,
        O_EVEN,
        O_MAX
    };


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    void initPart(bool force);

    static constexpr unsigned RingsBlock = 16;

    rings::Part part_;
    rings::PerformanceState performance_state_;
    rings::StringSynthPart string_synth_;
    rings::Strummer strummer_;
    rings::Patch patch_;

    static constexpr float kNoiseGateThreshold = 0.00003f;
    float inLevel_;

    float *in_buf_, *out_buf_, *aux_buf_;
    int io_buf_sz_;

    static const int REVERB_SZ = 32768;
    uint16_t buffer[REVERB_SZ];

    ssp::RmsTrack inRms_;
    ssp::RmsTrack outRms_[2];
    bool trig_;
    float noteInputTranspose_ = 0.0f;

    float inActivity_[I_MAX];
    float outActivity_[O_MAX];
    unsigned activityCount_ = 0;
    static constexpr unsigned ACTIVITY_PERIOD = 10;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
