#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ssp/BaseProcessor.h"


#include "Algo.h"
#include <atomic>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>

class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    static Percussa::SSP::PluginDescriptor *createDescriptor();

    const String getName() const override { return JucePlugin_Name; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }

    static constexpr unsigned MAX_ENG = 4;

    std::shared_ptr<Algo> algo(unsigned e) { if (e < MAX_ENG) return algo_[e]; else return nullptr; }

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

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;


protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
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


    std::shared_ptr<Algo> createAlgo(unsigned);
    std::shared_ptr<Algo> algo_[MAX_ENG];
    AudioSampleBuffer outBufs_;
    std::vector<unsigned> algoDisplayOrder_;
    void writeToXml(juce::XmlElement &xml);
    void readFromXml(juce::XmlElement &xml);


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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


