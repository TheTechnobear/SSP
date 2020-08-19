
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"

#include <cmath>

static constexpr unsigned NUM_PROGRAM_SLOTS = 20;

#ifdef __APPLE__
static const char*  presetProgramDir = "~/SSP/plugin.presets/pmix";
#else
static const char*  presetProgramDir = "/media/linaro/SYNTHOR/plugins.presets/pmix";
#endif


static const char *xmlTag = "PMIX";
static const char *prefType = ".xml";


const char* percussaParamsName [] = {
    "sspEnc1",
    "sspEnc2",
    "sspEnc3",
    "sspEnc4",
    "sspEncSw1",
    "sspEncSw2",
    "sspEncSw3",
    "sspEncSw4",
    "sspSw1",
    "sspSw2",
    "sspSw3",
    "sspSw4",
    "sspSw5",
    "sspSw6",
    "sspSw7",
    "sspSw8",
    "sspSwLeft",
    "sspSwRight",
    "sspSwUp",
    "sspSwDown",
    "sspSwShiftL",
    "sspSwShiftR"
};

const char* percussaParamsText [] = {
    "Encoder 1",
    "Encoder 2",
    "Encoder 3",
    "Encoder 4",
    "Encoder 1 Sw",
    "Encoder 2 Sw",
    "Encoder 3 Sw",
    "Encoder 4 Sw",
    "Button 1",
    "Button 2",
    "Button 3",
    "Button 4",
    "Button 5",
    "Button 6",
    "Button 7",
    "Button 8",
    "Button Left",
    "Button Right",
    "Button Up",
    "Button Down",
    "Button Shift L",
    "Button Shift R"
};


Pmix::Pmix()
{
    memset(params_, 0, sizeof(params_));
    initTracks();

    File f(presetProgramDir);
    if (!f.isDirectory()) {
        if (f.exists()) {
            Logger::writeToLog("Unable to create plugin.presets directory for plugin");
        } else {
            f.createDirectory();
        }
    }

    for (int i = 0; i < NUM_PROGRAM_SLOTS; i++) {
        String fn(String(presetProgramDir) + File::separatorString + String::formatted("%03.0f", float(i)) + String(prefType));
        File f(fn);
        if (!f.exists()) {
            f.create();
            currentProgram_ = i;
            writePreset();
        }
        currentProgram_ = -1;
    }
}

Pmix::~Pmix()
{
    ;
}

const String Pmix::getName() const
{
    return JucePlugin_Name;
}

int Pmix::getNumParameters()
{
    return Percussa::sspLast;
}

float Pmix::getParameter (int index)
{
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Pmix::setParameter (int index, float newValue)
{
    // this will have to change... as the +/-1 is larger than before
    // current idea is to move away from sendParamChangeMessageToListeners
    // to a differ 'changebroadcaster' to free up parameter change for 'proper use'
    switch (index) {
    case Percussa::sspEnc1:
    case Percussa::sspEnc2:
    case Percussa::sspEnc3:
    case Percussa::sspEnc4:
    {
        if (newValue > 0.5) {
            // TODO - check shoudl paramValues really hold actual value?
            params_[index - Percussa::sspFirst]++;
            AudioProcessor::sendParamChangeMessageToListeners(index, 1.0f);
        } else if (newValue < 0.5) {
            params_[index - Percussa::sspFirst]--;
            AudioProcessor::sendParamChangeMessageToListeners(index, -1.0f);
        } else {
            AudioProcessor::sendParamChangeMessageToListeners(index, 0.0f);
        }
        break;
    }
    default: {
        if (index < Percussa::sspLast) params_[index] = newValue;
        AudioProcessor::sendParamChangeMessageToListeners(index, newValue);
        break;
    }
    }
}

const String Pmix::getParameterName (int index)
{
    if (index < Percussa::sspLast) return percussaParamsName[index];
    return String();
}

const String Pmix::getParameterText (int index)
{
    if (index < Percussa::sspLast) return percussaParamsText[index];
    return String();
}


const String Pmix::getInputChannelName (int channelIndex) const
{
    return String("IN ") + String(channelIndex + 1);
}

const String Pmix::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_MAIN_L:        { return String("Out L");}
    case O_MAIN_R:        { return String("Out R");}
    case O_CUE_L:         { return String("Cue L");}
    case O_CUE_R:         { return String("Cue R");}
    case O_AUX_1_L:       { return String("Aux 1 L");}
    case O_AUX_1_R:       { return String("Aux 1 R");}
    case O_AUX_2_L:       { return String("Aux 2 L");}
    case O_AUX_2_R:       { return String("Aux 2 R");}
    }
    return String("Uknown:") + String (channelIndex + 1);
}

bool Pmix::isInputChannelStereoPair (int index) const
{
    return index <= I_IN_8;
}

bool Pmix::isOutputChannelStereoPair (int index) const
{
    return index <= O_AUX_2_R;
}

bool Pmix::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Pmix::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Pmix::silenceInProducesSilenceOut() const
{
    return false;
}

double Pmix::getTailLengthSeconds() const
{
    return 0.0;
}

int Pmix::getNumPrograms()
{
    unsigned c = 0;
    DirectoryIterator di(File(presetProgramDir), false, "*" + String(prefType));
    while (di.next()) c++;

    // NB: some hosts don't cope very well if you tell them there are 0 programs
    if (c == 0) c = 1;

    return c;
}

int Pmix::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return currentProgram_;
}

void Pmix::setCurrentProgram (int index)
{
    // SSP calls when program being loaded
    if (currentProgram_ != index) {
        currentProgram_ = index;
        readPreset();
    }
}

const String Pmix::getProgramName (int index)
{
    bool valid = false;
    String  fn = fileFromIdx(index, valid);
    return fn;
}

void Pmix::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void Pmix::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    inputBuffers_.setSize(I_MAX, samplesPerBlock);
    outputBuffers_.setSize(O_MAX, samplesPerBlock);

    // reset the RMS
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        auto& d = inTracks_[ich];
        d.lvl_ = 0.0f;
        d.lvlHead_ = 0;
        d.lvlSum_ = 0.0f;
        for (unsigned i = 0; i < TrackData::MAX_RMS; i++) {
            d.lvlHistory_[i] = 0.0f;
        }
    }

    for (unsigned och = 0; och < O_MAX; och++) {
        auto& d = outTracks_[och];
        d.lvl_ = 0.0f;
        d.lvlHead_ = 0;
        d.lvlSum_ = 0.0f;
        for (unsigned i = 0; i < TrackData::MAX_RMS; i++) {
            d.lvlHistory_[i] = 0.0f;
        }
    }
}

void Pmix::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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
float panGain(bool left, float p) {
    static constexpr float PIdiv2 = M_PI / 2.0f;
    float pan = (p + 1.0f) / 2.0f;
    if (left) {
        return cosf(pan * PIdiv2);
        //return std::cosf(pan * PIdiv2);
    }
    return sinf(pan * PIdiv2);
    //return std::sinf(pan * PIdiv2);
}

void Pmix::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
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
        //process input channels
        auto& inTrack = inTracks_[ich];
        unsigned inLeadCh = inTrack.dummy_ ? inTrack.follows_ : ich;
        auto& inLead = inTracks_[inLeadCh];


        bool    inMuted = inLead.mute_ ||   (insoloed && ! inLead.solo_);
        float   inMGain = inLead.gain_;
        auto    originbuf = buffer.getReadPointer(ich);
        inputBuffers_.copyFrom(ich, 0, originbuf, n, inMGain);

        // TODO: Q. perhaps input buffer are just for RMS?
        // issue is I only want to do the dcblock once!


        if (inLead.ac_) {
            for (unsigned i = 0; i < buffer.getNumSamples(); i++) {
                float out = 0.0f;
                float in = inputBuffers_.getSample(ich, i);
                dcBlock(in, inTrack.dcX1_, out, inTrack.dcY1_);
                inputBuffers_.setSample(ich, i, out);
            }
        }
        auto    inbuf = inputBuffers_.getReadPointer(ich);

        float inlvl = inTrack.useRMS_
                      ? inputBuffers_.getRMSLevel(ich, 0, n)
                      : inputBuffers_.getMagnitude(ich, 0, n);

        inTrack.lvlSum_ -= inTrack.lvlHistory_[inTrack.lvlHead_];
        inTrack.lvlHistory_[inTrack.lvlHead_] = inlvl;
        inTrack.lvlSum_ += inTrack.lvlHistory_[inTrack.lvlHead_];
        inTrack.lvlHead_ = (inTrack.lvlHead_ + 1) % TrackData::MAX_RMS;
        inTrack.lvl_ = inTrack.lvlSum_ / TrackData::MAX_RMS;


        for (unsigned o = 0; o < TrackData::OUT_TRACKS; o++) {
            bool masterCue = 
                    o > TrackData::CUE
                ||  (o == TrackData::CUE && inLead.cue_)
                ||  (o == TrackData::MASTER && !inLead.cue_);
            if (!inMuted && masterCue) {
                auto&  outTL = outTracks_[o * 2];
                float outGain = outTL.gain_ * outTL.level_[0];
                float lOutGain = panGain(true,   outTL.pan_);
                float rOutGain = panGain(false,  outTL.pan_);

                float inGain = inLead.level_[o];
                float lInGain = panGain(true,   inLead.pan_);
                float rInGain = panGain(false,  inLead.pan_);

                float lGain = (inGain * outGain * lOutGain * lInGain);
                float rGain = (inGain * outGain * rOutGain * rInGain);

                outputBuffers_.addFrom( o * 2     , 0, inbuf, n, lGain);
                outputBuffers_.addFrom((o * 2) + 1 , 0, inbuf, n, rGain);
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

        auto& trk = outTracks_[och];
        unsigned outLead = trk.dummy_ ? trk.follows_ : och;
        auto& ltrk = outTracks_[outLead];
        bool  outMuted = ltrk.mute_ ||   (outsoloed && ! ltrk.solo_);

        float lvl = trk.useRMS_
                    ? outputBuffers_.getRMSLevel(och, 0, n)
                    : outputBuffers_.getMagnitude(och, 0, n);


        trk.lvlSum_ -= trk.lvlHistory_[trk.lvlHead_];
        trk.lvlHistory_[trk.lvlHead_] = lvl;
        trk.lvlSum_ += trk.lvlHistory_[trk.lvlHead_];
        trk.lvlHead_ = (trk.lvlHead_ + 1) % TrackData::MAX_RMS;
        trk.lvl_ = trk.lvlSum_ / TrackData::MAX_RMS;

        if (!outMuted) {
            auto buf = outputBuffers_.getReadPointer(och);
            buffer.copyFrom(och, 0, buf, n, 1.0);
        } else {
            buffer.applyGain(och, 0, n, 0.0f);
        }
    }
}

bool Pmix::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Pmix::createEditor()
{
    return new PmixEditor (*this);
}

void Pmix::writePreset() {
    if (currentProgram_ < 0) return;


    bool valid = false;
    String  fn = String(presetProgramDir) + File::separatorString + fileFromIdx(currentProgram_, valid);
    Logger::writeToLog("Writing: " + String(currentProgram_) + " : " + fn);
    File f(fn);

    if (!valid) {
        Logger::writeToLog("Unable to write preset, not found, will create : " + String(currentProgram_));
        f.create();
    }


    FileOutputStream fileStream(f);
    fileStream.setPosition(0);
    fileStream.truncate();

    XmlElement xml(xmlTag);
    writeToXml(xml);
    xml.writeToStream(fileStream, String(""));

    // DynamicObject::Ptr v (new DynamicObject());
    // writeToJson(v);
    // var jsonVar(v.get());
    // JSON::writeToStream(fileStream, jsonVar);
    fileStream.flush();
}


void Pmix::readPreset() {
    if (currentProgram_ < 0) return;

    bool valid = false;
    String  fn = String(presetProgramDir) + File::separatorString + fileFromIdx(currentProgram_, valid);

    File f(fn);
    Logger::writeToLog("Reading: " + String(currentProgram_) + " : " + fn);

    if (!valid || !f.exists()) {
        Logger::writeToLog("Unable to read preset, file !exist : " + String(currentProgram_) + " : " + fn);
    }

    XmlDocument xmlFile(f);
    auto pXML = xmlFile.getDocumentElement();
    if (pXML) {
        readFromXml(*pXML);
        delete pXML;
    }
    // juce::var jsonVar = JSON::parse(f);
    // if (jsonVar == var::null) {
    //     Logger::writeToLog("Unable to read preset, unable to parse : " + String(currentProgram_) + " : " + fn);
    //     return;
    // }

    // if (!jsonVar.isObject()) {
    //     Logger::writeToLog("Unable to read preset, badly format : " + String(currentProgram_) + " : " + fn);
    //     return;
    // }
    // readFromJson(jsonVar);
}

void Pmix::writeToJson(DynamicObject::Ptr& v) {
    // v->setProperty("useRMS",          float(data_.useRMS_));
}

void Pmix::readFromJson(juce::var& jsonVar) {
    // data_.useRMS_      = (bool) jsonVar.getProperty("useRMS"    , 0.0f);
}

void Pmix::writeTrackXml(TrackData& t, juce::XmlElement& xml) {
    for (int lt = 0; lt < TrackData::OUT_TRACKS; lt++) {
        xml.setAttribute("level" + String(lt), (double) t.level_[lt]);
    }
    xml.setAttribute("pan",  (double) t.pan_);
    xml.setAttribute("gain", (double) t.gain_);
    xml.setAttribute("mute", (bool) t.mute_);
    xml.setAttribute("solo", (bool) t.solo_);
    xml.setAttribute("cue",  (bool) t.cue_);
    xml.setAttribute("ac",   (bool) t.ac_);
    //dummy_
    //follows_
}

void Pmix::writeToXml(XmlElement& xml) {
    auto inxml = xml.createNewChildElement("Input");
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        //process input channels
        auto& inTrack = inTracks_[ich];
        auto txml = inxml->createNewChildElement("T" + String(ich));
        if (txml) writeTrackXml(inTrack, *txml);
    }

    auto outxml = xml.createNewChildElement("Output");
    for (unsigned och = 0; och < O_MAX; och++) {
        //process input channels
        auto& outTrack = outTracks_[och];
        auto txml = outxml->createNewChildElement("T" + String(och));
        if (txml) writeTrackXml(outTrack, *txml);
    }
}

void  Pmix::readTrackXml(TrackData& t, juce::XmlElement& xml) {
    for (int lt = 0; lt < TrackData::OUT_TRACKS; lt++) {
        t.level_[lt] = xml.getDoubleAttribute("level" + String(lt), t.level_[lt]);
    }

    t.pan_  = xml.getDoubleAttribute("pan", t.pan_);
    t.gain_ = xml.getDoubleAttribute("gain", t.gain_);
    t.mute_ = xml.getBoolAttribute("mute", t.mute_);
    t.solo_ = xml.getBoolAttribute("solo", t.solo_);
    t.cue_  = xml.getBoolAttribute("cue", t.cue_);
    t.ac_   = xml.getBoolAttribute("ac", t.ac_);
    //dummy_
    //follows_
}

void Pmix::readFromXml(XmlElement& xml) {
    auto inxml = xml.getChildByName("Input");
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        //process input channels
        auto& inTrack = inTracks_[ich];
        auto txml = inxml->getChildByName("T" + String(ich));
        if (txml) readTrackXml(inTrack, *txml);
        else  Logger::writeToLog("failed to read T" + String(ich));
    }

    auto outxml = xml.getChildByName("Output");
    for (unsigned och = 0; och < O_MAX; och++) {
        //process input channels
        auto& outTrack = outTracks_[och];
        auto txml = outxml->getChildByName("T" + String(och));
        if (txml) readTrackXml(outTrack, *txml);
    }
}



void Pmix::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);

    // DynamicObject::Ptr v (new DynamicObject());
    // writeToJson(v);
    // var jsonVar(v.get());
    // String str = JSON::toString(jsonVar, true);
    // destData.append(str.toRawUTF8( ), str.getNumBytesAsUTF8( ) + 1);
}

void Pmix::setStateInformation (const void* data, int sizeInBytes)
{
    XmlElement *pXML = getXmlFromBinary(data, sizeInBytes);
    if (pXML) {
        // auto root=pXML->getChildByName(xmlTag);
        // if(root) readFromXml(*root);
        readFromXml(*pXML);
        delete pXML;
    }
    // const char* str = static_cast<const char*>(data);
    // auto jsonVar = JSON::parse(String::fromUTF8(str));
    // readFromJson(jsonVar);
}


String Pmix::fileFromIdx(int idx, bool& found) {
    StringArray files;
    DirectoryIterator di(File(presetProgramDir), false, "*" + String(prefType));
    while (di.next()) {
        files.add(di.getFile().getFileName());
    }
    files.sort(false);
    if (idx > files.size()) {
        found = false;
        return String::formatted("%03.0f", float(idx)) + String(prefType);
    }
    found = true;
    return files[idx];
}


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pmix();
}


void Pmix::initTracks() {
    for (unsigned ich = 0; ich < I_MAX; ich++) {
        auto& inTrack = inTracks_[ich];
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



