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

static constexpr unsigned CloudsBlock = 16;

struct CldsData {
    CldsData() {
        ltrig = false;

        iobufsz = CloudsBlock;
        ibuf = new clouds::ShortFrame[x->iobufsz];
        obuf = new clouds::ShortFrame[x->iobufsz];

        large_buf_size = (lsize <t_clds_tilde::LARGE_BUF ? t_clds_tilde::LARGE_BUF : lsize);
        large_buf = new uint8_t[large_buf_size];

        small_buf_size =  (ssize < t_clds_tilde::SMALL_BUF ? t_clds_tilde::SMALL_BUF : ssize);;
        small_buf = new uint8_t[small_buf_size];

        f_freeze = 0.0f;
        f_trig = 0.0f;
        f_position = 0.0f;
        f_size = 0.0f;
        f_pitch = 0.0f;
        f_density = 0.0f;
        f_texture = 0.0f;
        f_mix = 0.0f;
        f_spread = 0.0f;
        f_feedback = 0.0f;
        f_reverb = 0.0f;
        f_mode = 0.0f;
        f_mono = 0.0f;
        f_silence = 0.0f;
        f_bypass = 0.0f;
        f_lofi = 0.0f;

        processor.Init(
            large_buf,large_buf_size, 
            small_buf,small_buf_size);

        ltrig = false;
    }

    ~CldsData() {
        delete [] ibuf;
        delete [] obuf;
        delete [] small_buf;
        delete [] large_buf;
    }


    float f_freeze;
    float f_trig;
    float f_position;
    float f_size;
    float f_pitch;
    float f_density;
    float f_texture;
    float f_mix;
    float f_spread;
    float f_feedback;
    float f_reverb;
    float f_mode;
    float f_mono;
    float f_silence;
    float f_bypass;
    float f_lofi;

    clouds::GranularProcessor processor;
    bool ltrig;
    clouds::ShortFrame* ibuf;
    clouds::ShortFrame* obuf;
    int iobufsz;

    static constexpr unsigned LARGE_BUF = 524288;
    static constexpr unsigned SMALL_BUF = 262144;
    uint8_t* large_buf;
    int      large_buf_size;
    uint8_t* small_buf;
    int      small_buf_size;
};


class Clds  : public AudioProcessor
{
public:
    Clds();
    ~Clds();

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
    CldsData& data() { return data_;}

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
        I_LEFT,
        I_RIGHT,
        I_TRIG,
        I_VOCT,
        I_POS,
        I_SIZE,
        I_DENSITY,
        I_TEXT,
        // I_FREEZE,
        // I_BLEND,        
        I_MAX
    };
    enum {
        O_LEFT
        O_RIGHT,
        O_MAX
    };

    CldsData data_;
    float params_[Percussa::sspLast];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Clds)
};


