#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()), messageQueue_(MAX_MSGS) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::SigParams::SigParams(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    y_offset(*apvt.getParameter(getPID(pre, sn, ID::y_offset))),
    y_scale(*apvt.getParameter(getPID(pre, sn, ID::y_scale))),
    show(*apvt.getParameter(getPID(pre, sn, ID::show))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    t_scale(*apvt.getParameter(ID::t_scale)),
    freeze(*apvt.getParameter(ID::freeze)),
    trig_src(*apvt.getParameter(ID::trig_src)),
    trig_lvl(*apvt.getParameter(ID::trig_lvl)),
    ab_xy(*apvt.getParameter(ID::ab_xy)),
    cd_xy(*apvt.getParameter(ID::cd_xy)) {
    for (unsigned i = 0; i < MAX_SIG_IN; i++) {
        sigparams_.push_back(std::make_unique<SigParams>(apvt, ID::sig, i));
    }
}

static constexpr unsigned MAX_TIME_SPEC = 11;

struct TimeSpec {
    TimeSpec(String n, float v) : n_(n), v_(v) { ; }

    String n_;
    float v_;
} timeSpecs[MAX_TIME_SPEC] = {
    TimeSpec("1 mS", 0.001f),
    TimeSpec("5 mS", 0.005f),
    TimeSpec("10 mS", 0.010f),
    TimeSpec("20 mS", 0.020f),
    TimeSpec("50 mS", 0.050f),
    TimeSpec("100 mS", 0.100),
    TimeSpec("250 mS", 0.250),
    TimeSpec("500 mS", 0.500),
    TimeSpec("1 S", 1.0),
    TimeSpec("2 S", 2.0),
    TimeSpec("5 S", 5.0)
};


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    StringArray ts;
    for (unsigned i = 0; i < MAX_TIME_SPEC; i++) {
        ts.add(timeSpecs[i].n_);
    }

    StringArray trigs;
    trigs.add("None");
    trigs.add("IN A");
    trigs.add("IN B");
    trigs.add("IN C");
    trigs.add("IN D");
    trigs.add("Trig");

    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::t_scale, "Time", ts, 5));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::trig_src, "Trig", trigs, 0));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::trig_lvl, "Trig Lvl", -1.0f, 1.0f, 0.f));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::freeze, "Freeze", false));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::ab_xy, "AB XY", false));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::cd_xy, "CD XY", false));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::sig, "Signal", ID::separator);
    for (unsigned sn = 0; sn < MAX_SIG_IN; sn++) {
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::sig, sn, ID::y_scale), "Scale", -5.0f, 5.0f, 1.0f));
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::sig, sn, ID::y_offset), "Offset", -5.0f, 5.0f, 0.0f));
        sg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::sig, sn, ID::show), "Visible", true));
    }
    params.add(std::move(sg));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In A",
        "In B",
        "In C",
        "In D",
        "Trig"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out A",
        "Out B",
        "Out C",
        "Out D"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if (params_.freeze.getValue() > 0.5f) return;


    unsigned n = buffer.getNumSamples();
    unsigned tidx = params_.t_scale.convertFrom0to1((params_.t_scale.getValue()));
    float t = timeSpecs[tidx].v_;

    float tS = t * getSampleRate() / DIV_RES;

    float nextSample = lastSample_ + tS;
    float endSample = sampleCounter_ + n;


    if (nextSample - sampleCounter_ < -1) {
        // happens when we change timebase from slow to fast
        lastSample_ = sampleCounter_;
        nextSample = sampleCounter_;
    }

    while (nextSample < endSample) {
        float s = nextSample - sampleCounter_;

        if (s >= 0 && s < n - 1) {
            unsigned i0 = s, i1 = i0 + 1;
            float f1 = s - i0, f0 = 1.0f - f1;

            jassert(f0 >= 0.0f && f0 <= 1.0f);
            jassert(f1 >= 0.0f && f1 <= 1.0f);
            DataMsg msg;
            msg.sample_[0] = buffer.getSample(I_SIG_A, i0) * f0 + buffer.getSample(I_SIG_A, i1) * f1;
            msg.sample_[1] = buffer.getSample(I_SIG_B, i0) * f0 + buffer.getSample(I_SIG_B, i1) * f1;
            msg.sample_[2] = buffer.getSample(I_SIG_C, i0) * f0 + buffer.getSample(I_SIG_C, i1) * f1;
            msg.sample_[3] = buffer.getSample(I_SIG_D, i0) * f0 + buffer.getSample(I_SIG_D, i1) * f1;
            msg.trig_ = buffer.getSample(I_TRIG, i0) * f0 + buffer.getSample(I_TRIG, i1) * f1;
            if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
            lastSample_ = nextSample;
        } else if (s < 0) {
            unsigned i1 = 0;
            float f1 = s + 1.0f, f0 = 1.0f - f1;
            jassert(f0 >= 0.0f && f0 <= 1.0f);
            jassert(f1 >= 0.0f && f1 <= 1.0f);
            DataMsg msg;
            msg.sample_[0] = lastS_[0] * f0 + buffer.getSample(I_SIG_A, i1) * f1;
            msg.sample_[1] = lastS_[1] * f0 + buffer.getSample(I_SIG_B, i1) * f1;
            msg.sample_[2] = lastS_[2] * f0 + buffer.getSample(I_SIG_C, i1) * f1;
            msg.sample_[3] = lastS_[3] * f0 + buffer.getSample(I_SIG_D, i1) * f1;
            msg.trig_ = lastS_[4] * f0 + buffer.getSample(I_TRIG, i1) * f1;
            if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
            lastSample_ = nextSample;
        } else { // i1==n
            if (s + tS < n) {
                unsigned i1 = n - 1;
                DataMsg msg;
                msg.sample_[0] = buffer.getSample(I_SIG_A, i1);
                msg.sample_[1] = buffer.getSample(I_SIG_B, i1);
                msg.sample_[2] = buffer.getSample(I_SIG_C, i1);
                msg.sample_[3] = buffer.getSample(I_SIG_D, i1);
                msg.trig_ = buffer.getSample(I_TRIG, i1);
                if (!messageQueue_.try_enqueue(msg)) { ; } // queue full
                lastSample_ = nextSample;
            } else {
                unsigned i0 = n - 1;
                lastS_[0] = buffer.getSample(I_SIG_A, i0);
                lastS_[1] = buffer.getSample(I_SIG_B, i0);
                lastS_[2] = buffer.getSample(I_SIG_C, i0);
                lastS_[3] = buffer.getSample(I_SIG_D, i0);
                lastS_[4] = buffer.getSample(I_TRIG, i0);
            }
        }
        nextSample += tS;
    }

    sampleCounter_ = endSample;
//    static constexpr unsigned long MAX_SC = 480000 * 60 * 60; // 1 hour  @ 48k
//    if(sampleCounter_ > MAX_SC ) sampleCounter_ = sampleCounter_ % MAX_SC;
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this,new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


