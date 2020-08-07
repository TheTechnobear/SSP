
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
        String fn(String(presetProgramDir) + File::separatorString + String::formatted("%03.0f", float(i))  + String(".json"));
        File f(fn);
        if (!f.exists()) {
            f.create();
            currentProgram_ = i;
            writeToJson();
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
    // Logger::writeToLog("setParameter: : " + getParameterName(index) + " ->  " + String(newValue));
    // SSP currently sends control information as parameters
    // see Percussa.h for more info about the parameters below

    if (index < Percussa::sspLast) params_[index] = newValue;
    AudioProcessor::sendParamChangeMessageToListeners(index, newValue);
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
    case O_AUX_1_L:       { return String("Aux 1 L");}
    case O_AUX_1_R:       { return String("Aux 1 R");}
    case O_AUX_2_L:       { return String("Aux 2 L");}
    case O_AUX_2_R:       { return String("Aux 2 R");}
    case O_AUX_3_L:       { return String("Aux 3 L");}
    case O_AUX_3_R:       { return String("Aux 3 R");}
    }
    return String("Uknown:") + String (channelIndex + 1);
}

bool Pmix::isInputChannelStereoPair (int index) const
{
    return index <= I_IN_8;
}

bool Pmix::isOutputChannelStereoPair (int index) const
{
    return index <= O_AUX_3_R;
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
    DirectoryIterator di(File(presetProgramDir), false, "*.json");
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
        readFromJson();
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
        float   inMGain = inLead.gain_ + inLead.level_[0]; // MASTER
        auto    inbuf = buffer.getReadPointer(ich);
        inputBuffers_.copyFrom(ich, 0, inbuf, n, inMGain);

        float inlvl = inTrack.useRMS_
                      ? inputBuffers_.getRMSLevel(ich, 0, n)
                      : inputBuffers_.getMagnitude(ich, 0, n);

        inTrack.lvlSum_ -= inTrack.lvlHistory_[inTrack.lvlHead_];
        inTrack.lvlHistory_[inTrack.lvlHead_] = inlvl;
        inTrack.lvlSum_ += inTrack.lvlHistory_[inTrack.lvlHead_];
        inTrack.lvlHead_ = (inTrack.lvlHead_ + 1) % TrackData::MAX_RMS;
        inTrack.lvl_ = inTrack.lvlSum_ / TrackData::MAX_RMS;


        for (unsigned o = 0; o < TrackData::OUT_TRACKS; o++) {
            if (!inMuted) {
                auto&  outTL = outTracks_[o * 2];
                float outGain = outTL.gain_ + outTL.level_[0];
                float lOutGain = panGain(true,   outTL.pan_);
                float rOutGain = panGain(false,  outTL.pan_);

                float inGain = o > 0 ? (float) inLead.level_[o] : 1.0f; // master already applied
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

        if(!outMuted) {
            auto buf = outputBuffers_.getReadPointer(och);
            buffer.copyFrom(och, 0, buf, n, 1.0);
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

void Pmix::write() {
    writeToJson();
}

void Pmix::writeToJson() {
    if (currentProgram_ < 0) return;


    bool valid = false;
    String  fn = String(presetProgramDir) + File::separatorString + fileFromIdx(currentProgram_, valid);
    Logger::writeToLog("Writing: " + String(currentProgram_) + " : " + fn);
    File f(fn);

    if (!valid) {
        Logger::writeToLog("Unable to write preset, not found, will create : " + String(currentProgram_));
        f.create();
    }

    DynamicObject::Ptr v (new DynamicObject());
    // v->setProperty("f_freeze",          float(data_.f_freeze));

    FileOutputStream fileStream(f);
    fileStream.setPosition(0);
    fileStream.truncate();

    var jsonVar(v.get());
    JSON::writeToStream(fileStream, jsonVar);
    fileStream.flush();
}

void Pmix::readFromJson() {
    if (currentProgram_ < 0) return;

    bool valid = false;
    String  fn = String(presetProgramDir) + File::separatorString + fileFromIdx(currentProgram_, valid);

    File f(fn);
    Logger::writeToLog("Reading: " + String(currentProgram_) + " : " + fn);

    if (!valid || !f.exists()) {
        Logger::writeToLog("Unable to read preset, file !exist : " + String(currentProgram_) + " : " + fn);
    }


    auto jsonVar = JSON::parse(f);
    if (jsonVar == var::null) {
        Logger::writeToLog("Unable to read preset, unable to parse : " + String(currentProgram_) + " : " + fn);
        return;
    }

    if (!jsonVar.isObject()) {
        Logger::writeToLog("Unable to read preset, badly format : " + String(currentProgram_) + " : " + fn);
        return;
    }

    // data_.f_freeze      = jsonVar.getProperty("f_freeze"    , 0.0f);
}

void Pmix::getStateInformation (MemoryBlock& destData)
{
    // store state information

    // SSP not currently using - untested
    DynamicObject::Ptr v (new DynamicObject());
    // v->setProperty("f_freeze",          float(data_.f_freeze));

    var jsonVar(v.get());
    String str = JSON::toString(jsonVar, true);
    destData.append(str.toRawUTF8( ), str.getNumBytesAsUTF8( ) + 1);
}

void Pmix::setStateInformation (const void* data, int sizeInBytes)
{
    // recall state information - created by getStateInformation
    // SSP not currently using - untested

    const char* str = static_cast<const char*>(data);
    auto jsonVar = JSON::parse(String::fromUTF8(str));

    // data_.f_freeze      = jsonVar.getProperty("f_freeze"    , 0.0f);
}


String Pmix::fileFromIdx(int idx, bool& found) {
    StringArray files;
    DirectoryIterator di(File(presetProgramDir), false, "*.json");
    while (di.next()) {
        files.add(di.getFile().getFileName());
    }
    files.sort(false);
    if (idx > files.size()) {
        found = false;
        return String::formatted("%03.0f", float(idx)) + ".json";
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
    // outTracks_[0]// main master
    outTracks_[1].makeFollow(0);
    // outTracks_[2]// aux1
    outTracks_[3].makeFollow(1);
    // outTracks_[3]// aux1
    outTracks_[4].makeFollow(3);
    // outTracks_[5]// aux1
    outTracks_[5].makeFollow(5);
    // outTracks_[6]// aux1
    outTracks_[7].makeFollow(6);
}



