
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
    // SSP currently sends control information as parameters
    // see Percussa.h for more info about the parameters below

    if (index < Percussa::sspLast) params_[index] = newValue;
    AudioProcessor::sendParamChangeMessageToListeners(index, newValue);
}

const String Pmix::getParameterName (int index)
{
    return String();
}

const String Pmix::getParameterText (int index)
{
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
    return false;
}

bool Pmix::isOutputChannelStereoPair (int index) const
{
    return false;
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
        d.rms_ = 0.0f;
        d.rmsHead_ = 0;
        d.rmsSum_ = 0.0f;
        for (unsigned i = 0; i < TrackData::MAX_RMS; i++) {
            d.rmsHistory_[i] = 0.0f;
        }
    }

    for (unsigned och = 0; och < O_MAX; och++) {
        auto& d = outTracks_[och];
        d.rms_ = 0.0f;
        d.rmsHead_ = 0;
        d.rmsSum_ = 0.0f;
        for (unsigned i = 0; i < TrackData::MAX_RMS; i++) {
            d.rmsHistory_[i] = 0.0f;
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
        return std::cosf(pan * PIdiv2);
    }
    return std::sinf(pan * PIdiv2);
}

void Pmix::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    unsigned n = buffer.getNumSamples();
    bool soloed = false;

    for (unsigned ich = 0; ich < I_MAX; ich++) {
        //process input channels
        unsigned intr = inTracks_[ich].dummy_ ? inTracks_[ich].follows_ : ich;

        soloed |= inTracks_[intr].solo_;
        float inGain = inTracks_[intr].gain_ + inTracks_[intr].level_[0]; // MASTER
        auto inbuf = buffer.getReadPointer(ich);
        inputBuffers_.copyFrom(ich, 0, inbuf, n, inGain);

        float inRMS = inputBuffers_.getRMSLevel(ich, 0, n);

        auto& trd = inTracks_[ich];
        trd.rmsSum_ -= trd.rmsHistory_[trd.rmsHead_];
        trd.rmsHistory_[trd.rmsHead_] = inRMS;
        trd.rmsSum_ += trd.rmsHistory_[trd.rmsHead_];
        trd.rmsHead_ = (trd.rmsHead_ + 1) % TrackData::MAX_RMS;
        trd.rms_ = trd.rmsSum_ / TrackData::MAX_RMS;

        // notes:
        // mute/solo is not applied until building outputs
        // as these are mono, pan only gets applied at output stage

        // if we start allowing stereo input, then we will add pan at this stage!
        // this probably means we assume all channels are stereo, and just duplicate mono inputs.
        // hmm: this probably is the way forward .. but need to think it thru,
        // since we need to take care for source of input
    }


    // outputs are currently all stereo, so we have to add panning
    // if were start allow mono aux channels this will change.
    for (unsigned och = 0; och < O_MAX / 2; och++) {
        unsigned outtr = och * 2;
        float lRMS = 0.0f;
        float rRMS = 0.0f;

        outputBuffers_.applyGain(och, 0, n, 0.0f);
        outputBuffers_.applyGain(och + 1, 0, n, 0.0f);
        // mute output channel
        if (outTracks_[outtr].mute_) {
            lRMS = 0.0f;
            rRMS = 0.0f;
        } else {
            float outGain = outTracks_[outtr].gain_ * outTracks_[outtr].level_[och];
            float lOutGain = panGain(true,  outTracks_[outtr].pan_);
            float rOutGain = panGain(false, outTracks_[outtr].pan_);

            // process each input channel
            for (unsigned ich = 0; ich < I_MAX; ich++) {

                unsigned intr = inTracks_[ich].dummy_ ? inTracks_[ich].follows_ : ich;
                bool muted = inTracks_[intr].mute_ ||   (soloed && ! inTracks_[intr].solo_);
                if (!muted) {
                    float inGain = outtr > 0 ? (float) inTracks_[intr].level_[outtr] : 1.0f; // master already applied
                    float lInGain = panGain(true, inTracks_[intr].pan_);
                    float rInGain = panGain(false, inTracks_[intr].pan_);

                    auto inbuf = inputBuffers_.getReadPointer(ich);
                    float lGain = (inGain * outGain * lOutGain * lInGain);
                    float rGain = (inGain * outGain * rOutGain * rInGain);

                    outputBuffers_.addFrom(outtr        , 0, inbuf, n, lGain);
                    outputBuffers_.addFrom(outtr + 1    , 0, inbuf, n, rGain);
                }
            }
            // all inputs handled.

            // note: at this stage, not allowing outputs to feed to other outputs!
            lRMS = outputBuffers_.getRMSLevel(outtr    , 0, n);
            rRMS = outputBuffers_.getRMSLevel(outtr + 1, 0, n);
        }

        for (unsigned ot = 0; ot < 2; ot++) {
            auto& trd = outTracks_[outtr + ot];
            trd.rmsSum_ -= trd.rmsHistory_[trd.rmsHead_];
            trd.rmsHistory_[trd.rmsHead_] = (ot == 0 ? lRMS : rRMS);
            trd.rmsSum_ += trd.rmsHistory_[trd.rmsHead_];
            trd.rmsHead_ = (trd.rmsHead_ + 1) % TrackData::MAX_RMS;
            trd.rms_ = trd.rmsSum_ / TrackData::MAX_RMS;
        }
    }

    for(int och=0;och<O_MAX;och++) {
        auto buf = outputBuffers_.getReadPointer(och);
        buffer.copyFrom(och, 0, buf, n, 1.0);
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



