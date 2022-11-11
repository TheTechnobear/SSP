
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
    for(int i=0;i<0;i++) {
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
        rnboObj_.getParameterInfo(i,&info);
        if(info.visible) {
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
        if(info.visible) {
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

unsigned PluginProcessor::getLayerNumberSamples(unsigned layer) {
    unsigned maxD = rnboObj_.getNumExternalDataRefs();
    if(layer >= maxD) return 0;
    auto info = rnboObj_.getExternalDataInfo(layer);
    auto id = rnboObj_.getExternalDataId(layer);

}

void PluginProcessor::fillLayerData(unsigned layer, float* data, unsigned sz) {
    unsigned maxD = rnboObj_.getNumExternalDataRefs();
    if(layer >= maxD) return;

    RNBO::ExternalDataInfo info = rnboObj_.getExternalDataInfo(layer);
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
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();


    // set parameters up for patch, only set on change
    unsigned pi=0;
    for (auto &p: params_.rnboParams_) {
        float val = p->val_.getValue();
        if(lastParamVals_[pi]!=val) {
            rnboObj_.setParameterValue(p->idx_, normValue(p->val_));
            lastParamVals_[pi]=val;
        }
        pi++;
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
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


