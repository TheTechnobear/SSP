#pragma once

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "RNBO.h"

using namespace juce;

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};

PARAMETER_ID (main)

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

    struct RnboParam {
        using Parameter = juce::RangedAudioParameter;
        RnboParam(AudioProcessorValueTreeState &apvt, StringRef id, unsigned idx);

        unsigned idx_;
        std::string id_;
        std::string desc_;
        RNBO::ParameterInfo info_;
        Parameter &val_;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        std::vector<std::unique_ptr<RnboParam>> rnboParams_;
    } params_;

    unsigned getNumRnboParameters() { return rnbo_.nParams_; }

    static BusesProperties getBusesProperties() {
        BusesProperties props;
        RNBO::CoreObject rnboObj_;
        unsigned I_MAX = rnboObj_.getNumInputChannels();
        for (auto i = 0; i < I_MAX; i++) {
            props.addBus(true, getInputBusName(i), AudioChannelSet::mono());
        }
        unsigned O_MAX = rnboObj_.getNumOutputChannels();
        for (auto i = 0; i < O_MAX; i++) {
            props.addBus(false, getOutputBusName(i), AudioChannelSet::mono());
        }
        return props;
    }

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    int bufferSize_ = 128;

    struct {
        RNBO::CoreObject patch_;
        int nInputs_ = 0;
        RNBO::number **inputBuffers_;
        int nOutputs_ = 0;
        RNBO::number **outputBuffers_;
        int nParams_ = 0;
        float *lastParamVals_;
    } rnbo_;


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


