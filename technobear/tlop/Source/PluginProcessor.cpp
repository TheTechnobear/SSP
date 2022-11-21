
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

    nInputs_ = rnboObj_.getNumInputChannels();
    inputBuffers_ = new RNBO::number *[nInputs_];
    for (int i = 0; i < nInputs_; i++) {
        inputBuffers_[i] = new RNBO::number[bufferSize_];
    }
    nOutputs_ = rnboObj_.getNumOutputChannels();
    outputBuffers_ = new RNBO::number *[nOutputs_];
    for (int i = 0; i < nOutputs_; i++) {
        outputBuffers_[i] = new RNBO::number[bufferSize_];
    }

    nParams_ = params_.rnboParams_.size();
    lastParamVals_ = new float[nParams_];
    for (int i = 0; i < 0; i++) {
        lastParamVals_[i] = -1.0;
    }


}

PluginProcessor::~PluginProcessor() {
    for (int i = 0; i < nInputs_; i++) {
        delete inputBuffers_[i];
    }
    delete inputBuffers_;
    for (int i = 0; i < nOutputs_; i++) {
        delete outputBuffers_[i];
    }
    delete outputBuffers_;
}

PluginProcessor::RnboParam::RnboParam(AudioProcessorValueTreeState &apvt, StringRef id, unsigned idx) :
    id_(id), idx_(idx), val_(*apvt.getParameter(id)) {

    RNBO::CoreObject rnboObj_;
//    unsigned idx = rnboObj_.getParameterIndexForID(id_.c_str());
    desc_ = rnboObj_.getParameterName(idx_);
    rnboObj_.getParameterInfo(idx_, &info_);
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) {
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


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    RNBO::CoreObject rnboObj_;
    unsigned nParams = rnboObj_.getNumParameters();
    for (unsigned pn = 0; pn < nParams; pn++) {
        String id = rnboObj_.getParameterId(pn);
        RNBO::ParameterInfo info;
        rnboObj_.getParameterInfo(pn, &info);
        if (info.visible) {
            String desc = info.displayName;
            if (desc.length() == 0) desc = rnboObj_.getParameterName(pn);

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
    }
    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    RNBO::CoreObject rnboObj_;
    unsigned I_MAX = rnboObj_.getNumInputChannels();
    if (channelIndex < I_MAX) { return "In " + String(channelIndex); }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    RNBO::CoreObject rnboObj_;
    unsigned O_MAX = rnboObj_.getNumOutputChannels();
    if (channelIndex < O_MAX) { return "Out " + String(channelIndex); }
    return "ZZOut-" + String(channelIndex);
}

unsigned PluginProcessor::getLayerNumberSamples(unsigned lidx) {
    unsigned maxD = rnboObj_.getNumExternalDataRefs();
    if (lidx >= maxD || lidx >= MAX_LAYERS) return 0;

    auto &layer = layers_[lidx];
    if (layer.loopLayers_ == nullptr) return 0;

    auto info = rnboObj_.getExternalDataInfo(lidx);
    auto id = rnboObj_.getExternalDataId(lidx);
    return MAX_BUF_SIZE;
}

void PluginProcessor::fillLayerData(unsigned lidx, float *data, unsigned sz, float &cur, float &begin, float &end) {
    unsigned maxD = rnboObj_.getNumExternalDataRefs();
    if (lidx >= maxD || lidx >= MAX_LAYERS) return;
    auto &layer = layers_[lidx];

    if (layer.loopLayers_ == nullptr) return;
    for (int i = 0; i < MAX_BUF_SIZE && i < sz; i++) {
        data[i] = layer.loopLayers_[i];
    }
    cur = layer.cur_;
    begin = layer.begin_;
    end = layer.end_;

//    RNBO::ExternalDataInfo info = rnboObj_.getExternalDataInfo(layer);
}

void PluginProcessor::freeLayer(RNBO::ExternalDataId id, char *data) {
    delete (float *) (data);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);

    if (samplesPerBlock != bufferSize_) {
        bufferSize_ = samplesPerBlock;
        for (int i = 0; i < nInputs_; i++) {
            delete inputBuffers_[i];
            inputBuffers_[i] = new RNBO::number[bufferSize_];
        }
        for (int i = 0; i < nOutputs_; i++) {
            delete outputBuffers_[i];
            outputBuffers_[i] = new RNBO::number[bufferSize_];
        }
    }

    rnboObj_.prepareToProcess(sampleRate, samplesPerBlock);

    // allocate loop layers
    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layers_[i];
        layer.loopLayers_ = nullptr;
        for (int l = 0; l < rnboObj_.getNumExternalDataRefs(); l++) {
            auto id = rnboObj_.getExternalDataId(l);
            if (id == std::string("layer") + std::to_string(i + 1)) {
                RNBO::Float32AudioBuffer type(1, sampleRate);
                layer.loopLayers_ = new float[MAX_BUF_SIZE];
                for (int x = 0; x < MAX_BUF_SIZE; x++) layer.loopLayers_[x] = 0.0f;
                rnboObj_.setExternalData(id, (char *) layer.loopLayers_,
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
            if (p->id_.find(lstr, 0) != std::string::npos) {
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
        if (lastParamVals_[pi] != val) {
            rnboObj_.setParameterValue(p->idx_, normValue(p->val_));
            lastParamVals_[pi] = val;
        }
        pi++;
    }


    for (int lidx = 0; lidx < MAX_LAYERS; lidx++) {
        auto &layer = layers_[lidx];
        // OUTPUT layer from RNBO  : SUM ,out 4 Layer Out , Rec Pos, 4 Play Pos
        layer.cur_ = buffer.getSample(1 + MAX_LAYERS + 1 + lidx, n - 1);
    }




    // later can perhaps use vector copies?
    {
        // process input
        for (unsigned c = 0; c < nInputs_; c++) {
            for (unsigned i = 0; i < n; i++) {
                inputBuffers_[c][i] = buffer.getSample(c, i);
            }
        }
    }
    rnboObj_.process(inputBuffers_, nInputs_, outputBuffers_, nOutputs_, bufferSize_);

    {
        // copy output
        for (unsigned c = 0; c < nOutputs_; c++) {
            for (unsigned i = 0; i < n; i++) {
                buffer.setSample(c, i, outputBuffers_[c][i]);
            }
        }
    }

}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this, params_.rnboParams_.size() / 16));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


