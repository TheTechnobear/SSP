
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSP.h"


static const char *xmlTag = JucePlugin_Name;


Rngs::Rngs()
{
    memset(params_, 0, sizeof(params_));

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
    // Logger::writeToLog(getParameterName(index) + ":" + String(newValue));
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


const String Rngs::getParameterName (int index)
{
    if (index < Percussa::sspLast) return percussaParamsName[index];
    return String();
}

const String Rngs::getParameterText (int index)
{
    if (index < Percussa::sspLast) return percussaParamsText[index];
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
    return 1;
}

int Rngs::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return 0;
}

void Rngs::setCurrentProgram (int index)
{
}

const String Rngs::getProgramName (int index)
{
    return String("");
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

    bool stereoOut = params_[Percussa::sspOutEn1 + O_EVEN ] > 0.5f;

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
        float transpose = data_.f_pitch + RngsPitchOffset;
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

        performance_state.internal_exciter =  params_[Percussa::sspInEn1 + I_IN ] < 0.5f;
        performance_state.internal_strum =  params_[Percussa::sspInEn1 + I_STRUM ] < 0.5f;
        performance_state.internal_note =  params_[Percussa::sspInEn1 + I_VOCT ] < 0.5f;
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

        if (stereoOut) {
            for (int i = 0; i < RingsBlock; i++) {
                buffer.setSample(O_ODD, bidx + i, out[i]);
                buffer.setSample(O_EVEN, bidx + i, aux[i]);
            }
        } else {
            for (int i = 0; i < RingsBlock; i++) {
                buffer.setSample(O_ODD, bidx + i, (out[i] + aux[i] ) / 2.0f);
            }
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


void Rngs::writeToXml(XmlElement& xml) {
    xml.setAttribute("f_pitch",               double(data_.f_pitch));
    xml.setAttribute("f_structure",           double(data_.f_structure));
    xml.setAttribute("f_brightness",          double(data_.f_brightness));
    xml.setAttribute("f_damping",             double(data_.f_damping));
    xml.setAttribute("f_position",            double(data_.f_position));
    xml.setAttribute("f_polyphony",           double(data_.f_polyphony));
    xml.setAttribute("f_model",               double(data_.f_model));
    xml.setAttribute("f_bypass",              bool(data_.f_bypass));
    xml.setAttribute("f_easter_egg",          bool(data_.f_easter_egg));
    xml.setAttribute("f_in_gain",             double(data_.f_in_gain));
}

void Rngs::readFromXml(XmlElement& xml) {
    data_.f_pitch = xml.getDoubleAttribute("f_pitch", 0.0f);
    data_.f_structure = xml.getDoubleAttribute("f_structure", 0.45f);
    data_.f_brightness = xml.getDoubleAttribute("f_brightness", 0.5f);
    data_.f_damping = xml.getDoubleAttribute("f_damping", 0.5f);
    data_.f_position = xml.getDoubleAttribute("f_position", 0.5f);
    data_.f_polyphony = xml.getDoubleAttribute("f_polyphony", 0.0f);
    data_.f_model = xml.getDoubleAttribute("f_model", 0.0f);
    data_.f_bypass = xml.getBoolAttribute("f_bypass", 0.0f);
    data_.f_easter_egg = xml.getBoolAttribute("f_easter_egg", 0.0f);
    data_.f_in_gain = xml.getDoubleAttribute("f_in_gain", 0.0f);
    data_.f_trig = 0.0f;
}


void Rngs::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void Rngs::setStateInformation (const void* data, int sizeInBytes)
{
    XmlElement *pXML = getXmlFromBinary(data, sizeInBytes);
    if (pXML) {
        // auto root=pXML->getChildByName(xmlTag);
        // if(root) readFromXml(*root);
        readFromXml(*pXML);
        delete pXML;
    }


    // initialise again
    auto& part = data_.part;
    int polyphony = constrain(1 << int(data_.f_polyphony) , 1, rings::kMaxPolyphony);
    part.set_polyphony(polyphony);
    data_.string_synth.set_polyphony(polyphony);
    int imodel = constrain(data_.f_model, 0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
    part.set_model(model);
    data_.string_synth.set_fx(static_cast<rings::FxType>(model));
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Rngs();
}



