
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSP.h"


static const char *xmlTag = JucePlugin_Name;

PluginProcessor::PluginProcessor()
{
    memset(params_, 0, sizeof(params_));
    ac_ = false;
}

PluginProcessor::~PluginProcessor()
{
    ;
}

const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

int PluginProcessor::getNumParameters()
{
    return Percussa::sspLast;
}

float PluginProcessor::getParameter (int index)
{
    // SSP appears to only send parameters if they appear to have changed
    // so we need to track the parameters values(!)
    if (index < Percussa::sspLast) return params_[index];
    return 0.0f;
}

void PluginProcessor::setParameter (int index, float newValue)
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

const String PluginProcessor::getParameterName (int index)
{
    if (index < Percussa::sspLast) return percussaParamsName[index];
    return String();
}

const String PluginProcessor::getParameterText (int index)
{
    if (index < Percussa::sspLast) return percussaParamsText[index];
    return String();
}

const String PluginProcessor::getInputChannelName (int channelIndex) const
{
    if (channelIndex <= I_SIG_4R) {
        String n = "In ";
        unsigned idx = (channelIndex / 2) + 1;
        n = n + String(idx);
        if (n % 2 == 0) { n = n + String("L");}
        else { n = n + String("R");}
        return n;
    } else if (channelIndex <= I_VCA_4D) {
        unsigned idx = channelIndex - I_VCA_1A;
        String n = "Vca ";
        unsigned in = (idx / 4)  + 1;
        unsigned out = idx % 4;
        char buf[2] = 'A';
        buf[0] = 'A' + out;
        n = n + String(in) + String(buf);
        return n;
    }

    return String("unused:") + String (channelIndex + 1);
}

const String PluginProcessor::getOutputChannelName (int channelIndex) const
{

    if (channelIndex <= O_SIG_AR) {
        String n = "Out ";
        unsigned idx = (channelIndex / 2);
        char buf[2] = 'A';
        buf[0] = 'A' + idx;
        n = n + String(buf);
        if (n % 2 == 0) { n = n + String("L");}
        else { n = n + String("R");}
        return n;
    }

    return String("unused:") + String (channelIndex + 1);
}

bool PluginProcessor::isInputChannelStereoPair (int index) const
{
    return false;
}

bool PluginProcessor::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

int PluginProcessor::getCurrentProgram()
{
    // SSP queries what program is currently loaded
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
}

const String PluginProcessor::getProgramName (int index)
{
    return String("");
}

void PluginProcessor::changeProgramName (int index, const String& newName)
{
    // SSP - not using
}

void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    buffer_.setSize(MAX_SIG_OUT * 2, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    unsigned n = buffer.getNumSamples();

    // bool stereoIn = params_[Percussa::sspInEn1 + I_RIGHT ] > 0.5f;
    // bool stereoOut = params_[Percussa::sspOutEn1 + O_RIGHT ] > 0.5f;
    // for (int i = 0; i < buffer.getNumSamples(); i++) {
    //     buffer.getSample(I_LEFT, 0 );
    // }


    // copy input buffers
    // for(unsigned i=0;i< MAX_SIG_IN;i++) {
    //     unsigned idx = i*2;
    //     if(params_[I_SIG_1L+ idx] > 0.5f)
    //         buffer.copyFrom(idx, 0, buffer, I_SIG_1L idx 0, n);
    //     else 
    //         buffer.applyGain(idx, 0, n, 0.0f;)
        
    //     idx++;

    //     if(params_[I_SIG_1L+ idx] > 0.5f)
    //         buffer.copyFrom(idx, 0, buffer, I_SIG_1L idx 0, n);
    //     else 
    //         buffer.applyGain(idx, 0, n, 0.0f;)

    // }


    for(unsigned i=0;i< MAX_SIG_OUT*2;i++) {
        //clear output buffer
        buffer_.applyGain(i, 0, n,0.0f);
    }

    for(unsigned out=0;out<MAX_SIG_OUT;out++) {
        for(unsigned in=0;in<MAX_SIG_IN;in++) {
            float vca=vca_[in][out];

            // neen a tmp buffer there, to copy into, then i can use the multiply vector on it
            
            float* inl= buffer.getReadPointer((in*2)+I_SIG_1L);
            float* inl= buffer.getReadPointer((in*2)+I_SIG_1R);
            float* vcavec= buffer.getReadPointer((in*4) +out+ I_VCA_1A);

            multiply(inl,vcavec);
            multiply(inr,vcavec);

            addTo(outl, inl, vca);
            addTo(outr, inr, vca);

        }

    }
    



    // for (int i = 0; i < buffer.getNumSamples(); i++) {
    //     buffer.setSample(O_LEFT,i,0 );
    // }

}

bool PluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}


void PluginProcessor::writeToXml(XmlElement& xml) {
    xml.setAttribute("ac",  bool(ac_));
}

void PluginProcessor::readFromXml(XmlElement& xml) {
    ac_      = xml.getBoolAttribute("ac"  , false);
}


void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    XmlElement xml(xmlTag);
    writeToXml(xml);
    copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new PluginProcessor();
}



