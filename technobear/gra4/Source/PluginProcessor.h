#pragma once


#include "ssp/BaseProcessor.h"

#include <atomic>
#include <algorithm>

#include "RNBO.h"


#include <juce_audio_formats/juce_audio_formats.h>

using namespace juce;

namespace ID {
#define PARAMETER_ID(str) constexpr const char* str { #str };
constexpr const char *separator{":"};


//common
PARAMETER_ID (gain)

// rec- only
PARAMETER_ID (mode)
PARAMETER_ID (begin)
PARAMETER_ID (end)
PARAMETER_ID (loop)
PARAMETER_ID (rec)
PARAMETER_ID (mon)

// layer only
PARAMETER_ID (layer)

PARAMETER_ID (start)
PARAMETER_ID (length)
PARAMETER_ID (rate)
PARAMETER_ID (pan)
PARAMETER_ID (size)


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


    std::string getLayerFile(unsigned lidx) { return layers_[lidx].fileName_; }

    bool loadLayerFile(unsigned lidx, const std::string &fn, bool wait=true);
    bool saveLayerFile(unsigned lidx, const std::string &fn);

    struct LayerParams {
        using Parameter = juce::RangedAudioParameter;
        explicit LayerParams(AudioProcessorValueTreeState &apvt, unsigned ln);
        Parameter &start_;
        Parameter &length_;
        Parameter &rate_;
        Parameter &pan_;
        Parameter &gain_;
        Parameter &size_;
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

    static unsigned constexpr MAX_LAYERS = 4;
    enum {
        I_IN,
        I_LAYER1_TRIG,
        I_LAYER1_START,
        I_LAYER1_LENGTH,
        I_LAYER1_RATE,
        I_LAYER1_PAN,

        I_LAYER2_TRIG,
        I_LAYER2_START,
        I_LAYER2_LENGTH,
        I_LAYER2_RATE,
        I_LAYER2_PAN,

        I_LAYER3_TRIG,
        I_LAYER3_START,
        I_LAYER3_LENGTH,
        I_LAYER3_RATE,
        I_LAYER3_PAN,

        I_LAYER4_TRIG,
        I_LAYER4_START,
        I_LAYER4_LENGTH,
        I_LAYER4_RATE,
        I_LAYER4_PAN,

        I_REC_MODE,
//        I_REC_BEGIN,
//        I_REC_END,
        I_MAX
    };

    enum {
        O_SUM_L,
        O_SUM_R,
        O_LAYER1_L,
        O_LAYER1_R,
        O_LAYER2_L,
        O_LAYER2_R,
        O_LAYER3_L,
        O_LAYER3_R,
        O_LAYER4_L,
        O_LAYER4_R,
        O_MAX
    };

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    static void freeLayer(RNBO::ExternalDataId id, char *data);
    void customFromXml(juce::XmlElement *) override;
    void customToXml(juce::XmlElement *) override;
private:

    void allocateAllLayersData();
    void allocateLayerData(unsigned layer, unsigned newSize);
    void allocateRnboIO(unsigned blocksize);
    void setExternalData(unsigned lidx, float* newData, float newSize);

    struct {
        RNBO::CoreObject patch_;
        RNBO::number **inputBuffers_;
        RNBO::number **outputBuffers_;
        int nInputs_ = 0;
        int nOutputs_ = 0;
        int nParams_ = 0;
        int rnboIOSize_ = 0;
        float *lastParamVals_;
    } rnbo_;


    std::atomic_flag loadData_lock_ = ATOMIC_FLAG_INIT;
    struct {
        float *data_ = nullptr;
        float size_ = 0;
        float pSize_ = 0;
    } loadData_[MAX_LAYERS];


    struct { ;
        float start_ = 0.0f;
        float len_ = 1.0f;
        float cur_ = 0.0f;
        bool isRec_ = false;
        float recCur_ = 0.0f;
        float *data_ = nullptr;
        unsigned size_ = 0;
        std::string fileName_;
        float maxLayerSize_=180.0f;
    } layers_[MAX_LAYERS];

    unsigned sampleRate_ = 48000;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override {
        return true;
    }

#if __APPLE__
    static constexpr bool loadOnAudioThread_ = false;
#else
    static constexpr bool loadOnAudioThread_ = true;
#endif

    std::map<std::string, unsigned> nameToRnboIdMap_;

    static const String getInputBusName(int channelIndex);
    static const String getOutputBusName(int channelIndex);
    AudioFormatManager formatManager_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};


