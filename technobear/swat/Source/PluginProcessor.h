#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>
#include <unordered_map>
#include <memory>

#include "Algo.h"


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

    std::shared_ptr<Algo> algo() { return algo_;}
    void nextAlgo() {
        algoN_ = (algoN_ + 1)  % A_MAX ;
        algo_ = algos_[algoN_];
        // Logger::writeToLog("algo# " + String(algoN_));
    }

private:

    void initAlgos();

    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    enum {
        I_X_1,
        I_Y_1,
        I_Z_1,
        I_MAX
    };

    enum {
        O_A_1,
        O_B_1,
        O_MAX
    };

    float params_[Percussa::sspLast];

    std::unordered_map<unsigned, std::shared_ptr<Algo>> algos_;
    std::atomic<unsigned> algoN_;
    std::shared_ptr<Algo> algo_ = nullptr;

    AudioSampleBuffer outBufs_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


