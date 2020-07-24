
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Percussa.h"


static constexpr unsigned NUM_PROGRAM_SLOTS=20;

#ifdef __APPLE__
    static const char*  presetProgramDir="~/SSP/plugin.presets/rngs";
#else 
    static const char*  presetProgramDir="/media/linaro/SYNTHOR/plugins.presets/rngs";
#endif


Rngs::Rngs()
{
    memset(params_, 0, sizeof(params_));
    File f(presetProgramDir);
    if(!f.isDirectory()) {
        if(f.exists()) {
            Logger::writeToLog("Unable to create plugin.presets directory for plugin");
        } else {
            f.createDirectory();
        }
    }

    for(int i=0;i<NUM_PROGRAM_SLOTS;i++) {
        String fn(String(presetProgramDir) + File::separatorString + String::formatted("%03.0f",float(i))  + String(".json"));
        File f(fn);
        if(!f.exists()) {
            f.create();
            currentProgram_=i;
            writeToJson();
        }
        currentProgram_=-1;
    }

    // setProgram gets called before prepare to play
    // without this part is in an 'unknown' state
    data_.strummer.Init(0.01f, 48000 / RingsBlock);
    data_.string_synth.Init(data_.buffer);
    data_.part.Init(data_.buffer);
    auto& part = data_.part;
    int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);
    part.set_polyphony(polyphony);
    data_.string_synth.set_polyphony(polyphony);
    int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
    part.set_model(model);
    data_.string_synth.set_fx(static_cast<rings::FxType>(model));
}

Rngs::~Rngs()
{
    ;
}

const String Rngs::getName() const
{
    return JucePlugin_Name;
}

int Rngs::getNumParameters()
{
    return Percussa::sspLast;
}

float Rngs::getParameter (int index)
{
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Rngs::setParameter (int index, float newValue)
{
    // this will have to change... as the +/-1 is larger than before
    // current idea is to move away from sendParamChangeMessageToListeners
    // to a differ 'changebroadcaster' to free up parameter change for 'proper use'
    switch(index) {
        case Percussa::sspEnc1:
        case Percussa::sspEnc2: 
        case Percussa::sspEnc3:
        case Percussa::sspEnc4: 
        {
            if (newValue > 0.5) {  
                // TODO - check shoudl paramValues really hold actual value?
                params_[index-Percussa::sspFirst]++; 
                AudioProcessor::sendParamChangeMessageToListeners(index, 1.0f);
            } else if (newValue < 0.5) { 
                params_[index-Percussa::sspFirst]--; 
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

const String Rngs::getParameterName (int index)
{
    return String();
}

const String Rngs::getParameterText (int index)
{
    return String();
}

const String Rngs::getInputChannelName (int channelIndex) const
{
    switch (channelIndex) {
    case I_IN:          { return String("In");}
    case I_STRUM:       { return String("Strum");}
    case I_VOCT:        { return String("VOct");}
    case I_FM:          { return String("FM");}
    case I_STUCTURE:    { return String("Struct");}
    case I_BRIGHTNESS:  { return String("Bright");}
    case I_DAMPING:      { return String("Damp");}
    case I_POSITION:      { return String("Pos");}
    }
    return String("unused:") + String (channelIndex + 1);
}

const String Rngs::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_ODD : {
        return String("Odd");
    }
    case O_EVEN : {
        return String("Even");
    }
    }
    return String("unused:") + String (channelIndex + 1);
}

bool Rngs::isInputChannelStereoPair (int index) const
{
    return false;
}

bool Rngs::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool Rngs::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Rngs::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Rngs::silenceInProducesSilenceOut() const
{
    return false;
}

double Rngs::getTailLengthSeconds() const
{
    return 0.0;
}

int Rngs::getNumPrograms()
{
    unsigned c=0;
    DirectoryIterator di(File(presetProgramDir),false, "*.json");
    while(di.next()) c++;

    // NB: some hosts don't cope very well if you tell them there are 0 programs
    if(c==0) c=1;

    return c;
}

int Rngs::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return currentProgram_;
}

void Rngs::setCurrentProgram (int index)
{
    // SSP calls when program being loaded
    if(currentProgram_!=index) {
        currentProgram_ = index;
        readFromJson();
    }
}

const String Rngs::getProgramName (int index)
{
    bool valid=false;
    String  fn=fileFromIdx(index, valid);
    return fn;
}

void Rngs::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void Rngs::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // this is called by the SSP's software right after loading
    // the plugin and before it starts calling processBlock below


    data_.strummer.Init(0.01f, sampleRate / RingsBlock);
    data_.string_synth.Init(data_.buffer);
    data_.part.Init(data_.buffer);

    auto& part = data_.part;
    int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);
    part.set_polyphony(polyphony);
    data_.string_synth.set_polyphony(polyphony);
    int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
    part.set_model(model);
    data_.string_synth.set_fx(static_cast<rings::FxType>(model));
}

void Rngs::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



void Rngs::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    auto& in = data_.in;
    auto& out = data_.out;
    auto& aux = data_.aux;
    auto n = RingsBlock;
    size_t size = n;


    // Rings usually has a blocks size of 16,
    // SSP = 128 (@48k), so split up, so we read the control rate date every 16
    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {


        float gain = (data_.f_in_gain * 4.0f);
        float in_gain = constrain(1.0f + (gain * gain), 1.0f, 17.0f);

        bool strum = false;
        for (int i = 0; i < n; i++) {
            in[i] = out[i] = buffer.getSample(I_IN, bidx + i) * in_gain;
            bool trig = buffer.getSample(I_STRUM, bidx + i) > 0.5;
            if (trig != data_.f_trig && trig) {
                strum = true;
            }
            data_.f_trig = trig;
        }

        // control rate
        float transpose = data_.f_pitch;
        float note = cv2Pitch(buffer.getSample(I_VOCT, bidx));
        float fm = cv2Pitch(buffer.getSample(I_FM, bidx));
        float damping = data_.f_damping + buffer.getSample(I_DAMPING, bidx);
        float structure = data_.f_structure + buffer.getSample(I_STUCTURE, bidx);
        float brightness = data_.f_brightness + buffer.getSample(I_BRIGHTNESS, bidx);
        float position = data_.f_position + buffer.getSample(I_POSITION, bidx);
        float chord = data_.patch.structure * rings::kNumChords - 1;

        auto& patch = data_.patch;
        auto& part = data_.part;
        auto& performance_state = data_.performance_state;

        patch.brightness = constrain(brightness, 0.0f, 1.0f);
        patch.damping = constrain(damping, 0.0f, 1.0f);
        patch.position = constrain(position, 0.0f, 1.0f);
        patch.structure = constrain(structure, 0.0f, 0.9995f);

        performance_state.fm = constrain(fm, -48.0f, 48.0f);
        performance_state.note = note;
        performance_state.tonic = 12.0f + transpose;

        //TODO test only
        data_.f_internal_exciter =  params_[Percussa::sspInEn1 + I_IN ] < 0.5;
        data_.f_internal_strum =  params_[Percussa::sspInEn1 + I_STRUM ] < 0.5;
        data_.f_internal_note =  params_[Percussa::sspInEn1 + I_VOCT ] < 0.5;

        performance_state.internal_exciter =  params_[Percussa::sspInEn1 + I_IN ] < 0.5;
        performance_state.internal_strum =  params_[Percussa::sspInEn1 + I_STRUM ] < 0.5;
        performance_state.internal_note =  params_[Percussa::sspInEn1 + I_VOCT ] < 0.5;
        performance_state.chord = constrain(chord , 0, rings::kNumChords - 1);;

        if (!performance_state.internal_note) {
            // performance_state.note = 0.0;
            // quantize if using v/oct input
            performance_state.tonic = 12.0f + roundf(transpose);
        }

        performance_state.strum = strum;

        int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);

        if (polyphony != part.polyphony()) {
            part.set_polyphony(polyphony);
            data_.string_synth.set_polyphony(polyphony);
        }

        int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
        rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
        if (model != part.model()) {
            part.set_model(model);
            data_.string_synth.set_fx(static_cast<rings::FxType>(model));
        }


        part.set_bypass(data_.f_bypass > 0.5);

        if (data_.f_easter_egg > 0.5) {
            data_.strummer.Process(NULL, size, &(data_.performance_state));
            data_.string_synth.Process(data_.performance_state, data_.patch, in, out, aux, size);
        } else {
            // Apply noise gate.
            for (size_t i = 0; i < size; ++i) {
                float in_sample = in[i];
                float error, gain;
                error = in_sample * in_sample - data_.in_level;
                data_.in_level += error * (error > 0.0f ? 0.1f : 0.0001f);
                gain = data_.in_level <= data_.kNoiseGateThreshold
                       ? (1.0f / data_.kNoiseGateThreshold) * data_.in_level : 1.0f;
                in[i] = gain * in_sample;
            }
            data_.strummer.Process(in, size, &(data_.performance_state));
            part.Process(performance_state, patch, in, out, aux, size);
        }

        for (int i = 0; i < RingsBlock; i++) {
            buffer.setSample(O_ODD, bidx + i, out[i]);
            buffer.setSample(O_EVEN, bidx + i, aux[i]);
        }

    }
}

bool Rngs::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Rngs::createEditor()
{
    return new RngsEditor (*this);
}

void Rngs::write() {
    writeToJson();
}

void Rngs::writeToJson() {
    if(currentProgram_<0) return;


    bool valid=false;
    String  fn=String(presetProgramDir) + File::separatorString + fileFromIdx(currentProgram_, valid);
    Logger::writeToLog("Writing: " + String(currentProgram_) + " : " + fn);
    File f(fn);

    if(!valid) {
        Logger::writeToLog("Unable to write preset, not found, will create : " + String(currentProgram_));
        f.create();
    } 


    DynamicObject::Ptr v (new DynamicObject());
    v->setProperty("f_pitch",               float(data_.f_pitch));
    v->setProperty("f_structure",           float(data_.f_structure));
    v->setProperty("f_brightness",          float(data_.f_brightness));
    v->setProperty("f_damping",             float(data_.f_damping));
    v->setProperty("f_position",            float(data_.f_position));
    v->setProperty("f_polyphony",           float(data_.f_polyphony));
    v->setProperty("f_model",               float(data_.f_model));
    v->setProperty("f_bypass",              float(data_.f_bypass));
    v->setProperty("f_easter_egg",          float(data_.f_easter_egg));
    // v->setProperty("f_internal_strum",      float(data_.f_internal_strum));
    // v->setProperty("f_internal_exciter",    float(data_.f_internal_exciter));
    // v->setProperty("f_internal_note",       float(data_.f_internal_note));
    v->setProperty("f_in_gain",             float(data_.f_in_gain));


    FileOutputStream fileStream(f);
    fileStream.setPosition(0);
    fileStream.truncate();

    var jsonVar(v.get());
    JSON::writeToStream(fileStream,jsonVar);
    fileStream.flush();
}

void Rngs::readFromJson() {
    if(currentProgram_<0) return;

    bool valid=false;
    String  fn=String(presetProgramDir) + File::separatorString + fileFromIdx(currentProgram_, valid);

    File f(fn);
    Logger::writeToLog("Reading: " + String(currentProgram_) + " : " + fn);

    if(!valid || !f.exists()) {
        Logger::writeToLog("Unable to read preset, file !exist : " + String(currentProgram_) + " : " + fn);
    }


    auto jsonVar = JSON::parse(f);
    if(jsonVar==var::null) {
        Logger::writeToLog("Unable to read preset, unable to parse : " + String(currentProgram_) + " : " + fn);
        return;
    }

    if(!jsonVar.isObject()) {
        Logger::writeToLog("Unable to read preset, badly format : " + String(currentProgram_) + " : " + fn);
        return;
    }

    data_.f_pitch = jsonVar.getProperty("f_pitch",34.0f);
    data_.f_structure = jsonVar.getProperty("f_structure",0.45f);
    data_.f_brightness = jsonVar.getProperty("f_brightness",0.5f);
    data_.f_damping = jsonVar.getProperty("f_damping",0.5f);
    data_.f_position = jsonVar.getProperty("f_position",0.5f);
    data_.f_polyphony = jsonVar.getProperty("f_polyphony",0.0f);
    data_.f_model = jsonVar.getProperty("f_model",0.0f);
    data_.f_bypass = jsonVar.getProperty("f_bypass",0.0f);
    data_.f_easter_egg = jsonVar.getProperty("f_easter_egg",0.0f);
    // data_.f_internal_strum = jsonVar.getProperty("f_internal_strum",1.0f);
    // data_.f_internal_exciter = jsonVar.getProperty("f_internal_exciter",1.0f);
    // data_.f_internal_note = jsonVar.getProperty("f_internal_note",0.0f);
    data_.f_in_gain = jsonVar.getProperty("f_in_gain",0.0f);
    data_.f_trig=0.0f;

    // now initialialise with new data
    auto& part = data_.part;
    int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);
    part.set_polyphony(polyphony);
    data_.string_synth.set_polyphony(polyphony);
    int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
    part.set_model(model);
    data_.string_synth.set_fx(static_cast<rings::FxType>(model));
}

void Rngs::getStateInformation (MemoryBlock& destData)
{
    // store state information
    DynamicObject::Ptr v (new DynamicObject());
    v->setProperty("f_pitch",               float(data_.f_pitch));
    v->setProperty("f_structure",           float(data_.f_structure));
    v->setProperty("f_brightness",          float(data_.f_brightness));
    v->setProperty("f_damping",             float(data_.f_damping));
    v->setProperty("f_position",            float(data_.f_position));
    v->setProperty("f_polyphony",           float(data_.f_polyphony));
    v->setProperty("f_model",               float(data_.f_model));
    v->setProperty("f_bypass",              float(data_.f_bypass));
    v->setProperty("f_easter_egg",          float(data_.f_easter_egg));
    // v->setProperty("f_internal_strum",      float(data_.f_internal_strum));
    // v->setProperty("f_internal_exciter",    float(data_.f_internal_exciter));
    // v->setProperty("f_internal_note",       float(data_.f_internal_note));
    v->setProperty("f_in_gain",             float(data_.f_in_gain));


    var jsonVar(v.get());
    String str=JSON::toString(jsonVar,true);
    destData.append(str.toRawUTF8( ), str.getNumBytesAsUTF8( ) + 1);
}

void Rngs::setStateInformation (const void* data, int sizeInBytes)
{
    // recall state information - created by getStateInformation
    const char* str=static_cast<const char*>(data);
    auto jsonVar = JSON::parse(String::fromUTF8(str));

    data_.f_pitch = jsonVar.getProperty("f_pitch",34.0f);
    data_.f_structure = jsonVar.getProperty("f_structure",0.45f);
    data_.f_brightness = jsonVar.getProperty("f_brightness",0.5f);
    data_.f_damping = jsonVar.getProperty("f_damping",0.5f);
    data_.f_position = jsonVar.getProperty("f_position",0.5f);
    data_.f_polyphony = jsonVar.getProperty("f_polyphony",0.0f);
    data_.f_model = jsonVar.getProperty("f_model",0.0f);
    data_.f_bypass = jsonVar.getProperty("f_bypass",0.0f);
    data_.f_easter_egg = jsonVar.getProperty("f_easter_egg",0.0f);
    // data_.f_internal_strum = jsonVar.getProperty("f_internal_strum",1.0f);
    // data_.f_internal_exciter = jsonVar.getProperty("f_internal_exciter",1.0f);
    // data_.f_internal_note = jsonVar.getProperty("f_internal_note",0.0f);
    data_.f_in_gain = jsonVar.getProperty("f_in_gain",0.0f);

    data_.f_trig=0.0f;


    auto& part = data_.part;
    int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);
    part.set_polyphony(polyphony);
    data_.string_synth.set_polyphony(polyphony);
    int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
    part.set_model(model);
    data_.string_synth.set_fx(static_cast<rings::FxType>(model));
}

String Rngs::fileFromIdx(int idx, bool& found) {
    StringArray files; 
    DirectoryIterator di(File(presetProgramDir),false, "*.json");
    while(di.next()) {
        files.add(di.getFile().getFileName());
    }
    files.sort(false);
    if(idx > files.size()) {
        found = false;
        return String::formatted("%03.0f", float(idx)) + ".json";
    }
    found=true;
    return files[idx];
}


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Rngs();
}



