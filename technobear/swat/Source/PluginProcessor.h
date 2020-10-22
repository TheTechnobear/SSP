#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"
#include "Algo.h"

#include <atomic>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>

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

    static constexpr unsigned MAX_ENG = 4;

    std::shared_ptr<Algo> algo(unsigned e) { if (e < MAX_ENG) return algo_[e]; else return nullptr;}
    void nextAlgo(unsigned e) {
        if (e < MAX_ENG) {
            unsigned t = algo_[e]->type();
            std::vector<unsigned>::iterator it = std::find(algoDisplayOrder_.begin(), algoDisplayOrder_.end(), t);
            if (it == algoDisplayOrder_.end() || it == algoDisplayOrder_.end() - 1) it = algoDisplayOrder_.begin();
            else it++;
            t = *it;
            // Logger::writeToLog("algo# " + String(t));
            algo_[e] = createAlgo(t);
        }
    }

    void prevAlgo(unsigned e) {
        if (e < MAX_ENG) {
            unsigned t = algo_[e]->type();
            std::vector<unsigned>::iterator it = std::find(algoDisplayOrder_.begin(), algoDisplayOrder_.end(), t);
            if (it == algoDisplayOrder_.end() || it == algoDisplayOrder_.begin()) it = algoDisplayOrder_.end() - 1;
            else it--;
            t = *it;
            // Logger::writeToLog("algo# " + String(t));
            algo_[e] = createAlgo(t);
        }
    }


private:

    std::shared_ptr<Algo> createAlgo(unsigned);

    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    enum {
        I_X_1,
        I_Y_1,
        I_Z_1,
        I_X_2,
        I_Y_2,
        I_Z_2,
        I_X_3,
        I_Y_3,
        I_Z_3,
        I_X_4,
        I_Y_4,
        I_Z_4,
        I_MAX
    };

    enum {
        O_A_1,
        O_B_1,
        O_A_2,
        O_B_2,
        O_A_3,
        O_B_3,
        O_A_4,
        O_B_4,
        O_MAX
    };

    float params_[Percussa::sspLast];

    std::shared_ptr<Algo> algo_[MAX_ENG];

    AudioSampleBuffer outBufs_;

    std::vector<unsigned> algoDisplayOrder_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


