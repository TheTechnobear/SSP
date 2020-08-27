
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSP.h"


static const char *xmlTag = JucePlugin_Name;


Msw8::Msw8()
{
    memset(params_, 0, sizeof(params_));
}

Msw8::~Msw8()
{
    ;
}

const String Msw8::getName() const
{
    return JucePlugin_Name;
}

int Msw8::getNumParameters()
{
    return Percussa::sspLast;
}

float Msw8::getParameter (int index)
{
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void Msw8::setParameter (int index, float newValue)
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


const String Msw8::getParameterName (int index)
{
    if (index < Percussa::sspLast) return percussaParamsName[index];
    return String();
}

const String Msw8::getParameterText (int index)
{
    if (index < Percussa::sspLast) return percussaParamsText[index];
    return String();
}


const String Msw8::getInputChannelName (int channelIndex) const
{
    switch (channelIndex) {
    case I_IN_SEL:     { return String("InSel");}
    case I_OUT_SEL:    { return String("OutSel");}
    case I_SIG_1:       { return String("In 1");}
    case I_SIG_2:       { return String("In 2");}
    case I_SIG_3:       { return String("In 3");}
    case I_SIG_4:       { return String("In 4");}
    case I_SIG_5:       { return String("In 5");}
    case I_SIG_6:       { return String("In 6");}
    case I_SIG_7:       { return String("In 7");}
    case I_SIG_8:       { return String("In 8");}
    }
    return String("unused:") + String (channelIndex + 1);
}

const String Msw8::getOutputChannelName (int channelIndex) const
{

    switch (channelIndex) {
    case O_SIG_1:       { return String("Out 1");}
    case O_SIG_2:       { return String("Out 2");}
    case O_SIG_3:       { return String("Out 3");}
    case O_SIG_4:       { return String("Out 4");}
    case O_SIG_5:       { return String("Out 5");}
    case O_SIG_6:       { return String("Out 6");}
    case O_SIG_7:       { return String("Out 7");}
    case O_SIG_8:       { return String("Out 8");}
    }
    return String("unused:") + String (channelIndex + 1);
}

bool Msw8::isInputChannelStereoPair (int index) const
{
    return false;
}

bool Msw8::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool Msw8::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Msw8::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Msw8::silenceInProducesSilenceOut() const
{
    return false;
}

double Msw8::getTailLengthSeconds() const
{
    return 0.0;
}

int Msw8::getNumPrograms()
{
    return 1;
}

int Msw8::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return 0;
}

void Msw8::setCurrentProgram (int index)
{
}

const String Msw8::getProgramName (int index)
{
    return String("");
}

void Msw8::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void Msw8::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void Msw8::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



void Msw8::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

}

bool Msw8::hasEditor() const
{
    return true;
}

AudioProcessorEditor* Msw8::createEditor()
{
    return new Msw8Editor (*this);
}


void Msw8::writeToXml(XmlElement& xml) {
    xml.setAttribute("InSel",       double(data_.inSel_));
    xml.setAttribute("OutSel",      double(data_.outSel_));
    xml.setAttribute("UseActive",   double(data_.useActive_));
}

void Msw8::readFromXml(XmlElement& xml) {
    data_.inSel_ = xml.getDoubleAttribute("InSel", 0.0f);
    data_.outSel_ = xml.getDoubleAttribute("OutSel", 0.0f);
    data_.useActive_ = xml.getBoolAttribute("UseActive", false);
    // data_.inCount_ = 0;
    // data_.outCount_ = 0;
}


void Msw8::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void Msw8::setStateInformation (const void* data, int sizeInBytes)
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
    return new Msw8();
}



