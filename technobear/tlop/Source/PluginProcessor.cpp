
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
    for (int i = 0; i < rnbo_.nInputs_; i++) {
        rnbo_.inputBuffers_[i] = new RNBO::number[bufferSize_];
    }
    rnbo_.nOutputs_ = rnbo_.patch_.getNumOutputChannels();
    rnbo_.outputBuffers_ = new RNBO::number *[rnbo_.nInputs_];
    for (int i = 0; i < rnbo_.nOutputs_; i++) {
        rnbo_.outputBuffers_[i] = new RNBO::number[bufferSize_];
    }

    rnbo_.nParams_ = params_.rnboParams_.size();


    // we need to be able to reliably for from id->idx
    RNBO::ParameterInfo info;
    for (int i = 0; i < rnbo_.nParams_; i++) {
        std::string pid = rnbo_.patch_.getParameterId(i);
        rnbo_.patch_.getParameterInfo(i, &info);
        if (info.visible) {
            nameToRnboIdMap_.insert(std::make_pair(pid, i));
        }
    }

    //fixme: move to parameter notification to set parameters
    rnbo_.lastParamVals_ = new float[rnbo_.nParams_];
    for (int i = 0; i < rnbo_.nParams_; i++) {
        rnbo_.lastParamVals_[i] = -1.0;
    }

#if DEBUG
    assert(rnbo_.nInputs_ == I_MAX); // no internal buffers
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

PluginProcessor::RnboParam::RnboParam(AudioProcessorValueTreeState &apvt, StringRef id, unsigned idx) :
    id_(id), idx_(idx), val_(*apvt.getParameter(id)) {

    RNBO::CoreObject rnboObj_;
//    unsigned idx = rnboObj_.getParameterIndexForID(id_.c_str());
    desc_ = rnboObj_.getParameterName(idx_);
    rnboObj_.getParameterInfo(idx_, &info_);
}



std::string getLayerPid(unsigned ln, const char *pid) {
    return std::string("layer") + std::to_string(ln+1) + "/" + std::string(pid);
}

PluginProcessor::LayerParams::LayerParams(AudioProcessorValueTreeState &apvt, unsigned ln) :
    mode_(*apvt.getParameter(getLayerPid(ln, "mode"))),
    rate_(*apvt.getParameter(getLayerPid(ln, "rate"))),
    begin_(*apvt.getParameter(getLayerPid(ln, "begin"))),
    end_(*apvt.getParameter(getLayerPid(ln, "end"))),
    loop_(*apvt.getParameter(getLayerPid(ln, "loop"))),
    crossfade_(*apvt.getParameter(getLayerPid(ln, "crossfade"))),
    gain_(*apvt.getParameter(getLayerPid(ln, "gain"))) {

}


PluginProcessor::RecParams::RecParams(AudioProcessorValueTreeState &apvt) :
    layer_(*apvt.getParameter("rec-layer")),
    mode_(*apvt.getParameter("rec-mode")),
    loop_(*apvt.getParameter("rec-loop")),
    gain_(*apvt.getParameter("rec-gain")),
    mon_(*apvt.getParameter("rec-mon")) {

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    recParams_(std::make_unique<RecParams>(apvt))
{
    {
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

    {
        RNBO::CoreObject rnboObj_;
        unsigned nParams = rnboObj_.getNumParameters();
        for (unsigned i = 0; i < nParams; i++) {
            RNBO::ParameterInfo info;
            String id = rnboObj_.getParameterId(i);
            rnboObj_.getParameterInfo(i, &info);
            if (info.visible) {
                rnboParams_.push_back(std::make_unique<RnboParam>(apvt, id, i));
            }
        }
    }
}



//** this is static **//
AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    RNBO::CoreObject patch_;
    std::map<std::string,unsigned> nameToRnboIdMap_;

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
    switch (channelIndex) {
        case I_IN :
            return String("IN");
        case I_REC_MODE :
            return String("Rec Mode");
        case I_REC_BEGIN :
            return String("Rec Begin");
        case I_REC_END :
            return String("Rec End");
        default: {
            if (channelIndex < I_MAX) {
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

unsigned PluginProcessor::layerSampleSize(unsigned lidx) {
    unsigned maxD = rnbo_.patch_.getNumExternalDataRefs();
    if (lidx >= maxD || lidx >= MAX_LAYERS) return 0;

    auto &layer = layers_[lidx];
    if (layer.loopLayers_ == nullptr) return 0;

//    auto info = rnboObj_.getExternalDataInfo(lidx);
//    auto id = rnboObj_.getExternalDataId(lidx);
    return MAX_BUF_SIZE;
}

void PluginProcessor::fillLayerData(unsigned lidx,
                                    float *data, unsigned sz,
                                    float &cur, float &begin, float &end,
                                    bool &isRec, float &recCur
) {
    unsigned maxD = rnbo_.patch_.getNumExternalDataRefs();
    if (lidx >= maxD || lidx >= MAX_LAYERS) return;
    auto &layer = layers_[lidx];

    if (layer.loopLayers_ == nullptr) return;
    for (int i = 0; i < MAX_BUF_SIZE && i < sz; i++) {
        data[i] = layer.loopLayers_[i];
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

    if (samplesPerBlock != bufferSize_) {
        bufferSize_ = samplesPerBlock;
        for (int i = 0; i < rnbo_.nInputs_; i++) {
            delete rnbo_.inputBuffers_[i];
            rnbo_.inputBuffers_[i] = new RNBO::number[bufferSize_];
        }
        for (int i = 0; i < rnbo_.nOutputs_; i++) {
            delete rnbo_.outputBuffers_[i];
            rnbo_.outputBuffers_[i] = new RNBO::number[bufferSize_];
        }
    }

    rnbo_.patch_.prepareToProcess(sampleRate, samplesPerBlock);

    // allocate loop layers
    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layers_[i];
        layer.loopLayers_ = nullptr;
        for (int l = 0; l < rnbo_.patch_.getNumExternalDataRefs(); l++) {
            auto id = rnbo_.patch_.getExternalDataId(l);
            if (id == std::string("layer") + std::to_string(i + 1)) {
                RNBO::Float32AudioBuffer type(1, sampleRate);
                layer.loopLayers_ = new float[MAX_BUF_SIZE];
                for (int x = 0; x < MAX_BUF_SIZE; x++) layer.loopLayers_[x] = 0.0f;
                rnbo_.patch_.setExternalData(id, (char *) layer.loopLayers_,
                                             MAX_BUF_SIZE * sizeof(float) / sizeof(char),
                                             type, &freeLayer);
            }
        }
    }
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();

    // set parameters up for patch, only set on change
    unsigned pi = 0;
    for (auto &p: params_.rnboParams_) {
        //fixme - optimise by grouping rnboParams_ into layers
        for (int lidx = 0; lidx < MAX_LAYERS; lidx++) {
            auto &layer = layers_[lidx];
            std::string lstr = std::string("layer") + std::to_string(lidx + 1);
            if (p->id_.find(lstr, 0) == 0) {
                if (p->id_.find("begin") != std::string::npos) {
                    layer.begin_ = normValue(p->val_);
                }
                if (p->id_.find("end") != std::string::npos) {
                    layer.end_ = normValue(p->val_);
                }
            }
        }

        // normal handling to RNBO
        float val = p->val_.getValue();
        if (rnbo_.lastParamVals_[pi] != val) {
            rnbo_.patch_.setParameterValue(p->idx_, normValue(p->val_));
            rnbo_.lastParamVals_[pi] = val;
        }
        pi++;
    }

    int recLayerIdx = rnbo_.patch_.getParameterIndexForID("rec-layer");
    int recLayer = rnbo_.patch_.getParameterValue(recLayerIdx);
    // get positions
    for (int lidx = 0; lidx < MAX_LAYERS; lidx++) {
        auto &layer = layers_[lidx];
        // OUTPUT layer from RNBO  : SUM ,out 4 Layer Out , Rec Pos, 4 Play Pos
        layer.cur_ = buffer.getSample(1 + MAX_LAYERS + 1 + lidx, n - 1);

        layer.isRec_ = lidx == recLayer;
        layer.recCur_ = layer.isRec_ ? buffer.getSample(1 + MAX_LAYERS, n - 1) : 0;
    }

    // later can perhaps use vector copies?
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
                         bufferSize_);

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
    return new ssp::EditorHost(this, new PluginEditor(*this, (params_.rnboParams_.size() / 16) + 1));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


