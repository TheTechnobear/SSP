
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"


static constexpr unsigned NUM_PROGRAM_SLOTS = 20;

#ifdef __APPLE__
static const char*  presetProgramDir = "~/SSP/plugin.presets/pmix";
#else
static const char*  presetProgramDir = "/media/linaro/SYNTHOR/plugins.presets/pmix";
#endif

Pmix::Pmix()
{
    memset(params_, 0, sizeof(params_));
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
    switch (channelIndex) {
    case I_LEFT:        { return String("In L");}
    case I_RIGHT:       { return String("In R");}
    }
    return String("Uknown:") + String (channelIndex + 1);
}

const String Pmix::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_LEFT:        { return String("Out L");}
    case O_RIGHT:       { return String("Out R");}
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // this is called by the SSP's software right after loading
    // the plugin and before it starts calling processBlock below
}

void Pmix::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Pmix::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

    // float pitch     = data.f_pitch + cv2Pitch(buffer.getSample(I_VOCT, 0));


    // for (int bidx = 0; bidx < buffer.getNumSamples(); bidx++) {
    //     float l = buffer.getSample(I_LEFT, bidx);
    //     float r = buffer.getSample(I_RIGHT, bidx);
    // }

    // process

    // for (int bidx = 0; bidx < buffer.getNumSamples(); bidx++) {
    //     buffer.setSample(I_LEFT, l);
    //     buffer.setSample(I_RIGHT, r);
    // }
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



