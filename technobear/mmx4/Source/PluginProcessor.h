#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>

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

    void  setVCA(unsigned i, unsigned o, float v) { vca_[i][o] = v;}
    float getVCA(unsigned i, unsigned o) { return vca_[i][o];}

private:

    void calcBuffer(AudioSampleBuffer& buffer, unsigned in, unsigned vca, unsigned n);


    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    enum {
        I_SIG_1L,
        I_SIG_1R,
        I_SIG_2L,
        I_SIG_2R,
        I_SIG_3L,
        I_SIG_3R,
        I_SIG_4L,
        I_SIG_4R,
        I_VCA_1A,
        I_VCA_1B,
        I_VCA_1C,
        I_VCA_1D,
        I_VCA_2A,
        I_VCA_2B,
        I_VCA_2C,
        I_VCA_2D,
        I_VCA_3A,
        I_VCA_3B,
        I_VCA_3C,
        I_VCA_3D,
        I_VCA_4A,
        I_VCA_4B,
        I_VCA_4C,
        I_VCA_4D,
        I_MAX
    };

    enum {
        O_SIG_AL,
        O_SIG_AR,
        O_SIG_BL,
        O_SIG_BR,
        O_SIG_CL,
        O_SIG_CR,
        O_SIG_DL,
        O_SIG_DR,
        O_MAX
    };

    float params_[Percussa::sspLast];

    static constexpr unsigned MAX_SIG_IN = 4;
    static constexpr unsigned MAX_SIG_OUT = 4;

    std::atomic<float> vca_[MAX_SIG_IN][MAX_SIG_OUT];
    std::atomic<bool> ac_;

    AudioSampleBuffer outBufs_;
    AudioSampleBuffer workBuf_;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


