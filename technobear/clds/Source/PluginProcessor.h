#pragma once


#include "clouds/dsp/granular_processor.h"

#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>
#include <algorithm>


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}


static constexpr unsigned CloudsBlock = 32;

struct CldsData {
    CldsData() {
        iobufsz = CloudsBlock;
        ibuf = new clouds::ShortFrame[iobufsz];
        obuf = new clouds::ShortFrame[iobufsz];
        block_mem = new uint8_t[block_mem_sz];
        block_ccm = new uint8_t[block_ccm_sz];
        memset(block_mem, 0, sizeof(uint8_t)*block_mem_sz);
        memset(block_ccm, 0, sizeof(uint8_t)*block_ccm_sz);

        f_freeze    = 0.0f;
        f_position  = 0.0f;
        f_size      = 0.5f;
        f_pitch     = 0.0f;
        f_density   = -0.2f;
        f_texture   = 0.5f;
        f_mix       = 0.5f;
        f_spread    = 0.5f;
        f_feedback  = 0.1f;
        f_reverb    = 0.5f;
        f_mode      = 0.0f;
        f_in_gain   = 0.0f;

        // unused, since this is downsampling
        // and we are already at wrong SR
        f_mono      = 0.0f;
        f_lofi      = 0.0f;

        trig_or_freeze = 0.0f;

        processor.Init(
            block_mem, block_mem_sz,
            block_ccm, block_ccm_sz);

    }

    ~CldsData() {
        delete [] ibuf;
        delete [] obuf;
        delete [] block_ccm;
        delete [] block_mem;
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
    float f_in_gain;
    float trig_or_freeze;

    float f_mono;
    float f_lofi;


    clouds::GranularProcessor processor;
    clouds::ShortFrame* ibuf;
    clouds::ShortFrame* obuf;
    int iobufsz;

    static constexpr unsigned block_mem_sz =  118784;
    static constexpr unsigned block_ccm_sz =  65536 - 128;
    //static constexpr unsigned block_mem_sz =  (118784 * 3) / 2; // 48k vs 32k 
    //static constexpr unsigned block_ccm_sz =  ((65536 - 128) * 3 ) / 2 ; 
    uint8_t* block_mem;
    uint8_t* block_ccm;
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

    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);


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
        I_FREEZE,
        I_MIX,
        I_SPREAD,
        I_FEEDBACK,
        I_REVERB,
        I_MAX
    };

    enum {
        O_LEFT,
        O_RIGHT,
        O_MAX
    };

    CldsData data_;
    float params_[Percussa::sspLast];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Clds)
};


