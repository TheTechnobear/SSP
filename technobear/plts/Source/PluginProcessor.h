#pragma once

#include "plaits/dsp/voice.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"
#include "ssp/RmsTrack.h"

#include <atomic>
#include <algorithm>


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
#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}

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
    } params_;

    void getRMS(float &lOut, float &rOut) {
        lOut = outRms_[0].lvl();
        rOut = outRms_[1].lvl();
    }

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
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

    static constexpr unsigned PltsBlock = 16;
    static constexpr float PltsMaxEngine = 15.0f;
    plaits::Voice voice;
    plaits::Patch patch;
    char shared_buffer[16384];

    ssp::RmsTrack outRms_[2];

    float trig_ =0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};




#if 0

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

static constexpr unsigned PltsBlock = 16;

struct PltsData {
    PltsData() {
        pitch_ = 0.0f;
        harmonics_ = 0.0f;
        timbre_ = 0.5f;
        morph_ = 0.5f;

        model_ = 0.0f;
        lpg_colour_ = 0.5f;
        decay_ = 0.5f;

        freq_mod_=0.0f;
        timbre_mod_=0.0f;
        morph_mod_=0.0f;

        trig_ = 0;
    }
    ~PltsData() {
    }

    plaits::Voice voice;
    plaits::Patch patch;
    char shared_buffer[16384];

    std::atomic<float>  pitch_;
    std::atomic<float>  harmonics_;
    std::atomic<float>  timbre_;
    std::atomic<float>  morph_;

    std::atomic<float>  model_;
    std::atomic<float>  lpg_colour_;
    std::atomic<float>  decay_;

    std::atomic<float>  fm_;
    std::atomic<float>  trig_;
    std::atomic<float>  level_;

    std::atomic<float>  freq_mod_;
    std::atomic<float>  timbre_mod_;
    std::atomic<float>  morph_mod_;
};


class Plts  : public AudioProcessor
{
public:
    Plts();
    ~Plts();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    int getNumParameters() override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    PltsData& data() { return data_;}


private:
    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);
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

    PltsData data_;
    float params_[Percussa::sspLast];

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override { return true;}
    static const String getInputBusName (int channelIndex);
    static const String getOutputBusName (int channelIndex);
    static BusesProperties getBusesProperties()
    {
        BusesProperties props;
        for (auto i = 0; i < I_MAX; i++) {
            props.addBus(true, getInputBusName(i), AudioChannelSet::mono());
        }
        for (auto i = 0; i < O_MAX; i++) {
            props.addBus(false, getOutputBusName(i), AudioChannelSet::mono());
        }
        return props;
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plts)
};

#endif
