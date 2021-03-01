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
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
    initTracks();
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    pitch(*apvt.getParameter(ID::pitch)),
    vca(*apvt.getParameter(ID::vca)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -30.0f, +30.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca, "VCA", 0.0f, 100.0f, 50.0f));
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
        auto &d = inTracks_[ich];
        d.rms_.clear();
    }

    for (unsigned och = 0; och < O_MAX; och++) {
        auto &d = outTracks_[och];
        d.rms_.clear();
    }
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned n = buffer.getNumSamples();
    bool insoloed = false;
    bool outsoloed = false;

    for (unsigned ich = 0; ich < I_MAX; ich++) {
        insoloed |= inTracks_[ich].solo_;
    }

    for (int och = 0; och < O_MAX; och++) {
        outsoloed |= outTracks_[och].solo_;
        outputBuffers_.applyGain(och, 0, n, 0.0f);
    }

    for (unsigned ich = 0; ich < I_MAX; ich++) {
        bool inEnabled = inputEnabled[ich];
        if (!inEnabled) {
            auto &inTrack = inTracks_[ich];
            // zero rms
            inTrack.rms_.process(0.0f);
            continue;
        }

        //process input channels
        auto &inTrack = inTracks_[ich];
        unsigned inLeadCh = inTrack.dummy_ ? inTrack.follows_ : ich;
        auto &inLead = inTracks_[inLeadCh];


        bool inMuted = inLead.mute_ || (insoloed && !inLead.solo_);
        float inMGain = inLead.gain_;
        auto originbuf = buffer.getReadPointer(ich);
        inputBuffers_.copyFrom(ich, 0, originbuf, n, inMGain);

        if (inLead.ac_) {
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
                || (o == TrackData::CUE && inLead.cue_)
                || (o == TrackData::MASTER && !inLead.cue_);

            if (!inMuted && masterCue) {
                auto &outTL = outTracks_[o * 2];
                float outGain = outTL.gain_ * outTL.level_[0];
                float lOutGain = panGain(true, outTL.pan_);
                float rOutGain = panGain(false, outTL.pan_);

                float inGain = inLead.level_[o];
                float lInGain = panGain(true, inLead.pan_);
                float rInGain = panGain(false, inLead.pan_);

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
            auto &trk = outTracks_[och];
            trk.rms_.process(0.0f);
            // zero output
            buffer.applyGain(och, 0, n, 0.0f);
            continue;
        }

        auto &trk = outTracks_[och];
        unsigned outLead = trk.dummy_ ? trk.follows_ : och;
        auto &ltrk = outTracks_[outLead];
        bool outMuted = ltrk.mute_ || (outsoloed && !ltrk.solo_);

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
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        auto &inTrack = inTracks_[ich];
        //default input tracks to have dc blocking
        inTrack.ac_ = true;
    }

    // outTracks_[0]// main
    outTracks_[1].makeFollow(0);

    // outTracks_[2]// cue
    outTracks_[3].makeFollow(2);

    // outTracks_[4]// aux 1
    outTracks_[5].makeFollow(4);

    // outTracks_[6]// aux 2
    outTracks_[7].makeFollow(6);
}


void PluginProcessor::writeTrackXml(TrackData &t, juce::XmlElement &xml) {
    for (int lt = 0; lt < TrackData::OUT_TRACKS; lt++) {
        xml.setAttribute("level" + String(lt), (double) t.level_[lt]);
    }
    xml.setAttribute("pan", (double) t.pan_);
    xml.setAttribute("gain", (double) t.gain_);
    xml.setAttribute("mute", (bool) t.mute_);
    xml.setAttribute("solo", (bool) t.solo_);
    xml.setAttribute("cue", (bool) t.cue_);
    xml.setAttribute("ac", (bool) t.ac_);
    //dummy_
    //follows_
}

void PluginProcessor::writeToXml(XmlElement &xml) {
    auto inxml = xml.createNewChildElement("Input");
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        //process input channels
        auto &inTrack = inTracks_[ich];
        auto txml = inxml->createNewChildElement("T" + String(ich));
        if (txml) writeTrackXml(inTrack, *txml);
    }

    auto outxml = xml.createNewChildElement("Output");
    for (unsigned och = 0; och < O_MAX; och++) {
        //process input channels
        auto &outTrack = outTracks_[och];
        auto txml = outxml->createNewChildElement("T" + String(och));
        if (txml) writeTrackXml(outTrack, *txml);
    }
}

void PluginProcessor::readTrackXml(TrackData &t, juce::XmlElement &xml) {
    for (int lt = 0; lt < TrackData::OUT_TRACKS; lt++) {
        t.level_[lt] = xml.getDoubleAttribute("level" + String(lt), t.level_[lt]);
    }

    t.pan_ = xml.getDoubleAttribute("pan", t.pan_);
    t.gain_ = xml.getDoubleAttribute("gain", t.gain_);
    t.mute_ = xml.getBoolAttribute("mute", t.mute_);
    t.solo_ = xml.getBoolAttribute("solo", t.solo_);
    t.cue_ = xml.getBoolAttribute("cue", t.cue_);
    t.ac_ = xml.getBoolAttribute("ac", t.ac_);
    //dummy_
    //follows_
}

void PluginProcessor::readFromXml(XmlElement &xml) {
    auto inxml = xml.getChildByName("Input");
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        //process input channels
        auto &inTrack = inTracks_[ich];
        auto txml = inxml->getChildByName("T" + String(ich));
        if (txml) readTrackXml(inTrack, *txml);
        else Logger::writeToLog("failed to read T" + String(ich));
    }

    auto outxml = xml.getChildByName("Output");
    for (unsigned och = 0; och < O_MAX; och++) {
        //process input channels
        auto &outTrack = outTracks_[och];
        auto txml = outxml->getChildByName("T" + String(och));
        if (txml) readTrackXml(outTrack, *txml);
    }
}


void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    static const char *xmlTag = JucePlugin_Name;
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    auto pXML = getXmlFromBinary(data, sizeInBytes);
    if (pXML) {
        // auto root=pXML->getChildByName(xmlTag);
        // if(root) readFromXml(*root);
        readFromXml(*pXML);
    }
}



