
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSP.h"


static const char *xmlTag = JucePlugin_Name;

#include "algos/Algos.h"

void PluginProcessor::initAlgos() {

    algos_[A_P_ADDER] = std::make_shared<AgPrecAdder>();
    algoN_ = A_P_ADDER;
    algo_ = algos_[algoN_];
}



PluginProcessor::PluginProcessor()
{
    memset(params_, 0, sizeof(params_));
    initAlgos();
}

PluginProcessor::~PluginProcessor()
{
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
    switch (channelIndex) {
    case I_X_1: { return String("X 1");}
    case I_Y_1: { return String("Y 1");}
    case I_Z_1: { return String("Z 1");}
    }
    return String("unused:") + String (channelIndex + 1);
}

const String PluginProcessor::getOutputChannelName (int channelIndex) const
{
    switch (channelIndex) {
    case O_A_1: { return String("A 1");}
    case O_B_1: { return String("B 1");}
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
    outBufs_.setSize(2, samplesPerBlock);
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    unsigned n = buffer.getNumSamples();

    bool outEnabledA = params_[Percussa::sspOutEn1 + O_A_1 ] > 0.5f;
    bool outEnabledB = params_[Percussa::sspOutEn1 + O_B_1 ] > 0.5f;

    if (outEnabledA || outEnabledB) {
        bool inEnabledX = params_[Percussa::sspInEn1 + I_X_1 ] > 0.5f;
        bool inEnabledY = params_[Percussa::sspInEn1 + I_Y_1 ] > 0.5f;
        bool inEnabledZ = params_[Percussa::sspInEn1 + I_Z_1 ] > 0.5f;

        float* outA = outEnabledA ? buffer.getWritePointer(0) : nullptr;
        float* outB = outEnabledB ? buffer.getWritePointer(1) : nullptr;

        const float* inX = inEnabledX ? buffer.getReadPointer(I_X_1) : nullptr;
        const float* inY = inEnabledY ? buffer.getReadPointer(I_Y_1) : nullptr;
        const float* inZ = inEnabledZ ? buffer.getReadPointer(I_Z_1) : nullptr;

        algo_->process(inX, inY, inZ, outA, outB, n);
    }


    if (outEnabledA) {
        buffer.copyFrom(O_A_1, 0, outBufs_, 0, 0, n);
    } else {
        buffer.applyGain(O_B_1, 0, n, 0.0f);
    }

    if (outEnabledB) {
        buffer.copyFrom(O_A_1, 0, outBufs_, 1, 0, n);
    } else {
        buffer.applyGain(O_B_1, 0, n, 0.0f);
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
    // xml.setAttribute("x1", double(x_));
    // xml.setAttribute("y1", double(y_));
    // xml.setAttribute("z1", double(z_));
}

void PluginProcessor::readFromXml(XmlElement& xml) {
    // x_      = xml.getDoubleAttribute("x1"  , 0.0f);
    // y_      = xml.getDoubleAttribute("y1"  , 0.0f);
    // z_      = xml.getDoubleAttribute("z1"  , 0.0f);
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



