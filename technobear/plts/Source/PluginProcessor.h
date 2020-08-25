#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>

#include "plaits/dsp/voice.h"

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

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

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

protected:
    float cv2Pitch(float r) {
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first C note
        static constexpr float p2 = 0.21187f; // second C note
        static constexpr float scale = 12.0f / (p2 - p1);
        float arg = r;
        arg = arg - p1;
        arg = arg * scale;
        return arg;
    }

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

        //TODO?
        // VCA
        // LPG
        // TRANS

        I_MAX
    };
    enum {
        O_OUT,
        O_AUX,
        O_MAX
    };

    PltsData data_;
    float params_[Percussa::sspLast];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plts)
};


