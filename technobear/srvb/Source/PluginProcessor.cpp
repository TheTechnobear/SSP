
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMiniEditor.h"
#include "ssp/EditorHost.h"

inline float constrainFloat(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    lpfreq(*apvt.getParameter(ID::lpfreq)),
    feedback(*apvt.getParameter(ID::feedback)),
    mix(*apvt.getParameter(ID::mix)),
    freeze(*apvt.getParameter(ID::freeze)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::lpfreq, "LP Freq", 0.0f, 24000.0f, 24000.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::feedback, "Feedback", 0.0f, 100.0f, 10.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::mix, "Mix", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::freeze, "Freeze", false));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In L",
        "In R"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out L",
        "Out R"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    reverbSc_.Init(newSampleRate);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if(activityCount_==0) {
        for(int i=0;i<I_MAX;i++) {
            inActivity_[i]=buffer.getSample(i,0);
        }
    }
    bool stereoIn = inputEnabled[O_RIGHT];
    bool stereoOut = outputEnabled[O_RIGHT];

    inRms_[0].process(buffer, I_LEFT);
    if (stereoIn) inRms_[1].process(buffer, I_RIGHT);
    bool freeze = params_.freeze.getValue() > 0.5f;
    float mix = params_.mix.getValue();
    float imix = 1.0f - mix;
    reverbSc_.SetFeedback(freeze ? 1.0f : params_.feedback.getValue());
    reverbSc_.SetLpFreq(normValue(params_.lpfreq));

    int sz = buffer.getNumSamples();
    for (int s = 0; s < sz; s++) {
        float ils = freeze ? 0.0f : buffer.getSample(I_LEFT, s);
        float irs = freeze ? 0.0f : buffer.getSample(I_RIGHT, s);
        float ols = 0.0f, ors = 0.0f;
        reverbSc_.Process(ils, irs, &ols, &ors);

        buffer.setSample(O_LEFT, s, (ils * imix) + (ols * mix));
        buffer.setSample(O_RIGHT, s, (irs * imix) + (ors * mix));
    }

    outRms_[0].process(buffer, O_LEFT);
    if (stereoOut) outRms_[1].process(buffer, O_RIGHT);

    if(activityCount_==0) {
        for(int i=0;i<O_MAX;i++) {
            outActivity_[i]=buffer.getSample(i,0);
        }
    }
    activityCount_ = (activityCount_ + 1 ) % ACTIVITY_PERIOD;
}


AudioProcessorEditor *PluginProcessor::createEditor() {
#ifdef FORCE_COMPACT_UI
    return new ssp::EditorHost(this, new PluginMiniEditor(*this),true);
#else
    if (useCompactUI()) {
        return new ssp::EditorHost(this, new PluginMiniEditor(*this), useCompactUI());

    } else {
        return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI());
    }
#endif
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


