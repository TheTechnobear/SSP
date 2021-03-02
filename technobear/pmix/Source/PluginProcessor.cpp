#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

// from : https://www.kvraudio.com/forum/viewtopic.php?t=148865

// -3dB at center, sin/cos taper
// LeftOut = cos(pan*pi/2) * MonoIn;
// RightOut = sin(pan*pi/2) * MonoIn;

// -3db at center, sqrt taper
// LeftOut = sqrt(pan) * MonoIn;
// RightOut = sqrt(1-pan) * MonoIn;

// -6db at center, linear taper
// LeftOut = pan * MonoIn;
// RightOut = (1-pan) * MonoIn;

// to do: optimise, use a sin lookup table
inline float panGain(bool left, float p) {
    static constexpr float PIdiv2 = M_PI / 2.0f;
    float pan = (p + 1.0f) / 2.0f;
    if (left) {
        return cosf(pan * PIdiv2);
        //return std::cosf(pan * PIdiv2);
    }
    return sinf(pan * PIdiv2);
    //return std::sinf(pan * PIdiv2);
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();
    initTracks();
}

String getPID(StringRef io, unsigned tn, StringRef id) {
    return io + String(ID::separator) + String(tn) + String(ID::separator) + id;
}


TrackData::TrackData(AudioProcessorValueTreeState &apvt, StringRef io, unsigned tn) :
    level{
        (*apvt.getParameter(getPID(io, tn, ID::level) + ":0")),
        (*apvt.getParameter(getPID(io, tn, ID::level) + ":1")),
        (*apvt.getParameter(getPID(io, tn, ID::level) + ":2")),
        (*apvt.getParameter(getPID(io, tn, ID::level) + ":3"))
    },
    pan(*apvt.getParameter(getPID(io, tn, ID::pan))),
    gain(*apvt.getParameter(getPID(io, tn, ID::gain))),
    mute(*apvt.getParameter(getPID(io, tn, ID::mute))),
    solo(*apvt.getParameter(getPID(io, tn, ID::solo))),
    cue(*apvt.getParameter(getPID(io, tn, ID::cue))),
    ac(*apvt.getParameter(getPID(io, tn, ID::ac))) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    {
        auto ts = std::make_unique<AudioProcessorParameterGroup>(ID::in, "Input", ID::separator);
        for (unsigned tn = 0; tn < IN_T_MAX; tn++) {
            auto t = std::make_unique<AudioProcessorParameterGroup>(String(tn), String(tn), ID::separator);
            auto g = std::make_unique<AudioProcessorParameterGroup>(ID::level, "Levels", ID::separator);
            String prefix = ts->getID() + ts->getSeparator() + String(tn) + ts->getSeparator();
            for (unsigned i = 0; i < TrackData::OUT_TRACKS; i++) {
                String lprefix = prefix + g->getID() + g->getSeparator();
                float lvl = i == 0 ? 1.0f : 0.0f;
                g->addChild(std::make_unique<ssp::BaseFloatParameter>(lprefix + String(i), String(i), 0.0, 4.0f, lvl));
            }
            t->addChild(std::move(g));
            t->addChild(std::make_unique<ssp::BaseFloatParameter>(prefix + ID::pan, "Pan", -1.0, 1.0f, 0.0f));
            t->addChild(std::make_unique<ssp::BaseFloatParameter>(prefix + ID::gain, "Gain", 0.0, 3.0f, 1.0f));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::mute, "Mute", false));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::solo, "Solo", false));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::cue, "Cue", false));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::ac, "AC", true));
            ts->addChild(std::move(t));
        }
        params.add(std::move(ts));
    }

    {
        auto ts = std::make_unique<AudioProcessorParameterGroup>(ID::out, "Output", ":");
        for (unsigned tn = 0; tn < OUT_T_MAX; tn++) {
            auto t = std::make_unique<AudioProcessorParameterGroup>(String(tn), String(tn), ":");
            auto g = std::make_unique<AudioProcessorParameterGroup>(ID::level, "Levels", ":");
            String prefix = ts->getID() + ts->getSeparator() + String(tn) + ts->getSeparator();
            for (unsigned i = 0; i < TrackData::OUT_TRACKS; i++) {
                String lprefix = prefix + g->getID() + g->getSeparator();
                float lvl = i == 0 ? 1.0f : 0.0f;
                g->addChild(std::make_unique<ssp::BaseFloatParameter>(lprefix + String(i), String(i), 0.0, 4.0f, lvl));
            }
            t->addChild(std::move(g));
            t->addChild(std::make_unique<ssp::BaseFloatParameter>(prefix + ID::pan, "Pan", -1.0, 1.0f, 0.0f));
            t->addChild(std::make_unique<ssp::BaseFloatParameter>(prefix + ID::gain, "Gain", 0.0, 3.0f, 1.0f));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::mute, "Mute", false));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::solo, "Solo", false));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::cue, "Cue", false));
            t->addChild(std::make_unique<ssp::BaseBoolParameter>(prefix + ID::ac, "AC", true));
            ts->addChild(std::move(t));
        }
        params.add(std::move(ts));
    }
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    if (channelIndex < I_MAX) return String("IN ") + String(channelIndex + 1);
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Main L",
        "Main R",
        "Cue L",
        "Cue R",
        "Aux 1 L",
        "Aux 1 R",
        "Aux 2 L",
        "Aux 2 R"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    inputBuffers_.setSize(I_MAX, samplesPerBlock);
    outputBuffers_.setSize(O_MAX, samplesPerBlock);

    // reset the RMS
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        auto &d = *inTracks_[ich];
        d.rms_.clear();
    }

    for (unsigned och = 0; och < O_MAX; och++) {
        auto &d = *outTracks_[och];
        d.rms_.clear();
    }
}

inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned n = buffer.getNumSamples();
    bool insoloed = false;
    bool outsoloed = false;

    for (unsigned ich = 0; ich < I_MAX; ich++) {
        insoloed |= bool(inTracks_[ich]->solo.getValue());
    }

    for (int och = 0; och < O_MAX; och++) {
        outsoloed |= bool(outTracks_[och]->solo.getValue());
        outputBuffers_.applyGain(och, 0, n, 0.0f);
    }

    for (unsigned ich = 0; ich < I_MAX; ich++) {
        bool inEnabled = inputEnabled[ich];
        if (!inEnabled) {
            auto &inTrack = *inTracks_[ich];
            // zero rms
            inTrack.rms_.process(0.0f);
            continue;
        }

        //process input channels
        auto &inTrack = *inTracks_[ich];
        unsigned inLeadCh = inTrack.dummy_ ? inTrack.follows_ : ich;
        auto &inLead = *inTracks_[inLeadCh];


        bool inMuted = inLead.mute.getValue() || (insoloed && !inLead.solo.getValue());
        float inMGain = normValue(inLead.gain);
        auto originbuf = buffer.getReadPointer(ich);
        inputBuffers_.copyFrom(ich, 0, originbuf, n, inMGain);

        if (inLead.ac.getValue()) {
            for (unsigned i = 0; i < buffer.getNumSamples(); i++) {
                float out = 0.0f;
                float in = inputBuffers_.getSample(ich, i);
                dcBlock(in, inTrack.dcX1_, out, inTrack.dcY1_);
                inputBuffers_.setSample(ich, i, out);
            }
        }
        auto inbuf = inputBuffers_.getReadPointer(ich);
        inTrack.rms_.process(inputBuffers_, ich);

        for (unsigned o = 0; o < TrackData::OUT_TRACKS; o++) {
            bool outEnabled = outputEnabled[o * 2] || outputEnabled[o * 2 + 1];

            if (!outEnabled) {
                // just skip
                continue;
            }

            bool masterCue =
                o > TrackData::CUE
                || (o == TrackData::CUE && inLead.cue.getValue())
                || (o == TrackData::MASTER && !inLead.cue.getValue());

            if (!inMuted && masterCue) {
                auto &outTL = *outTracks_[o * 2];
                float outGain = normValue(outTL.gain) * normValue(outTL.level[0]);
                float lOutGain = panGain(true, normValue(outTL.pan));
                float rOutGain = panGain(false, normValue(outTL.pan));

                float inGain = normValue(inLead.level[o]);
                float lInGain = panGain(true, normValue(inLead.pan));
                float rInGain = panGain(false, normValue(inLead.pan));

                float lGain = (inGain * outGain * lOutGain * lInGain);
                float rGain = (inGain * outGain * rOutGain * rInGain);

                outputBuffers_.addFrom(o * 2, 0, inbuf, n, lGain);
                outputBuffers_.addFrom((o * 2) + 1, 0, inbuf, n, rGain);
            }
        }

        // notes:
        // mute/solo is not applied until building outputs
        // as these are mono, pan only gets applied at output stage

        // if we start allowing stereo input, then we will add pan at this stage!
        // this probably means we assume all channels are stereo, and just duplicate mono inputs.
        // hmm: this probably is the way forward .. but need to think it thru,
        // since we need to take care for source of input
    }

    // calc output levels, and copy to vst buffer
    for (int och = 0; och < O_MAX; och++) {
        bool outEnabled = outputEnabled[och];
        if (!outEnabled) {
            // zero rms
            auto &trk = *outTracks_[och];
            trk.rms_.process(0.0f);
            // zero output
            buffer.applyGain(och, 0, n, 0.0f);
            continue;
        }

        auto &trk = *outTracks_[och];
        unsigned outLead = trk.dummy_ ? trk.follows_ : och;
        auto &ltrk = *outTracks_[outLead];
        bool outMuted = ltrk.mute.getValue() || (outsoloed && !ltrk.solo.getValue());

        trk.rms_.process(outputBuffers_, och);

        if (!outMuted) {
            auto buf = outputBuffers_.getReadPointer(och);
            buffer.copyFrom(och, 0, buf, n, 1.0);
        } else {
            buffer.applyGain(och, 0, n, 0.0f);
        }
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


void PluginProcessor::initTracks() {


    for (unsigned i = 0; i < IN_T_MAX; i++) {
        inTracks_.push_back(std::make_unique<TrackData>(vts(), ID::in, i));
    }
    for (unsigned i = 0; i < OUT_T_MAX; i++) {
        outTracks_.push_back(std::make_unique<TrackData>(vts(), ID::out, i));
    }

    //TODO , do I need to initialise here?
//    for (unsigned ich = 0; ich < I_MAX; ich++) {
//        auto &inTrack = *inTracks_[ich];
//        //default input tracks to have dc blocking
//        inTrack.ac.setValue(true);
//    }

    // outTracks_[0]// main
    outTracks_[1]->makeFollow(0);

    // outTracks_[2]// cue
    outTracks_[3]->makeFollow(2);

    // outTracks_[4]// aux 1
    outTracks_[5]->makeFollow(4);

    // outTracks_[6]// aux 2
    outTracks_[7]->makeFollow(6);
}

