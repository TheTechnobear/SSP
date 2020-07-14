#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>

#include "rings/dsp/part.h"
#include "rings/dsp/patch.h"
#include "rings/dsp/strummer.h"
#include "rings/dsp/string_synth_part.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

static constexpr unsigned RingsBlock = 16;

struct RngsData {
    RngsData() {
        f_pitch = 34.0f;
        f_structure = 0.45f;
        f_brightness = 0.5f;
        f_damping = 0.5f;
        f_position = 0.5f;

        f_polyphony = 0.0f;
        f_model = 0.0f;

        f_bypass = 0.0f;
        f_easter_egg = 0.0f;

        f_internal_exciter = 1;
        f_internal_strum = 0;
        f_internal_note = 1;

        f_trig = 0;

        f_in_gain = 0.0f;

        in_level = 0.0f;
        kNoiseGateThreshold = 0.00003f;

        iobufsz = RingsBlock;
        in = new float[iobufsz];
        out = new float[iobufsz];
        aux = new float[iobufsz];
    }
    ~RngsData() {
        delete [] in;
        delete [] out;
        delete [] aux;
        in = nullptr;
        out = nullptr;
        aux = nullptr;
    }

    // poly - 1-4 button
    // mode - 1-4 button

    // frequency = para+cvin
    // structure = para+cvin
    // brightness = para+cvin
    // damping = para+cvin
    // position = para+cvin

    // strum = cvin
    // v/oct = cvin  (note: v/oct diff from frequency)
    // in = cvin

    // odd = audio out
    // even = audio out



    std::atomic<float>  f_pitch;
    std::atomic<float>  f_structure;
    std::atomic<float>  f_brightness;
    std::atomic<float>  f_damping;
    std::atomic<float>  f_position;
    std::atomic<float>  f_polyphony;
    std::atomic<float>  f_model;

    std::atomic<float>  f_bypass;
    std::atomic<float>  f_easter_egg;

    std::atomic<float>  f_internal_strum;
    std::atomic<float>  f_internal_exciter;
    std::atomic<float>  f_internal_note;

    std::atomic<float>  f_in_gain;

    std::atomic<bool>   f_trig;

    rings::Part part;
    rings::PerformanceState performance_state;
    rings::StringSynthPart string_synth;
    rings::Strummer strummer;
    rings::Patch patch;


    float kNoiseGateThreshold;
    float in_level;

    float *in, *out, *aux;
    int iobufsz;

    static const int REVERB_SZ = 32768;
    uint16_t buffer[REVERB_SZ];
};


class Rngs  : public AudioProcessor
{
public:
    Rngs();
    ~Rngs();

    void write();

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
    RngsData& data() { return data_;}

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

    void writeToJson();
    void readFromJson();
    String fileFromIdx(int idx, bool&);

private:
    int currentProgram_ = -1;
    enum {
        I_IN,
        I_STRUM,
        I_VOCT,
        I_FM,
        I_STUCTURE,
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

    RngsData data_;
    float params_[Percussa::sspLast];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Rngs)
};


