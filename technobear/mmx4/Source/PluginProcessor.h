#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

PARAMETER_ID (vca_1a)
PARAMETER_ID (vca_1b)
PARAMETER_ID (vca_1c)
PARAMETER_ID (vca_1d)

PARAMETER_ID (vca_2a)
PARAMETER_ID (vca_2b)
PARAMETER_ID (vca_2c)
PARAMETER_ID (vca_2d)

PARAMETER_ID (vca_3a)
PARAMETER_ID (vca_3b)
PARAMETER_ID (vca_3c)
PARAMETER_ID (vca_3d)

PARAMETER_ID (vca_4a)
PARAMETER_ID (vca_4b)
PARAMETER_ID (vca_4c)
PARAMETER_ID (vca_4d)

#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor() override = default;

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    enum {
        I_SIG_1L,
        I_SIG_1R,
        I_SIG_2L,
        I_SIG_2R,
        I_SIG_3L,
        I_SIG_3R,
        I_SIG_4L,
        I_SIG_4R,
        I_VCA_1A,
        I_VCA_1B,
        I_VCA_1C,
        I_VCA_1D,
        I_VCA_2A,
        I_VCA_2B,
        I_VCA_2C,
        I_VCA_2D,
        I_VCA_3A,
        I_VCA_3B,
        I_VCA_3C,
        I_VCA_3D,
        I_VCA_4A,
        I_VCA_4B,
        I_VCA_4C,
        I_VCA_4D,
        I_MAX
    };

    enum {
        O_SIG_AL,
        O_SIG_AR,
        O_SIG_BL,
        O_SIG_BR,
        O_SIG_CL,
        O_SIG_CR,
        O_SIG_DL,
        O_SIG_DR,
        O_MAX
    };


public:
    static constexpr unsigned MAX_SIG_IN = (I_SIG_4L - I_SIG_1L) / 2 + 1;
    static constexpr unsigned MAX_SIG_OUT = (O_SIG_DL - O_SIG_AL) / 2 + 1;

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        std::reference_wrapper<Parameter> vca[MAX_SIG_IN][MAX_SIG_OUT];
    } params_;

    float getVCA(unsigned i, unsigned o) {
        PluginParams::Parameter &p = params_.vca[i][o];
        return p.convertFrom0to1(p.getValue());
    }

    float getVCACV(unsigned i, unsigned o) { return lastVcaCV_[i][o]; }

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

    static Colour getIconColour() { return juce::Colours::lightblue;}

private:
    std::atomic<float> lastVcaCV_[MAX_SIG_IN][MAX_SIG_OUT];

    AudioSampleBuffer outBufs_;
    AudioSampleBuffer workBuf_;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }
    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




