#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "daisysp.h"

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID(filters)
PARAMETER_ID (cutoff)
PARAMETER_ID (res)
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
        I_CUTOFF_1,
        I_RES_1,
        I_IN_2,
        I_CUTOFF_2,
        I_RES_2,
        I_IN_3,
        I_CUTOFF_3,
        I_RES_3,
        I_IN_4,
        I_CUTOFF_4,
        I_RES_4,
        I_MAX
    };
    enum {
        O_OUT_1,
        O_OUT_2,
        O_OUT_3,
        O_OUT_4,
        O_MAX
    };

    static constexpr unsigned MAX_FILTERS = 4;


    struct Filter {
        using Parameter = juce::RangedAudioParameter;
        Filter(AudioProcessorValueTreeState &apvt, unsigned id);
        unsigned id_ = 0;
        String pid_;
        Parameter &cutoff;
        Parameter &res;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        std::vector<std::unique_ptr<Filter>> filters_;
    } params_;

    Filter &getFilter(unsigned n) {
        jassert(n < params_.filters_.size());
        return *(params_.filters_[n]);
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
    static inline float normValue(RangedAudioParameter &p) {
        return p.convertFrom0to1(p.getValue());
    }

    std::vector<std::unique_ptr<daisysp::MoogLadder>> filters_;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


