
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSP.h"


inline float TO_SHORTFRAME(float v)   { return constrain(v * 32767.0f, -32768.0f, 32767.0f);}
inline float FROM_SHORTFRAME(short v) { return (float(v) / 32768.0f); }


static const char *xmlTag = JucePlugin_Name;

Clds::Clds()
{
    memset(params_, 0, sizeof(params_));

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
    // this will have to change... as the +/-1 is larger than before
    // current idea is to move away from sendParamChangeMessageToListeners
    // to a differ 'changebroadcaster' to free up parameter change for 'proper use'
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

const String Clds::getParameterName (int index)
{
    if (index < Percussa::sspLast) return percussaParamsName[index];
    return String();
}

const String Clds::getParameterText (int index)
{
    if (index < Percussa::sspLast) return percussaParamsText[index];
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
    case I_FREEZE:      { return String("Freeze");}
    case I_MIX:         { return String("Mix");}
    case I_SPREAD:      { return String("Spread");}
    case I_FEEDBACK:    { return String("Feedback");}
    case I_REVERB:      { return String("Reverb");}
    }
    return String("unused:") + String (channelIndex + 1);
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
    return String("unused:") + String (channelIndex + 1);
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
    return 1;
}

int Clds::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return 0;
}

void Clds::setCurrentProgram (int index)
{
}

const String Clds::getProgramName (int index)
{
    return String("");
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
    auto& data = data_;

    if(data.processor==nullptr) {
        data.processor = new clouds::GranularProcessor;
        auto& processor = *data_.processor;

        data.ibuf = new clouds::ShortFrame[data.iobufsz];
        data.obuf = new clouds::ShortFrame[data.iobufsz];
        data.block_mem = new uint8_t[data.block_mem_sz*2];
        data.block_ccm = new uint8_t[data.block_ccm_sz*2];
        memset(data.block_mem, 0, sizeof(uint8_t)*data.block_mem_sz*2);
        memset(data.block_ccm, 0, sizeof(uint8_t)*data.block_ccm_sz*2);
        memset(processor.mutable_parameters(), 0, sizeof(clouds::Parameters));
        processor.Init(
            data.block_mem, data.block_mem_sz,
            data.block_ccm, data.block_ccm_sz);

    }


    auto& ibuf = data_.ibuf;
    auto& obuf = data_.obuf;
    auto& processor = *data_.processor;

    auto n = CloudsBlock;

    bool stereoIn = params_[Percussa::sspInEn1 + I_RIGHT ] > 0.5f;
    bool stereoOut = params_[Percussa::sspOutEn1 + O_RIGHT ] > 0.5f;

    // note: clouds is run at 32khz, we are running at 48khz!
    // Clouds usually has a blocks size of 16,(?)
    // SSP = 128 (@48k), so split up, so we read the control rate date every 16
    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {

        bool trig = false;
        float gain = (data.f_in_gain * 4.0f);
        float in_gain = constrain(1.0f + (gain * gain), 1.0f, 17.0f);

        for (int i = 0; i < n; i++) {

            ibuf[i].l = TO_SHORTFRAME(buffer.getSample(I_LEFT, bidx + i) * in_gain);
            ibuf[i].r = stereoIn ? TO_SHORTFRAME(buffer.getSample(I_RIGHT, bidx + i) * in_gain) : ibuf[i].l;

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
        float position  = data.f_position + buffer.getSample(I_POS, bidx);
        float size      = data.f_size + buffer.getSample(I_SIZE, bidx);
        float density   = ((data.f_density + buffer.getSample(I_DENSITY, bidx)) + 1.0f ) / 2.0f;
        float texture   = data.f_texture + buffer.getSample(I_TEXT, bidx);
        bool  freeze    = (data.f_freeze + buffer.getSample(I_FREEZE, bidx)) > 0.5f;
        float mix       = data.f_mix + buffer.getSample(I_MIX, bidx);
        float spread    = data.f_spread + buffer.getSample(I_SPREAD, bidx);
        float feedback  = data.f_feedback + buffer.getSample(I_FEEDBACK, bidx);
        float reverb    = data.f_reverb + buffer.getSample(I_REVERB, bidx);

        //restrict density to .2 to .8 for granular mode, outside this breaks up
        // density = constrain(density, 0.0f, 1.0f);
        // density = (mode == clouds::PLAYBACK_MODE_GRANULAR) ? (density * 0.6f) + 0.2f : density;

        p->freeze = freeze;

        p->gate        = trig;
        p->trigger     = p->gate;

        p->pitch       = constrain(pitch,      -48.0f, 48.0f);
        p->position    = constrain(position,   0.0f, 1.0f);
        p->size        = constrain(size,       0.0f, 1.0f);
        p->texture     = constrain(texture,    0.0f, 1.0f);
        p->density     = constrain(density,    0.0f, 1.0f);

        p->dry_wet     = constrain(mix,           0.0f, 1.0f);
        p->stereo_spread = constrain(spread,      0.0f, 1.0f);
        p->feedback    = constrain(feedback,      0.0f, 1.0f);
        p->reverb      = constrain(reverb,        0.0f, 1.0f);

        processor.Process(ibuf, obuf, n);

        if (stereoOut) {
            for (int i = 0; i < CloudsBlock; i++) {
                buffer.setSample(O_LEFT, bidx + i, FROM_SHORTFRAME(obuf[i].l));
                buffer.setSample(O_RIGHT, bidx + i, FROM_SHORTFRAME(obuf[i].r));
            }
        } else {
            for (int i = 0; i < CloudsBlock; i++) {
                buffer.setSample(O_LEFT, bidx + i, ( FROM_SHORTFRAME(obuf[i].l) + FROM_SHORTFRAME(obuf[i].r) ) / 2.0f) ;
            }
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


void Clds::writeToXml(XmlElement& xml) {
    xml.setAttribute("f_freeze",          bool(data_.f_freeze));
    xml.setAttribute("f_position",        double(data_.f_position));
    xml.setAttribute("f_size",            double(data_.f_size));
    xml.setAttribute("f_pitch",           double(data_.f_pitch));
    xml.setAttribute("f_density",         double(data_.f_density));
    xml.setAttribute("f_texture",         double(data_.f_texture));
    xml.setAttribute("f_mix",             double(data_.f_mix));
    xml.setAttribute("f_spread",          double(data_.f_spread));
    xml.setAttribute("f_feedback",        double(data_.f_feedback));
    xml.setAttribute("f_reverb",          double(data_.f_reverb));
    xml.setAttribute("f_mode",            double(data_.f_mode));
    xml.setAttribute("f_in_gain",         double(data_.f_in_gain));
    xml.setAttribute("f_mono",            bool(data_.f_mono));
    xml.setAttribute("f_lofi",            bool(data_.f_lofi));
}

void Clds::readFromXml(XmlElement& xml) {
    data_.f_freeze      = xml.getBoolAttribute("f_freeze"    , 0.0f);
    data_.f_position    = xml.getDoubleAttribute("f_position"  , 0.5f);
    data_.f_size        = xml.getDoubleAttribute("f_size"      , 0.5f);
    data_.f_pitch       = xml.getDoubleAttribute("f_pitch"     , 0.0f);
    data_.f_density     = xml.getDoubleAttribute("f_density"   , -0.2f);
    data_.f_texture     = xml.getDoubleAttribute("f_texture"   , 0.5f);
    data_.f_mix         = xml.getDoubleAttribute("f_mix"       , 0.5f);
    data_.f_spread      = xml.getDoubleAttribute("f_spread"    , 0.5f);
    data_.f_feedback    = xml.getDoubleAttribute("f_feedback"  , 0.1f);
    data_.f_reverb      = xml.getDoubleAttribute("f_reverb"    , 0.5f);
    data_.f_mode        = xml.getDoubleAttribute("f_mode"      , 0.0f);
    data_.f_in_gain     = xml.getDoubleAttribute("f_in_gain"   , 0.0f);

    data_.f_mono        = xml.getBoolAttribute("f_mono"      , 0.0f);
    data_.f_lofi        = xml.getBoolAttribute("f_lofi"      , 0.0f);
}


void Clds::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void Clds::setStateInformation (const void* data, int sizeInBytes)
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
    return new Clds();
}



