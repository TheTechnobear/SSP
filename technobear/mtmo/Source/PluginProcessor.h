#pragma once

#include <readerwriterqueue.h>

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>


using namespace juce;

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };

 PARAMETER_ID (dummy)

#undef PARAMETER_ID
}

class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor() override = default;

    const String getName() const override { return JucePlugin_Name; }

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override;

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

         Parameter& dummy;
    } params_;

    moodycamel::ReaderWriterQueue<MidiMessage> &messageQueue() { return messageQueue_; }

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


    enum {
        I_MAX
    };

    enum {
        O_DUMMY_L,
        O_DUMMY_R,
        O_MAX
    };

private:
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    double clockTs_ = 0.0f;

    moodycamel::ReaderWriterQueue<MidiMessage> messageQueue_;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




