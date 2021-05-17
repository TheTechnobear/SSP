#pragma once


#include "clouds/dsp/granular_processor.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"
#include "ssp/RmsTrack.h"

#include <atomic>
#include <algorithm>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (position)
PARAMETER_ID (size)
PARAMETER_ID (density)
PARAMETER_ID (texture)
PARAMETER_ID (mix)
PARAMETER_ID (spread)
PARAMETER_ID (feedback)
PARAMETER_ID (reverb)
PARAMETER_ID (pitch)
PARAMETER_ID (mode)
PARAMETER_ID (in_gain)
PARAMETER_ID (freeze)

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

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        Parameter &position;
        Parameter &size;
        Parameter &density;
        Parameter &texture;

        Parameter &mix;
        Parameter &spread;
        Parameter &feedback;
        Parameter &reverb;

        Parameter &pitch;
        Parameter &mode;
        Parameter &in_gain;
        Parameter &freeze;
    } params_;

    void getRMS(float &lIn, float &rIn, float &lOut, float &rOut) {
        lIn = inRms_[0].lvl();
        rIn = inRms_[1].lvl();
        lOut = outRms_[0].lvl();
        rOut = outRms_[1].lvl();
    }

protected:
    void midiNoteInput(unsigned note, unsigned velocity) override { if (velocity > 0) noteInputTranspose_ = float(note) - 60.f; }

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    enum {
        I_LEFT,
        I_RIGHT,
        I_TRIG,
        I_VOCT,
        I_POS,
        I_SIZE,
        I_DENSITY,
        I_TEXT,
        I_FREEZE,
        I_MIX,
        I_SPREAD,
        I_FEEDBACK,
        I_REVERB,
        I_MAX
    };

    enum {
        O_LEFT,
        O_RIGHT,
        O_MAX
    };

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

    clouds::GranularProcessor *granularProcessor_;
    clouds::ShortFrame *ibuf_;
    clouds::ShortFrame *obuf_;
    static constexpr unsigned CloudsBlock = 32;
    static constexpr unsigned IO_BUF_SZ = CloudsBlock;
    static constexpr unsigned BLOCK_MEM_SZ = 118784;
    static constexpr unsigned BLOCK_CCM_SZ = 65536 - 128;
    //static constexpr unsigned BLOCK_MEM_SZ =  (118784 * 3) / 2; // 48k vs 32k
    //static constexpr unsigned BLOCK_CCM_SZ =  ((65536 - 128) * 3 ) / 2 ;
    uint8_t *block_mem_;
    uint8_t *block_ccm_;

    ssp::RmsTrack inRms_[2];
    ssp::RmsTrack outRms_[2];

    float noteInputTranspose_ = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


