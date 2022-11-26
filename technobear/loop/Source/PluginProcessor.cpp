
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"


inline float constrainFloat(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();

    rnbo_.nInputs_ = rnbo_.patch_.getNumInputChannels();
    rnbo_.inputBuffers_ = new RNBO::number *[rnbo_.nInputs_];
    rnbo_.nOutputs_ = rnbo_.patch_.getNumOutputChannels();
    rnbo_.outputBuffers_ = new RNBO::number *[rnbo_.nOutputs_];
    for (int i = 0; i < rnbo_.nInputs_; i++) {
        rnbo_.inputBuffers_[i] = nullptr;
    }
    for (int i = 0; i < rnbo_.nOutputs_; i++) {
        rnbo_.outputBuffers_[i] = nullptr;
    }

    rnbo_.nParams_ = rnbo_.patch_.getNumParameters();
    // we need to be able to reliably for from id->idx
    RNBO::ParameterInfo info;
    for (int i = 0; i < rnbo_.nParams_; i++) {
        std::string pid = rnbo_.patch_.getParameterId(i);
        rnbo_.patch_.getParameterInfo(i, &info);
        if (info.visible) {
            nameToRnboIdMap_.insert(std::make_pair(pid, i));
        }
    }

    for (int i = 0; i < MAX_LAYERS; i++) {
        for (int ext_idx = 0; ext_idx < rnbo_.patch_.getNumExternalDataRefs(); ext_idx++) {
            auto id = rnbo_.patch_.getExternalDataId(ext_idx);
            if (id == std::string("layer") + std::to_string(i + 1)) {
                layerBufMap_[i] = ext_idx;
            }
        }
    }

    rnbo_.lastParamVals_ = new float[rnbo_.nParams_];
    for (int i = 0; i < rnbo_.nParams_; i++) {
        rnbo_.lastParamVals_[i] = -100.0;
    }

    allocateAllLayersData();

#if DEBUG
    assert(rnbo_.nInputs_ == I_MAX + 2); // not using rec begin,end
    assert(rnbo_.nOutputs_ == O_MAX + 5); // don't output positions
#endif

}

PluginProcessor::~PluginProcessor() {
    for (int i = 0; i < rnbo_.nInputs_; i++) {
        delete rnbo_.inputBuffers_[i];
    }
    delete rnbo_.inputBuffers_;
    for (int i = 0; i < rnbo_.nOutputs_; i++) {
        delete rnbo_.outputBuffers_[i];
    }
    delete rnbo_.outputBuffers_;
}

std::string getLayerPid(unsigned ln, const char *pid) {
    return std::string("layer") + std::to_string(ln + 1) + "/" + std::string(pid);
}

PluginProcessor::LayerParams::LayerParams(AudioProcessorValueTreeState &apvt, unsigned ln) :
    mode_(*apvt.getParameter(getLayerPid(ln, "mode"))),
    rate_(*apvt.getParameter(getLayerPid(ln, "rate"))),
    begin_(*apvt.getParameter(getLayerPid(ln, "begin"))),
    end_(*apvt.getParameter(getLayerPid(ln, "end"))),
    loop_(*apvt.getParameter(getLayerPid(ln, "loop"))),
    xfade_(*apvt.getParameter(getLayerPid(ln, "xfade"))),
    gain_(*apvt.getParameter(getLayerPid(ln, "gain"))),
    size_(*apvt.getParameter(getLayerPid(ln, "size"))) {
}


PluginProcessor::RecParams::RecParams(AudioProcessorValueTreeState &apvt) :
    layer_(*apvt.getParameter("rec-layer")),
    mode_(*apvt.getParameter("rec-mode")),
    begin_(*apvt.getParameter("rec-begin")),
    end_(*apvt.getParameter("rec-end")),
    loop_(*apvt.getParameter("rec-loop")),
    gain_(*apvt.getParameter("rec-gain")),
    mon_(*apvt.getParameter("rec-mon")) {

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    recParams_(std::make_unique<RecParams>(apvt)) {
    RNBO::CoreObject patch_;
    std::map<std::string, unsigned> nameToRnboIdMap_;

    RNBO::ParameterInfo info;
    for (int i = 0; i < patch_.getNumParameters(); i++) {
        std::string pid = patch_.getParameterId(i);
        patch_.getParameterInfo(i, &info);
        if (info.visible) {
            nameToRnboIdMap_.insert(std::make_pair(pid, i));
        }
    }

    for (unsigned i = 0; i < MAX_LAYERS; i++) {
        auto layer = std::make_unique<LayerParams>(apvt, i);
        layers_.push_back(std::move(layer));
    }
}


//** this is static **//
AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    RNBO::CoreObject patch_;
    std::map<std::string, unsigned> nameToRnboIdMap_;

    RNBO::ParameterInfo info;
    for (int i = 0; i < patch_.getNumParameters(); i++) {
        std::string pid = patch_.getParameterId(i);
        patch_.getParameterInfo(i, &info);
        if (info.visible) {
            nameToRnboIdMap_.insert(std::make_pair(pid, i));
        }
    }


    for (auto &param: nameToRnboIdMap_) {
        std::string id = param.first;
        int pn = param.second;

        RNBO::ParameterInfo info;
        patch_.getParameterInfo(pn, &info);
        String desc = info.displayName;
        if (desc.length() == 0) desc = patch_.getParameterName(pn);

        if (info.enumValues) {
            juce::StringArray choices;
            for (unsigned i = 0; i < info.steps; i++) {
                choices.add(info.enumValues[i]);
            }
            params.add(std::make_unique<ssp::BaseChoiceParameter>(id, desc, choices, info.initialValue));
        } else {
            if (info.steps < 2) {
                params.add(std::make_unique<ssp::BaseFloatParameter>(id, desc, info.min, info.max, info.initialValue));
            } else if (info.steps == 2) {
                params.add(std::make_unique<ssp::BaseBoolParameter>(id, desc, info.initialValue > 0.5f));
            } else {
                float inc = (info.max - info.min) / (info.steps - 1);
                params.add(std::make_unique<ssp::BaseFloatParameter>(id, desc, info.min, info.max, info.initialValue, inc));
            }
        }
    }

    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    if (channelIndex >= I_LAYER1_RATE && channelIndex <= I_LAYER4_END) {
        unsigned nLayerSigs = I_LAYER2_RATE - I_LAYER1_RATE;
        unsigned layer = (channelIndex - I_LAYER1_RATE) / nLayerSigs;
        unsigned sig = ((channelIndex - I_LAYER1_RATE) - layer * nLayerSigs) % nLayerSigs;
        std::string lstr = std::string("Layer ") + std::to_string(layer + 1);
        switch (sig) {
            case 0:
                return lstr + std::string(" Rate");
            case 1:
                return lstr + std::string(" Begin");
            case 2:
                return lstr + std::string(" End");
            default :
                return lstr + std::string("ZZ ") + std::to_string(sig);
        }
    }

    switch (channelIndex) {
        case I_IN :
            return String("IN");
        case I_REC_MODE :
            return String("Rec Mode");
//        case I_REC_BEGIN :
//            return String("Rec Begin");
//        case I_REC_END :
//            return String("Rec End");
        default: {
        }
    }

//    if (channelIndex < I_MAX) { return "In " + String(channelIndex); }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    switch (channelIndex) {
        case O_SUM :
            return String("Sum");
        default: {
            if (channelIndex < O_MAX) {
                unsigned nLayerSigs = O_LAYER2 - O_LAYER1;
                unsigned layer = (channelIndex - O_LAYER1) / nLayerSigs;
                unsigned sig = ((channelIndex - O_LAYER1) - layer * nLayerSigs) % nLayerSigs;
                std::string lstr = std::string("Layer ") + std::to_string(layer + 1);
                switch (sig) {
                    case 0:
                        return lstr + std::string(" Out");
                    default :
                        return lstr + std::string(" ZZ ") + std::to_string(sig);
                }
            }
        }
    }

    if (channelIndex < O_MAX) { return "Out " + String(channelIndex); }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::fillLayerData(unsigned lidx,
                                    float *data, unsigned sz,
                                    float &cur, float &begin, float &end,
                                    bool &isRec, float &recCur
) {
    if ( lidx >= MAX_LAYERS) return;
    auto &layer = layers_[lidx];

    if (layer.data_ == nullptr) return;

    unsigned dinc = layer.size_ / sz;
    for (int i = 0; i < layer.size_ && i < sz; i++) {
        data[i] = layer.data_[i * dinc];
    }

    cur = layer.cur_;
    begin = layer.begin_;
    end = layer.end_;
    isRec = layer.isRec_;
    recCur = layer.recCur_;
}

void PluginProcessor::freeLayer(RNBO::ExternalDataId id, char *data) {
    delete (float *) (data);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);

    if (sampleRate_ != sampleRate) {
        // force layer data to be reallocated
        sampleRate_ = sampleRate;
        for (int i = 0; i < MAX_LAYERS; i++) {
            auto &layer = layers_[i];
            layer.data_ = nullptr;
            layer.size_ = 0;
        }
    }


    allocateRnboIO(samplesPerBlock);

    rnbo_.patch_.prepareToProcess(sampleRate, samplesPerBlock);

    allocateAllLayersData();
}

void PluginProcessor::allocateRnboIO(unsigned samplesPerBlock) {
    if (samplesPerBlock != rnbo_.rnboIOSize_) {
        rnbo_.rnboIOSize_ = samplesPerBlock;
        for (int i = 0; i < rnbo_.nInputs_; i++) {
            delete rnbo_.inputBuffers_[i];
            rnbo_.inputBuffers_[i] = new RNBO::number[rnbo_.rnboIOSize_];
        }
        for (int i = 0; i < rnbo_.nOutputs_; i++) {
            delete rnbo_.outputBuffers_[i];
            rnbo_.outputBuffers_[i] = new RNBO::number[rnbo_.rnboIOSize_];
        }
    }
}


void PluginProcessor::allocateAllLayersData() {
    for (unsigned i = 0; i < MAX_LAYERS; i++) {
        unsigned newSize = normValue(params_.layers_[i]->size_);
        float bufSize = newSize * sampleRate_;
        allocateLayerData(i, bufSize);
    }
}

void PluginProcessor::allocateLayerData(unsigned lidx, unsigned newSize) {
    auto &layer = layers_[lidx];
    if (layer.size_ != newSize) {
        if (layer.data_ != nullptr) {
            // dont delete, rnbo will do this with freLayer
        }
        RNBO::Float32AudioBuffer type(1, sampleRate_);
        layer.size_ = newSize;
        layer.data_ = new float[layer.size_];
        for (int x = 0; x < layer.size_; x++) {
            layer.data_[x] = 0.0f;
        }

        std::string id = std::string("layer") + std::to_string(lidx + 1);
        rnbo_.patch_.setExternalData(id.c_str(), (char *) layer.data_,
                                     layer.size_ * sizeof(float) / sizeof(char),
                                     type, &freeLayer);
    }
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();

    allocateAllLayersData();

    for (auto &iter: nameToRnboIdMap_) {
        auto &pid = iter.first;
        int pidx = iter.second;

        auto p = getParameter(pid);
        float val = p->getValue();
        if (rnbo_.lastParamVals_[pidx] != val) {
            rnbo_.patch_.setParameterValue(pidx, normValue(*p));
            rnbo_.lastParamVals_[pidx] = val;
        }
    }

    int recLayer = rnbo_.patch_.getParameterValue(nameToRnboIdMap_["rec-layer"]);

    {
        // process input
        for (unsigned c = 0; c < I_MAX; c++) {
            for (unsigned i = 0; i < n; i++) {
                rnbo_.inputBuffers_[c][i] = buffer.getSample(c, i);
            }
        }
    }

    rnbo_.patch_.process(rnbo_.inputBuffers_, rnbo_.nInputs_,
                         rnbo_.outputBuffers_, rnbo_.nOutputs_,
                         rnbo_.rnboIOSize_);

    // get positions
    for (int lidx = 0; lidx < MAX_LAYERS; lidx++) {
        auto &layer = layers_[lidx];

        layer.cur_ = rnbo_.outputBuffers_[1 + MAX_LAYERS + 1 + lidx][n - 1];
        layer.isRec_ = lidx == recLayer;
        layer.recCur_ = layer.isRec_ ? rnbo_.outputBuffers_[1 + MAX_LAYERS][n - 1] : 0.0f;
        layer.begin_ = normValue(*getParameter(getLayerPid(lidx, "begin")));
        layer.end_ = normValue(*getParameter(getLayerPid(lidx, "end")));
    }

    {
        // copy output
        for (unsigned c = 0; c < O_MAX; c++) {
            for (unsigned i = 0; i < n; i++) {
                buffer.setSample(c, i, rnbo_.outputBuffers_[c][i]);
            }
        }
    }

}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


