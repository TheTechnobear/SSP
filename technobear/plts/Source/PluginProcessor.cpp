
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSP.h"


static const char *xmlTag = JucePlugin_Name;


Plts::Plts()
{
    memset(params_, 0, sizeof(params_));
}

Plts::~Plts()
{
    ;
}

const String Plts::getName() const
{
    return JucePlugin_Name;
}

int Plts::getNumParameters()
{
    return Percussa::sspLast;
}

float Plts::getParameter (int index)
{
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Plts::setParameter (int index, float newValue)
{
    // this will have to change... as the +/-1 is larger than before
    // current idea is to move away from sendParamChangeMessageToListeners
    // to a differ 'changebroadcaster' to free up parameter change for 'proper use'
    //Logger::writeToLog(getParameterName(index) + ":" + String(newValue));
    switch (index) {
    case Percussa::sspEnc1:
    case Percussa::sspEnc2:
    case Percussa::sspEnc3:
    case Percussa::sspEnc4:
    {
        if (newValue > 0.5f) {
            params_[index - Percussa::sspFirst]++;
            AudioProcessor::sendParamChangeMessageToListeners(index, 1.0f);
        } else if (newValue < 0.5f) {
            params_[index - Percussa::sspFirst]--;
            AudioProcessor::sendParamChangeMessageToListeners(index, -1.0f);
        } else {
            params_[index] = 0.0f;
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


const String Plts::getParameterName (int index)
{
    if (index < Percussa::sspLast) return percussaParamsName[index];
    return String();
}

const String Plts::getParameterText (int index)
{
    if (index < Percussa::sspLast) return percussaParamsText[index];
    return String();
}


const String Plts::getInputChannelName (int channelIndex) const
{
    switch (channelIndex) {
    case I_VOCT:        { return String("VOct");}
    case I_TRIG:        { return String("Trig");}
    case I_LEVEL:       { return String("Level");}
    case I_HARMONICS:   { return String("Harm");}
    case I_TIMBRE:      { return String("Timbre");}
    case I_MORPH:       { return String("Morph");}
    case I_FM:          { return String("FM");}
    case I_MODEL:       { return String("Model");}
    }
    return String("unused:") + String (channelIndex + 1);
}

const String Plts::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_OUT : {
        return String("Out");
    }
    case O_AUX : {
        return String("Aux");
    }
    }
    return String("unused:") + String (channelIndex + 1);
}

bool Plts::isInputChannelStereoPair (int index) const
{
    return false;
}

bool Plts::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool Plts::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Plts::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Plts::silenceInProducesSilenceOut() const
{
    return false;
}

double Plts::getTailLengthSeconds() const
{
    return 0.0;
}

int Plts::getNumPrograms()
{
    return 1;
}

int Plts::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return 0;
}

void Plts::setCurrentProgram (int index)
{
}

const String Plts::getProgramName (int index)
{
    return String("");
}

void Plts::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void Plts::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // this is called by the SSP's software right after loading
    // the plugin and before it starts calling processBlock below
    auto &voice=data_.voice;
    stmlib::BufferAllocator allocator(data_.shared_buffer, sizeof(data_.shared_buffer));
    voice.Init(&allocator);
}

void Plts::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



void Plts::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    auto n = PltsBlock;
    auto &voice=data_.voice;
    auto &patch=data_.patch;

    bool auxOut = params_[Percussa::sspOutEn1 + O_AUX ] > 0.5f;

    // bool voctEn  = params_[Percussa::sspInEn1 + I_VOCT ] > 0.5f;
    bool fmEn  = params_[Percussa::sspInEn1 + I_FM ] > 0.5f;
    bool timbreEn  = params_[Percussa::sspInEn1 + I_TIMBRE ] > 0.5f;
    bool morphEn  = params_[Percussa::sspInEn1 + I_MORPH ] > 0.5f;
    bool trigEn  = params_[Percussa::sspInEn1 + I_TRIG ] > 0.5f;
    bool levelEn  = params_[Percussa::sspInEn1 + I_LEVEL ] > 0.5f;

    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {

        bool trig = false;
        for (int i = 0; i < n; i++) {
            bool t = buffer.getSample(I_TRIG, bidx + i) > 0.5;
            if (t != data_.trig_ && t) {
                trig = true;
            }
            data_.trig_ = t;
        }


        //todo constrain

        static constexpr float PltsPitchOffset = 60.0f - 3.044f;
        float pitch = data_.pitch_ + PltsPitchOffset;

        patch.engine = constrain(data_.model_,0.0f,15.0f); //check

        patch.note = pitch;
        //patch.note = 60.f + pitch * 12.f;
        patch.harmonics = data_.harmonics_ ;
        patch.timbre = data_.timbre_;
        patch.morph = data_.morph_;
        patch.lpg_colour = data_.lpg_colour_;
        patch.decay = data_.decay_;

        patch.frequency_modulation_amount = data_.freq_mod_;
        patch.timbre_modulation_amount = data_.timbre_mod_ ;
        patch.morph_modulation_amount = data_.morph_mod_;

        // Construct modulations
        plaits::Modulations modulations;
        modulations.engine = buffer.getSample(I_MODEL, bidx) / 5.f;
        modulations.note = cv2Pitch(buffer.getSample(I_VOCT, bidx));
        modulations.frequency = cv2Pitch(buffer.getSample(I_FM, bidx));
        modulations.harmonics = buffer.getSample(I_HARMONICS, bidx) / 5.f;
        modulations.timbre = buffer.getSample(I_TIMBRE, bidx) / 8.f;
        modulations.morph = buffer.getSample(I_MORPH, bidx) / 8.f;

        modulations.trigger = data_.trig_;
        modulations.level = buffer.getSample(I_LEVEL, bidx) / 8.f;

        // modulations.frequency_patched = voctEn;
        modulations.frequency_patched = fmEn;
        modulations.timbre_patched = timbreEn;
        modulations.morph_patched = morphEn;
        modulations.trigger_patched = trigEn;
        modulations.level_patched = levelEn;

        // Render frames
        plaits::Voice::Frame output[PltsBlock];
        voice.Render(patch, modulations, output, PltsBlock);

        if (auxOut) {
            for (int i = 0; i < PltsBlock; i++) {
                buffer.setSample(O_OUT, bidx + i, output[i].out / 32768.f);
                buffer.setSample(O_AUX, bidx + i, output[i].aux / 32768.f);
            }
        } else {
            for (int i = 0; i < PltsBlock; i++) {
                buffer.setSample(O_OUT, bidx + i, output[i].out / 32768.f);
            }
            buffer.clear(O_AUX,0,n);
        }
    }
}

bool Plts::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Plts::createEditor()
{
    return new PltsEditor (*this);
}


void Plts::writeToXml(XmlElement& xml) {
    xml.setAttribute("pitch",       double(data_.pitch_));
    xml.setAttribute("harmonics",   double(data_.harmonics_));
    xml.setAttribute("timbre",      double(data_.timbre_));
    xml.setAttribute("morph",       double(data_.morph_));

    xml.setAttribute("freq_mod",    double(data_.freq_mod_));
    xml.setAttribute("timbre_mod",  double(data_.timbre_mod_));
    xml.setAttribute("morph_mod",    double(data_.morph_mod_));



    xml.setAttribute("model",       double(data_.model_));
    xml.setAttribute("lpgcolour",   double(data_.lpg_colour_));
    xml.setAttribute("decay",       double(data_.decay_));
}


void Plts::readFromXml(XmlElement& xml) {
    data_.pitch_ = xml.getDoubleAttribute("pitch", 0.0f);
    data_.harmonics_ = xml.getDoubleAttribute("harmonics", 0.0f);
    data_.timbre_ = xml.getDoubleAttribute("timbre", 0.5f);
    data_.morph_ = xml.getDoubleAttribute("morph", 0.5f);


    data_.freq_mod_ = xml.getDoubleAttribute("freq_mod", 0.0f);
    data_.timbre_mod_ = xml.getDoubleAttribute("timbre_mod", 0.0f);
    data_.morph_mod_ = xml.getDoubleAttribute("morph_mod", 0.0f);

    data_.model_ = xml.getDoubleAttribute("model", 0.0f);
    data_.lpg_colour_ = xml.getDoubleAttribute("lpgcolour", 0.5f);
    data_.decay_ = xml.getDoubleAttribute("decay", 0.5f);
}


void Plts::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void Plts::setStateInformation (const void* data, int sizeInBytes)
{
    XmlElement *pXML = getXmlFromBinary(data, sizeInBytes);
    if (pXML) {
        // auto root=pXML->getChildByName(xmlTag);
        // if(root) readFromXml(*root);
        readFromXml(*pXML);
        delete pXML;
    }
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plts();
}



