
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
    // Logger::writeToLog(getParameterName(index) + ":" + String(newValue));
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

        static constexpr unsigned inFrom = Percussa::sspInEn1 + I_SIG_1;
        static constexpr unsigned inTo = Percussa::sspInEn1 + (I_MAX - 1);
        static constexpr unsigned outFrom = Percussa::sspOutEn1 + O_SIG_A;
        static constexpr unsigned outTo = Percussa::sspOutEn1 + (O_MAX - 1);

        if (index >= inFrom  || index <= inTo) {
            data_.inCount_ = 0;
            for (int i = inFrom; i <= inTo ; i++) {
                data_.inCount_ += (params_[i] > 0.5f);
            }
        }
        if (index >= outFrom || index <= outTo) {
            data_.outCount_ = 0;
            for (int i = outFrom; i <= outTo ; i++) {
                data_.outCount_ += (params_[i] > 0.5f);
            }

        }
        // Logger::writeToLog("IO Enabled : " + String(data_.inCount_) + "/" + String(data_.outCount_));

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
    case O_SIG_A:       { return String("Out A");}
    case O_SIG_B:       { return String("Out B");}
    case O_SIG_C:       { return String("Out C");}
    case O_SIG_D:       { return String("Out D");}
    case O_SIG_E:       { return String("Out E");}
    case O_SIG_F:       { return String("Out F");}
    case O_SIG_G:       { return String("Out G");}
    case O_SIG_H:       { return String("Out H");}
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
    inputBuffer_.setSize(1, samplesPerBlock);
    lastBuffer_.setSize(1, samplesPerBlock);
}

void Msw8::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



void Msw8::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    unsigned n = buffer.getNumSamples();
    float cvInS = buffer.getSample(I_IN_SEL, 0);
    float cvOutS = buffer.getSample(I_OUT_SEL, 0);

    // determine indexes of io
    unsigned iIdx = 0, oIdx = 0;
    unsigned inSIdx = unsigned(((constrain(data_.inSel_ + cvInS, -1.0f, 0.999f) + 1.0f )  * (data_.useActive_ ? data_.inCount_ : 8.0f) ) / 2.0f);
    unsigned outSIdx = unsigned(((constrain(data_.outSel_ + cvOutS, -1.0f, 0.999f) + 1.0f )  * (data_.useActive_ ? data_.outCount_ : 8.0f) ) / 2.0f);

    if (data_.useActive_) {
        for (unsigned x = 0; x < 8; x++) {
            if (params_[Percussa::sspInEn1 + I_SIG_1 + x] > 0.5f) {
                if (inSIdx == iIdx) {
                    break;
                }
                iIdx++;
            }
        }
        for (unsigned y = 0; y < 8; y++) {
            if (params_[Percussa::sspOutEn1 + O_SIG_A + y] > 0.5f) {
                if (outSIdx == oIdx) {
                    break;
                }
                oIdx++;
            }
        }
    } else {
        iIdx = inSIdx;
        oIdx = outSIdx;
    }

    // what io are active
    bool iActive =  params_[Percussa::sspInEn1 + I_SIG_1 + iIdx] > 0.5f;
    bool oActive =  params_[Percussa::sspOutEn1 + O_SIG_A + oIdx] > 0.5f;
    bool liActive = params_[Percussa::sspInEn1 + I_SIG_1 +  data_.lastInIdx_] > 0.5f;
    bool loActive = params_[Percussa::sspOutEn1 + O_SIG_A + data_.lastOutIdx_] > 0.5f;

//    bool soft = data_.soft_;
    bool soft=true;


    // copy input to output
    // if soft, then we need to ramp inputs and outputs when they change
    if (iActive) {
        inputBuffer_.copyFrom(0, 0, buffer, I_SIG_1 + iIdx, 0, n);
    } else {
        inputBuffer_.applyGain(0, 0, n, 0.0f);
    }


    if (soft) {
        if (data_.lastInIdx_ != iIdx) {
            if (liActive) {
                // only need in buf if its differ
                lastBuffer_.copyFrom(0, 0, buffer, data_.lastInIdx_, 0, n);
                // fade down 0..n/2 , mute n/2..n,
                lastBuffer_.applyGainRamp(0, 0, n / 2, 1.0f, 0.0f); // ramp down
                lastBuffer_.applyGain(0, n / 2, n, 0.0f);
            } else {
                lastBuffer_.applyGain(0, 0, n, 0.0f);
            }
        }
    }


    buffer.clear();


    if (soft && data_.lastInIdx_ != iIdx) {
        // mute 0..n/2, fade up n/2.. n
        inputBuffer_.applyGain(0, 0, n / 2, 0.0f);
        inputBuffer_.applyGainRamp(0, n / 2, n, 0.f, 1.0f);// ramp up
    }

    if (oActive) {
        buffer.copyFrom(O_SIG_A + oIdx, 0, inputBuffer_, 0, 0, n);
    }

    if (soft) {
        if (data_.lastOutIdx_ != oIdx) {
            // output changed
            if (data_.lastInIdx_ != iIdx) {
                // copy, ramps already applied
                if (loActive) {
                    buffer.copyFrom(O_SIG_A + data_.lastOutIdx_, 0, lastBuffer_, 0, 0, n);
                }
            } else {
                // input buff unchanged, but we need to ramp down old output, ramp up new output
                if (loActive) {
                    buffer.copyFrom(O_SIG_A + data_.lastOutIdx_, 0, inputBuffer_, 0, 0, n);
                    buffer.applyGainRamp(O_SIG_A + data_.lastOutIdx_, 0, n / 2, 1.0f, 0.0f); // ramp down
                    buffer.applyGain(O_SIG_A + data_.lastOutIdx_, n / 2, n, 0.0f);
                }

                if (oActive) {
                    buffer.applyGain(O_SIG_A + oIdx, 0, n / 2, 0.0f);
                    buffer.applyGainRamp(O_SIG_A + oIdx, n / 2, n, 0.f, 1.0f); // ramp up
                }
            }
        } else {
            // output unchanged
            if (data_.lastInIdx_ != iIdx) {
                // copy, ramps already applied
                if (oActive) {
                    buffer.copyFrom(O_SIG_A + oIdx, 0, lastBuffer_, 0, 0, n / 2);
                }
            } // else , unchanged so nothing needed
        }
    }

    data_.lastInIdx_ = iIdx;
    data_.lastOutIdx_ = oIdx;
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
    xml.setAttribute("Soft",   double(data_.soft_));
}

void Msw8::readFromXml(XmlElement& xml) {
    data_.inSel_ = xml.getDoubleAttribute("InSel", 0.0f);
    data_.outSel_ = xml.getDoubleAttribute("OutSel", 0.0f);
    data_.useActive_ = xml.getBoolAttribute("UseActive", false);
    data_.soft_ = xml.getBoolAttribute("Soft", false);
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



