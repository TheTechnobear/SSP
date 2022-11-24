#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "RNBO.h"

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


PARAMETER_ID (mode)
PARAMETER_ID (begin)
PARAMETER_ID (end)
PARAMETER_ID (loop)
PARAMETER_ID (gain)

// rec- only
PARAMETER_ID (rec)
PARAMETER_ID (mon)

// layer only
PARAMETER_ID (layer)
PARAMETER_ID (rate)
PARAMETER_ID (crossfade)


#undef PARAMETER_ID
}


class PluginProcessor : public ssp::BaseProcessor {
public:
    explicit PluginProcessor();
    explicit PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts, AudioProcessorValueTreeState::ParameterLayout layout);
    ~PluginProcessor();

    const String getName() const override { return JucePlugin_Name; }


    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer &, MidiBuffer &) override;

    AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override { return true; }


    struct LayerParams {
        using Parameter = juce::RangedAudioParameter;
        explicit LayerParams(AudioProcessorValueTreeState &apvt, unsigned ln);
        Parameter &mode_;
        Parameter &rate_;
        Parameter &begin_;
        Parameter &end_;
        Parameter &loop_;
        Parameter &xfade_;
        Parameter &gain_;
    };

    struct RecParams {
        using Parameter = juce::RangedAudioParameter;
        explicit RecParams(AudioProcessorValueTreeState &apvt);
        Parameter &layer_;
        Parameter &mode_;
        Parameter &begin_;
        Parameter &end_;
        Parameter &loop_;
        Parameter &gain_;
        Parameter &mon_;
    };

    struct PluginParams {
        using Parameter = juce::RangedAudioParameter;
        explicit PluginParams(juce::AudioProcessorValueTreeState &);

        std::unique_ptr<RecParams> recParams_;
        std::vector<std::unique_ptr<LayerParams>> layers_;
    } params_;

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

    void fillLayerData(unsigned layer, float *data, unsigned sz,
                       float &cur, float &begin, float &end,
                       bool &isRec, float &recCur);

    unsigned numLayers() { return MAX_LAYERS; }

    unsigned layerSampleSize(unsigned layer);

    static unsigned constexpr MAX_LAYERS = 4;
    static unsigned constexpr MAX_BUF_SIZE = (48000 * 10);

    enum {
        I_IN,
        I_REC_MODE,
        I_REC_BEGIN,
        I_REC_END,
        I_LAYER1_RATE,
        I_LAYER1_BEGIN,
        I_LAYER1_END,
        I_LAYER2_RATE,
        I_LAYER2_BEGIN,
        I_LAYER2_END,
        I_LAYER3_RATE,
        I_LAYER3_BEGIN,
        I_LAYER3_END,
        I_LAYER4_RATE,
        I_LAYER4_BEGIN,
        I_LAYER4_END,
        I_MAX
    };

    enum {
        O_SUM,
        O_LAYER1,
        O_LAYER2,
        O_LAYER3,
        O_LAYER4,
//        O_REC_POS,
//        O_LAYER1_POS,
//        O_LAYER2_POS,
//        O_LAYER3_POS,
//        O_LAYER4_POS,
        O_MAX
    };

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    static void freeLayer(RNBO::ExternalDataId id, char *data);
private:

    int bufferSize_ = 128;
    struct {
        RNBO::CoreObject patch_;
        RNBO::number **inputBuffers_;
        RNBO::number **outputBuffers_;
        int nInputs_ = 0;
        int nOutputs_ = 0;
        int nParams_ = 0;
        float *lastParamVals_;
    } rnbo_;


    struct { ;
        float begin_;
        float end_;
        float cur_;
        bool isRec_;
        float recCur_;
        float *loopLayers_;
    } layers_[MAX_LAYERS];

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

    std::map<std::string, unsigned> nameToRnboIdMap_;

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


