
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
        if (channelIndex % 2 == 0) { n = n + String("L");}
        else { n = n + String("R");}
        return n;
    } else if (channelIndex <= I_VCA_4D) {
        unsigned idx = channelIndex - I_VCA_1A;
        String n = "Vca ";
        unsigned in = (idx / 4)  + 1;
        unsigned out = idx % 4;
        char buf[2] = "A";
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
        char buf[2] = "A";
        buf[0] = 'A' + idx;
        n = n + String(buf);
        if (channelIndex % 2 == 0) { n = n + String("L");}
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
    outBufs_.setSize(MAX_SIG_OUT * 2, samplesPerBlock);
    workBuf_.setSize(1, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginProcessor::calcBuffer(AudioSampleBuffer& buffer, unsigned in, unsigned vca, unsigned n) {
    workBuf_.copyFrom(0, 0, buffer, in,0, n);
    float* wflts = workBuf_.getWritePointer(0);
    const float* vcaflts = buffer.getReadPointer(vca);
    FloatVectorOperations::multiply(wflts, vcaflts, n);
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    unsigned n = buffer.getNumSamples();

    for (unsigned i = 0; i < MAX_SIG_OUT * 2; i++) {
        //clear output buffer
        outBufs_.applyGain(i, 0, n, 0.0f);
    }

    for (unsigned out = 0; out < MAX_SIG_OUT; out++) {
        bool outL = O_SIG_AL + out * 2;
        bool outR = O_SIG_AR + out * 2;
        bool outEnabledL = params_[Percussa::sspOutEn1 + outL ] > 0.5f;
        bool outEnabledR = params_[Percussa::sspOutEn1 + outR ] > 0.5f;
        if (outEnabledL || outEnabledR ) {

            for (unsigned in = 0; in < MAX_SIG_IN; in++) {
                bool inL = I_SIG_1L + out * 2;
                bool inR = I_SIG_1R + out * 2;
                bool inEnabledL = params_[Percussa::sspInEn1 + inL ] > 0.5f;
                bool inEnabledR = params_[Percussa::sspInEn1 + inR ] > 0.5f;

                bool vcaI = (in * 4) + out + I_VCA_1A;
                bool vcaEnabled = params_[Percussa::sspInEn1 + vcaI ] > 0.5f;

                float vca = vca_[in][out];

                if (inEnabledL) {
                    if (vcaEnabled) {
                        calcBuffer(buffer, inL, vcaI, n);
                        outBufs_.addFrom(outL, 0, workBuf_, 0, 0, n, vca);
                    } else {
                        outBufs_.addFrom(outL, 0, buffer, inL, 0, n, vca);
                    }
                }
                if (inEnabledR) {
                    if (vcaEnabled) {
                        calcBuffer(buffer, inR, vcaI, n);
                        outBufs_.addFrom(outR, 0, workBuf_, 0, n, vca);
                    } else {
                        outBufs_.addFrom(outR, 0, buffer, inR, 0, n, vca);
                    }
                }
            }
        }
    }

    for (unsigned out = 0; out < MAX_SIG_OUT; out++) {
        bool outL = O_SIG_AL + out * 2;
        bool outR = O_SIG_AR + out * 2;
        bool outEnabledL = params_[Percussa::sspOutEn1 + outL ] > 0.5f;
        bool outEnabledR = params_[Percussa::sspOutEn1 + outR ] > 0.5f;

        if (outEnabledL) {
            buffer.copyFrom(outL, 0, outBufs_,outL,0, n);
        } else {
            buffer.applyGain(outL, 0, n, 0.0f);
        }

        if (outEnabledR) {
            buffer.copyFrom(outR, 0, outBufs_,outR,0, n);
        } else {
            buffer.applyGain(outR, 0, n, 0.0f);
        }
    }
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



