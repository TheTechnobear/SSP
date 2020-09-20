#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>

class Scales {
public:
    Scales() {;}
    ~Scales() {;}
    // unsigned floatToInt(float f) { return unsigned(f * MAX_SCALE);}
    // const String& getName(unsigned s) {}
    // static constexpr unsigned MAX_SCALE=1;
};


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

    unsigned getScale() { return scale_;}
    unsigned getRoot() { return root_;}
    float lastSig(unsigned i) { return lastSig_[i];}

private:
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


    std::atomic<unsigned> scale_;
    std::atomic<unsigned> root_;

    static constexpr unsigned MAX_SIG = 4;
    std::atomic<float> lastSig_[MAX_SIG];
    std::atomic<float> lastTrig_[MAX_SIG];

    Random randomGen_;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


