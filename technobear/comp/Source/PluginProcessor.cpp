
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMiniEditor.h"
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
    for(int i=0;i<I_MAX;i++) inActivity_[i]=0;
    for(int i=0;i<O_MAX;i++) outActivity_[i]=0;
}

PluginProcessor::~PluginProcessor() {
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    ratio(*apvt.getParameter(ID::ratio)),
    threshold(*apvt.getParameter(ID::threshold)),
    attack(*apvt.getParameter(ID::attack)),
    release(*apvt.getParameter(ID::release)),
    makeup(*apvt.getParameter(ID::makeup)),
    automakeup(*apvt.getParameter(ID::automakeup)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::ratio, "Ratio", 1.0f, 40.0f, 2.0f)); //1.0 -> 40.
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::threshold, "Threshold", -80.0f, 0.0f, -12.0f)); // 0.0 -> -80.
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::attack, "Attack", 0.001f, 10.0f, 0.1f));// 0.001 -> 10
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::release, "Release", 0.001f, 10.0f, 0.1f));// 0.001 -> 10
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::makeup, "Makeup", 0.0f, 80.0f, 0.0f)); //  0.0 -> 80
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::automakeup, "AutoM", true));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In L",
        "In R",
        "Sidechain"
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

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);
    compressor_.Init(sampleRate);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if(activityCount_==0) {
        for(int i=0;i<I_MAX;i++) {
            inActivity_[i]=buffer.getSample(i,0);
        }
    }

    size_t n = buffer.getNumSamples();
    float ratio = normValue(params_.ratio);
    float threshold = normValue(params_.threshold);
    float attack = normValue(params_.attack);
    float release = normValue(params_.release);
    float makeup = normValue(params_.makeup);
    bool automakeup = params_.automakeup.getValue() > 0.5f;

    bool sidechain = isInputEnabled(I_SIDECHAIN);
    bool stereoOut = isOutputEnabled(O_RIGHT);
    bool stereoIn = isOutputEnabled(O_RIGHT);


    compressor_.SetRatio(ratio);
    compressor_.SetThreshold(threshold);
    compressor_.SetAttack(attack);
    compressor_.SetRelease(release);
    if(automakeup) {
//        compressor_.SetMakeup(makeup);
        compressor_.AutoMakeup(automakeup);
    } else {
        compressor_.AutoMakeup(automakeup);
        compressor_.SetMakeup(makeup);
    }

    inRms_[I_LEFT].process(buffer, I_LEFT);
    if (stereoIn) inRms_[I_RIGHT].process(buffer, I_RIGHT);

    bool sidechainDone = false;
    for (unsigned i = I_LEFT; i <= I_RIGHT; i++) {
        if (isInputEnabled(i) && isOutputEnabled(i)) {
            for (int s = 0; s < n; s++) {
                float in = buffer.getSample(i, s);
                if (sidechain) {
                    if (!sidechainDone) { // only needed to be done once
                        float key = buffer.getSample(I_SIDECHAIN, s);
                        compressor_.Process(key);
                    }
                    buffer.setSample(i, s, compressor_.Apply(in));
                } else {
                    buffer.setSample(i, s, compressor_.Process(in));
                } // sidechain
            } // per sample
            if (sidechain) sidechainDone = true;
        }// io enabled
    } // channel

    if (stereoOut && !isInputEnabled(I_RIGHT)) {
        for (int s = 0; s < n; s++) {
            buffer.setSample(O_RIGHT, s, buffer.getSample(O_LEFT, s));
        }
    }

    outRms_[O_LEFT].process(buffer, O_LEFT);
    if (stereoOut) outRms_[O_RIGHT].process(buffer, O_RIGHT);

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


