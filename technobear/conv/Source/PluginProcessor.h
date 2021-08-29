#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"
#include "ssp/RmsTrack.h"

#include <atomic>
#include <algorithm>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (mix)
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

        Parameter &mix;
    } params_;

    void getRMS(float &inL, float &inR, float &lOut, float &rOut) {
        inL = inRms_[0].lvl();
        inR = inRms_[1].lvl();
        lOut = outRms_[0].lvl();
        rOut = outRms_[1].lvl();
    }

    void setStateInformation(const void *data, int sizeInBytes) override;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    enum {
        I_INL,
        I_INR,
        I_MIX,
        I_MAX
    };
    enum {
        O_OUTL,
        O_OUTR,
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

    ssp::RmsTrack inRms_[2];
    ssp::RmsTrack outRms_[2];

    dsp::Convolution convolution_;
    AudioSampleBuffer workBuf_;
    AudioSampleBuffer IRBuf_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
