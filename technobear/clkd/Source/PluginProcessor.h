#pragma once

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "Clock.h"

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (source)
PARAMETER_ID (clkindiv)
PARAMETER_ID (bpm)
PARAMETER_ID (midippqn)
PARAMETER_ID (usetrigs)

// tree div:val
PARAMETER_ID (div)
PARAMETER_ID (val)

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
        I_CLK,
        I_RESET,
        I_RUN,
        I_MIDICLK,
        I_MAX
    };
    enum {
        O_CLK_1,
        O_CLK_2,
        O_CLK_3,
        O_CLK_4,
        O_CLK_5,
        O_CLK_6,
        O_CLK_7,
        O_CLK_8,
        O_RESET,
        O_RUN,
        O_MAX
    };

    static constexpr unsigned MAX_CLK_OUT = (O_CLK_8 - O_CLK_1) + 1;

    struct DivParam {
        using Parameter = juce::RangedAudioParameter;
        DivParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned id);
        Parameter &val;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);
        Parameter &source;
        Parameter &clkindiv;
        Parameter &bpm;
        Parameter &midippqn;
        Parameter &usetrigs;

        std::vector<std::unique_ptr<DivParam>> divisions_;
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

    void toggleRunRequest() { toggleRunRequest_ = true; }

    bool isUsingTrigs();

    void toggleUseTrigs() { toggleUseTrigs_ = true; }

    bool isRunning() { return runState_; }

    void getClockStates(bool *states);

    void requestReset() { resetRequest_ = true; }

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    enum Source {
        SRC_INTERNAL,
        SRC_CLKIN,
        SRC_MIDI,
        SRC_MAX
    };

    enum MidiPPQN {
        MPPQN_24,
        MPPQN_48,
        MPPQN_96,
        MPPQN_192,
        MPPQN_MAX
    };

    // these are NOTE values !
    enum ClkInDiv {
        CI_X1,
        CI_1d2,
        CI_1d4,
        CI_1d8,
        CI_1d16,
        CI_1d32,
        CI_1d64,
        CI_MAX
    };

    // there are mult/divisions of clock
    enum ClkOutDiv {
        CO_M64,
        CO_M32,
        CO_M16,
        CO_M8,
        CO_M4,
        CO_M2,
        CO_X1,
        CO_D2,
        CO_D4,
        CO_D8,
        CO_D16,
        CO_D32,
        CO_D64,
        CO_MAX
    };


    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }


    void setClockTargets(unsigned samples, unsigned trigs, bool useTrigs);
    void updateClockSampleTargets(unsigned samples);

    void calcInternalSampleTarget(const float &sampleRate, const ClkInDiv &div, const float &bpm, float &samples);
    void calcMidiSampleTarget(const float &lastClock, const ClkInDiv &div, const MidiPPQN &ppqn, float &samples);
    void calcClkInSampleTarget(const float &lastClock, const ClkInDiv &div, float &samples);

    float sampleRate_ = 0.0f;
    bool useTrigs_ = false;

    // track to see if they change!
    Source source_ = SRC_MAX;
    float bpm_ = 10000.0f;
    ClkInDiv clockInDiv_ = CI_MAX;
    MidiPPQN ppqn_ = MPPQN_MAX;

    unsigned sampleCount_ = 0;
    unsigned lastSampleCount_ = 0;

    bool toggleRunRequest_ = false;
    bool resetRequest_ = false;
    bool toggleUseTrigs_ = false;

    bool runState_ = true;


    Clock clocks_[MAX_CLK_OUT];

    // track cvs
    static const unsigned clockTrigTime = 512; //TODO: remove after testing
//    static const unsigned clockTrigTime = 64;
    static const unsigned uiTrigTime = 4000; // need a slower trig for UI
    float lastCv_[I_MAX] = {0.0f, 0.0f, 0.0f};
    unsigned clkTrigTime_[O_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned uiTrigTime_[O_MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    unsigned midiPPQNRate_[MPPQN_MAX] = {24, 48, 96, 192};

    inline float normValue(RangedAudioParameter &p) {
        return p.convertFrom0to1(p.getValue());
    }

    std::vector<float> clockInDivMults_;
    std::vector<float> clockOutDivMults_;
    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


