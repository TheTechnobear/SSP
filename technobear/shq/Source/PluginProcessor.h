#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>

#include "Quantizer.h"


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

class PluginProcessor  : public AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor();

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

    unsigned scale() { return scale_;}
    void scale(unsigned v) { scale_ = v;}

    unsigned root() { return root_;}
    void root(unsigned v) {root_ = v;}

    bool quant() { return quant_;}
    void quant(bool v) {quant_ = v;}

    float lastSig(unsigned i) { return lastSig_[i];}

    Quantizer& quantizer() { return quantizer_;}

private:

    float cv2Pitch(float r) {
#ifndef __APPLE__        
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first A note
        static constexpr float p2 = 0.21187f; // second A note
#else 
        static constexpr float p1 = 0.0f; // first A note
        static constexpr float p2 = 0.2f; // second A note
#endif         
        static constexpr float scale = 12.0f / (p2 - p1);
        float arg = r;
        arg = arg - p1;
        arg = arg * scale;
        return arg;
    }

    float pitch2Cv(float r) {
#ifndef __APPLE__        
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first A note
        static constexpr float p2 = 0.21187f; // second A note
#else 
        static constexpr float p1 = 0.0f; // first A note
        static constexpr float p2 = 0.2f; // second A note
#endif         
        static constexpr float iscale = (p2 - p1) / 12.0f;
        float arg = r;
        arg = arg * iscale; 
        arg = arg + p1;
        return arg;
    }

    float processCV(float value, float scale, float root);
    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    enum {
        I_SIG_1,
        I_TRIG_1,
        I_SIG_2,
        I_TRIG_2,
        I_SIG_3,
        I_TRIG_3,
        I_SIG_4,
        I_TRIG_4,
        I_SCALE,
        I_ROOT,
        I_MAX
    };

    enum {
        O_SIG_1,
        O_TRIG_1,
        O_SIG_2,
        O_TRIG_2,
        O_SIG_3,
        O_TRIG_3,
        O_SIG_4,
        O_TRIG_4,
        O_SCALE,
        O_ROOT,
        O_MAX
    };

    float params_[Percussa::sspLast];


    std::atomic<unsigned>   scale_;
    std::atomic<unsigned>   root_;
    std::atomic<bool>       quant_;

    static constexpr unsigned MAX_SIG = 4;
    std::atomic<float> lastSig_[MAX_SIG];
    std::atomic<float> lastTrig_[MAX_SIG];

    Random randomGen_;
    Quantizer quantizer_;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


