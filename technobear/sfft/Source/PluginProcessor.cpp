
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
    rnbo_.outputBuffers_ = new RNBO::number *[rnbo_.nOutputs_];
    for (int i = 0; i < rnbo_.nOutputs_; i++) {
        rnbo_.outputBuffers_[i] = new RNBO::number[bufferSize_];
    }

    rnbo_.nParams_ = params_.rnboParams_.size();
    rnbo_.lastParamVals_ = new float[rnbo_.nParams_];
    for (int i = 0; i < rnbo_.nParams_; i++) {
        rnbo_.lastParamVals_[i] = -1.0;
    }
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
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();


    // set parameters up for patch, only set on change
    unsigned pi = 0;
    for (auto &p: params_.rnboParams_) {
        float val = p->val_.getValue();
        if (rnbo_.lastParamVals_[pi] != val) {
            rnbo_.patch_.setParameterValue(p->idx_, normValue(p->val_));
            rnbo_.lastParamVals_[pi] = val;
        }
        pi++;
    }
    // later can perhaps use vector copies?
    {
        // process input
        for (unsigned c = 0; c < rnbo_.nInputs_; c++) {
            for (unsigned i = 0; i < n; i++) {
                rnbo_.inputBuffers_[c][i] = buffer.getSample(c, i);
            }
        }
    }
    rnbo_.patch_.process(rnbo_.inputBuffers_, rnbo_.nInputs_, rnbo_.outputBuffers_, rnbo_.nOutputs_, bufferSize_);

    {
        // copy output
        for (unsigned c = 0; c < rnbo_.nOutputs_; c++) {
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


