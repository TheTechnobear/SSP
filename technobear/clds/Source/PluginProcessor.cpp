
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"


static constexpr unsigned NUM_PROGRAM_SLOTS = 20;

#ifdef __APPLE__
static const char*  presetProgramDir = "~/SSP/plugin.presets/clds";
#else
static const char*  presetProgramDir = "/media/linaro/SYNTHOR/plugins.presets/clds";
#endif

inline float TO_SHORTFRAME(float v)   { return constrain(v * 32767.0f, -32768.0f, 32767.0f);}
inline float FROM_SHORTFRAME(short v) { return (float(v) / 32768.0f); }

Clds::Clds()
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

Clds::~Clds()
{
    ;
}

const String Clds::getName() const
{
    return JucePlugin_Name;
}

int Clds::getNumParameters()
{
    return Percussa::sspLast;
}

float Clds::getParameter (int index)
{
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Clds::setParameter (int index, float newValue)
{
    // SSP currently sends control information as parameters
    // see Percussa.h for more info about the parameters below

    if (index < Percussa::sspLast) params_[index] = newValue;
    AudioProcessor::sendParamChangeMessageToListeners(index, newValue);
}

const String Clds::getParameterName (int index)
{
    return String();
}

const String Clds::getParameterText (int index)
{
    return String();
}

const String Clds::getInputChannelName (int channelIndex) const
{
    switch (channelIndex) {
    case I_LEFT:        { return String("In L");}
    case I_RIGHT:       { return String("In R");}
    case I_TRIG:        { return String("Trig");}
    case I_VOCT:        { return String("VOct");}
    case I_POS:         { return String("Pos");}
    case I_SIZE:        { return String("Size");}
    case I_DENSITY:     { return String("Density");}
    case I_TEXT:        { return String("Texture");}
    }
    return String("Uknown:") + String (channelIndex + 1);
}

const String Clds::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_LEFT : {
        return String("Out L");
    }
    case O_RIGHT : {
        return String("Out R");
    }
    }
    return String("Uknown:") + String (channelIndex + 1);
}

bool Clds::isInputChannelStereoPair (int index) const
{
    return false;
}

bool Clds::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool Clds::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Clds::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Clds::silenceInProducesSilenceOut() const
{
    return false;
}

double Clds::getTailLengthSeconds() const
{
    return 0.0;
}

int Clds::getNumPrograms()
{
    unsigned c = 0;
    DirectoryIterator di(File(presetProgramDir), false, "*.json");
    while (di.next()) c++;

    // NB: some hosts don't cope very well if you tell them there are 0 programs
    if (c == 0) c = 1;

    return c;
}

int Clds::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return currentProgram_;
}

void Clds::setCurrentProgram (int index)
{
    // SSP calls when program being loaded
    if (currentProgram_ != index) {
        currentProgram_ = index;
        readFromJson();
    }
}

const String Clds::getProgramName (int index)
{
    bool valid = false;
    String  fn = fileFromIdx(index, valid);
    return fn;
}

void Clds::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void Clds::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // this is called by the SSP's software right after loading
    // the plugin and before it starts calling processBlock below
}

void Clds::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Clds::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    auto& ibuf = data_.ibuf;
    auto& obuf = data_.obuf;
    auto& processor = data_.processor;
    auto& data = data_;

    auto n = CloudsBlock;

    // note: clouds is run at 32khz, we are running at 48khz!
    // Clouds usually has a blocks size of 16,(?)
    // SSP = 128 (@48k), so split up, so we read the control rate date every 16
    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {

        bool trig = false;
        float gain = (data.f_in_gain * 4.0f);
        float in_gain = constrain(1.0f + (gain * gain), 1.0f, 17.0f);

        for (int i = 0; i < n; i++) {

            ibuf[i].l = TO_SHORTFRAME(buffer.getSample(I_LEFT, bidx + i) * in_gain);
            ibuf[i].r = TO_SHORTFRAME(buffer.getSample(I_RIGHT, bidx + i) * in_gain);

            if (buffer.getSample(I_TRIG, bidx + i) > 0.5) {
                trig = true;
            }
        }

        // control rate
        clouds::PlaybackMode mode  = (clouds::PlaybackMode) ( int(data.f_mode) % clouds::PLAYBACK_MODE_LAST);

        processor.set_playback_mode(mode);
        processor.set_silence(false);
        processor.set_bypass(false);
        processor.set_num_channels(2);
        processor.set_low_fidelity(false);
        processor.Prepare();

        // processor.set_quality();
        // processor.set_bypass(data.f_bypass > 0.5f);
        // processor.set_silence(data.f_silence > 0.5f);
        // processor.set_num_channels(f_mono  < 0.5f ? 1 : 2 );
        // processor.set_low_fidelity(f_lofi > 0.5f);


        auto p = processor.mutable_parameters();

        float pitch     = data.f_pitch + cv2Pitch(buffer.getSample(I_VOCT, bidx));
        pitch = pitch + 1.50f; // SSP adapt?!
        float position  = data.f_position + buffer.getSample(I_POS, bidx);
        float size      = data.f_size + buffer.getSample(I_SIZE, bidx);
        float density   = ((data.f_density + buffer.getSample(I_DENSITY, bidx)) + 1.0f ) / 2.0f;
        float texture   = data.f_texture + buffer.getSample(I_TEXT, bidx);


        //restrict density to .2 to .8 for granular mode, outside this breaks up
        // density = constrain(density, 0.0f, 1.0f);
        // density = (mode == clouds::PLAYBACK_MODE_GRANULAR) ? (density * 0.6f) + 0.2f : density;

        p->freeze = (data.f_freeze > 0.5f) ||  (data.trig_or_freeze && trig) ;

        p->gate        = (!data.trig_or_freeze) && trig;
        p->trigger     = p->gate;

        p->pitch       = constrain(pitch,      -48.0f, 48.0f);
        p->position    = constrain(position,   0.0f, 1.0f);
        p->size        = constrain(size,       0.0f, 1.0f);
        p->texture     = constrain(texture,    0.0f, 1.0f);
        p->density     = constrain(density,    0.0f, 1.0f);

        p->dry_wet     = constrain(data.f_mix,           0.0f, 1.0f);
        p->stereo_spread = constrain(data.f_spread,      0.0f, 1.0f);
        p->feedback    = constrain(data.f_feedback,      0.0f, 1.0f);
        p->reverb      = constrain(data.f_reverb,        0.0f, 1.0f);

        processor.Process(ibuf, obuf, n);

        for (int i = 0; i < CloudsBlock; i++) {
            buffer.setSample(O_LEFT, bidx + i, FROM_SHORTFRAME(obuf[i].l));
            buffer.setSample(O_RIGHT, bidx + i, FROM_SHORTFRAME(obuf[i].r));
        }
    }
}

bool Clds::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Clds::createEditor()
{
    return new CldsEditor (*this);
}

void Clds::write() {
    writeToJson();
}

void Clds::writeToJson() {
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
    v->setProperty("f_freeze",          float(data_.f_freeze));
    v->setProperty("f_position",        float(data_.f_position));
    v->setProperty("f_size",            float(data_.f_size));
    v->setProperty("f_pitch",           float(data_.f_pitch));
    v->setProperty("f_density",         float(data_.f_density));
    v->setProperty("f_texture",         float(data_.f_texture));
    v->setProperty("f_mix",             float(data_.f_mix));
    v->setProperty("f_spread",          float(data_.f_spread));
    v->setProperty("f_feedback",        float(data_.f_feedback));
    v->setProperty("f_reverb",          float(data_.f_reverb));
    v->setProperty("f_mode",            float(data_.f_mode));
    v->setProperty("f_in_gain",         float(data_.f_in_gain));
    v->setProperty("trig_or_freeze",    float(data_.trig_or_freeze));

    v->setProperty("f_mono",            float(data_.f_mono));
    v->setProperty("f_lofi",            float(data_.f_lofi));

    FileOutputStream fileStream(f);
    fileStream.setPosition(0);
    fileStream.truncate();

    var jsonVar(v.get());
    JSON::writeToStream(fileStream, jsonVar);
    fileStream.flush();
}

void Clds::readFromJson() {
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

    data_.f_freeze      = jsonVar.getProperty("f_freeze"    , 0.0f);
    data_.f_position    = jsonVar.getProperty("f_position"  , 0.5f);
    data_.f_size        = jsonVar.getProperty("f_size"      , 0.5f);
    data_.f_pitch       = jsonVar.getProperty("f_pitch"     , 0.0f);
    data_.f_density     = jsonVar.getProperty("f_density"   , -0.2f);
    data_.f_mix         = jsonVar.getProperty("f_mix"       , 0.5f);
    data_.f_spread      = jsonVar.getProperty("f_spread"    , 0.5f);
    data_.f_feedback    = jsonVar.getProperty("f_feedback"  , 0.1f);
    data_.f_reverb      = jsonVar.getProperty("f_reverb"    , 0.5f);
    data_.f_mode        = jsonVar.getProperty("f_mode"      , 0.0f);
    data_.f_in_gain     = jsonVar.getProperty("f_in_gain"   , 0.0f);
    data_.trig_or_freeze = jsonVar.getProperty("trig_or_freeze" , 0.0f);

    data_.f_mono        = jsonVar.getProperty("f_mono"      , 0.0f);
    data_.f_lofi        = jsonVar.getProperty("f_lofi"      , 0.0f);
}

void Clds::getStateInformation (MemoryBlock& destData)
{
    // store state information

    // SSP not currently using - untested
    DynamicObject::Ptr v (new DynamicObject());
    v->setProperty("f_freeze",          float(data_.f_freeze));
    v->setProperty("f_position",        float(data_.f_position));
    v->setProperty("f_size",            float(data_.f_size));
    v->setProperty("f_pitch",           float(data_.f_pitch));
    v->setProperty("f_density",         float(data_.f_density));
    v->setProperty("f_texture",         float(data_.f_texture));
    v->setProperty("f_mix",             float(data_.f_mix));
    v->setProperty("f_spread",          float(data_.f_spread));
    v->setProperty("f_feedback",        float(data_.f_feedback));
    v->setProperty("f_reverb",          float(data_.f_reverb));
    v->setProperty("f_mode",            float(data_.f_mode));
    v->setProperty("f_in_gain",         float(data_.f_in_gain));
    v->setProperty("trig_or_freeze",    float(data_.trig_or_freeze));

    v->setProperty("f_mono",            float(data_.f_mono));
    v->setProperty("f_lofi",            float(data_.f_lofi));

    var jsonVar(v.get());
    String str = JSON::toString(jsonVar, true);
    destData.append(str.toRawUTF8( ), str.getNumBytesAsUTF8( ) + 1);
}

void Clds::setStateInformation (const void* data, int sizeInBytes)
{
    // recall state information - created by getStateInformation
    // SSP not currently using - untested

    const char* str = static_cast<const char*>(data);
    auto jsonVar = JSON::parse(String::fromUTF8(str));

    data_.f_freeze      = jsonVar.getProperty("f_freeze"    , 0.0f);
    data_.f_position    = jsonVar.getProperty("f_position"  , 0.5f);
    data_.f_size        = jsonVar.getProperty("f_size"      , 0.5f);
    data_.f_pitch       = jsonVar.getProperty("f_pitch"     , 0.0f);
    data_.f_density     = jsonVar.getProperty("f_density"   , -0.2f);
    data_.f_mix         = jsonVar.getProperty("f_mix"       , 0.5f);
    data_.f_spread      = jsonVar.getProperty("f_spread"    , 0.5f);
    data_.f_feedback    = jsonVar.getProperty("f_feedback"  , 0.1f);
    data_.f_reverb      = jsonVar.getProperty("f_reverb"    , 0.5f);
    data_.f_mode        = jsonVar.getProperty("f_mode"      , 0.0f);
    data_.f_in_gain     = jsonVar.getProperty("f_in_gain"   , 0.0f);
    data_.trig_or_freeze = jsonVar.getProperty("trig_or_freeze" , 0.0f);

    data_.f_mono        = jsonVar.getProperty("f_mono"      , 0.0f);
    data_.f_lofi        = jsonVar.getProperty("f_lofi"      , 0.0f);

}


String Clds::fileFromIdx(int idx, bool& found) {
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
    return new Clds();
}



