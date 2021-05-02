#include "BaseProcessor.h"

#include <juce_core/juce_core.h>
#include <juce_audio_devices/juce_audio_devices.h>

#include <assert.h>
namespace ssp {

#ifdef __APPLE__
// apple
#ifdef DISABLE_SSP_PARAMS
// disable ssp parameters , so enable all io
static constexpr bool exposeSSPParams = false;
static constexpr bool defIOState = true;
#else
// enable ssp parameters
static constexpr bool exposeSSPParams = true;
static constexpr bool defIOState = true;
#endif

#else
// linux etc
static constexpr bool defIOState = false;
static constexpr bool exposeSSPParams = true;
#endif


class EncoderParameter : public BaseFloatParameter {
public:
    EncoderParameter(String parameterID,
                     String parameterName,
                     float minValue,
                     float maxValue,
                     float defaultValue,
                     float inc,
                     BaseFloatParameter::ParamCallback callback = nullptr)
        : BaseFloatParameter(parameterID, parameterName, minValue, maxValue, defaultValue, 0.0f, callback) {
    }

    float getValue() const override {
        return v_;
    }

    bool isAutomatable() const override {
        return exposeSSPParams;

    }

    void setValue(float v) override {
        if (v > 0.5f) {
            v_ += v / 1000.f;
        } else if (v < 0.5f) {
            v_ -= v / 1000.f;
        } else {
            v_ = 0.0f;
        }
    }

private:
    float v_ = 0.0f;
};


class SSPBoolParameter : public BaseBoolParameter {
public:

    SSPBoolParameter(String parameterID,
                     String parameterName,
                     bool defaultValue,
                     BaseBoolParameter::ParamCallback callback = nullptr)
        : BaseBoolParameter(parameterID, parameterName, defaultValue, callback) {

    }

    bool isAutomatable() const override {
        return exposeSSPParams;
    }

};


BaseProcessor::BaseProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    juce::AudioProcessorValueTreeState::ParameterLayout pl)
    : AudioProcessor(ioLayouts), apvts(*this, nullptr, "state", std::move(pl)) {
    addListener(this);
}

BaseProcessor::~BaseProcessor() {
    removeListener(this);
}


void BaseProcessor::addBaseParameters(AudioProcessorValueTreeState::ParameterLayout &params) {
    for (unsigned i = 0; i < sspParams::numEnc; i++) {
        params.add(std::make_unique<EncoderParameter>(
            percussaParamsName[Percussa::sspEnc1 + i],
            percussaParamsText[Percussa::sspEnc1 + i],
            -1.0f, 1.0f, 0.0f, 0.5f));
    }

    for (unsigned i = 0; i < sspParams::numEnc; i++) {
        params.add(std::make_unique<SSPBoolParameter>(
            percussaParamsName[Percussa::sspEncSw1 + i],
            percussaParamsText[Percussa::sspEncSw1 + i],
            false));
    }

    for (unsigned i = 0; i < sspParams::numSw; i++) {
        params.add(std::make_unique<SSPBoolParameter>(
            percussaParamsName[Percussa::sspSw1 + i],
            percussaParamsText[Percussa::sspSw1 + i],
            false));
    }

    for (unsigned i = 0; i < sspParams::numCtrlSw; i++) {
        params.add(std::make_unique<SSPBoolParameter>(
            percussaParamsName[Percussa::sspSwLeft + i],
            percussaParamsText[Percussa::sspSwLeft + i],
            false));
    }


    for (unsigned i = 0; i < sspParams::numOut; i++) {
        params.add(std::make_unique<SSPBoolParameter>(
            percussaParamsName[Percussa::sspOutEn1 + i],
            percussaParamsText[Percussa::sspOutEn1 + i],
            defIOState,
            [this, i](const String &id, bool b) { onOutputChanged(i, b); })
        );
    }
    for (unsigned i = 0; i < sspParams::numIn; i++) {
        params.add(std::make_unique<SSPBoolParameter>(
            percussaParamsName[Percussa::sspInEn1 + i],
            percussaParamsText[Percussa::sspInEn1 + i],
            defIOState,
            [this, i](const String &id, bool b) { onInputChanged(i, b); })
        );
    }
}

void BaseProcessor::init() {
    for (unsigned i = 0; i < sspParams::numOut; i++) {
        onOutputChanged(i, defIOState);
    }
    for (unsigned i = 0; i < sspParams::numIn; i++) {
        onInputChanged(i, defIOState);
    }
}


static const char *MIDI_TAG_IN_DEV = "MIDI_IN";
static const char *MIDI_TAG_OUT_DEV = "MIDI_OUT";

void BaseProcessor::midiFromXml(juce::XmlElement *xml) {
    setMidiIn(xml->getStringAttribute(MIDI_TAG_IN_DEV).toStdString());
    setMidiOut(xml->getStringAttribute(MIDI_TAG_OUT_DEV).toStdString());
}

void BaseProcessor::midiToXml(juce::XmlElement *xml) {
    auto in = MidiInput::getAvailableDevices();
    if (in.size() > 0) midiInDeviceId_ = in[0].identifier.toStdString();

    xml->setAttribute(MIDI_TAG_IN_DEV, midiInDeviceId_);
    xml->setAttribute(MIDI_TAG_OUT_DEV, midiOutDeviceId_);
}


void BaseProcessor::customFromXml(juce::XmlElement *xml) {

}

void BaseProcessor::customToXml(juce::XmlElement *xml) {

}

static const char *VST_XML_TAG = "VST";
static const char *MIDI_XML_TAG = "MIDI";
static const char *CUSTOM_XML_TAG = "CUSTOM";


void BaseProcessor::getStateInformation(MemoryBlock &destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xmlVst = std::make_unique<XmlElement>(VST_XML_TAG);
    std::unique_ptr<juce::XmlElement> xmlState(state.createXml());
    std::unique_ptr<juce::XmlElement> xmlMidi = std::make_unique<XmlElement>(MIDI_XML_TAG);
    std::unique_ptr<juce::XmlElement> xmlCustom = std::make_unique<XmlElement>(CUSTOM_XML_TAG);

    midiToXml(xmlMidi.get());
    customToXml(xmlCustom.get());

    if (xmlState) xmlVst->addChildElement(xmlState.release());
    if (xmlMidi) xmlVst->addChildElement(xmlMidi.release());
    if (xmlCustom) xmlVst->addChildElement(xmlCustom.release());
    copyXmlToBinary(*xmlVst, destData);
}


void BaseProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr) {
        if (xml->hasTagName(apvts.state.getType())) {
            // backwards compat
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
        } else if (xml->hasTagName(VST_XML_TAG)) {
            auto xmlState = xml->getChildByName(apvts.state.getType()); //STATE
            if (xmlState != nullptr) apvts.replaceState(juce::ValueTree::fromXml(*xmlState));

            auto xmlMidi = xml->getChildByName(MIDI_XML_TAG);
            if (xmlMidi != nullptr) midiFromXml(xmlMidi);

            auto xmlCustom = xml->getChildByName(CUSTOM_XML_TAG);
            if (xmlCustom != nullptr) customFromXml(xmlCustom);
        }
    }
}


void BaseProcessor::onInputChanged(unsigned i, bool b) {
    if (i < sspParams::numIn) inputEnabled[i] = b;
}

void BaseProcessor::onOutputChanged(unsigned i, bool b) {
    if (i < sspParams::numOut) outputEnabled[i] = b;
}

void BaseProcessor::setMidiIn(std::string id) {
    midiInDeviceId_ = id;
    if (!midiInDeviceId_.empty()) {
//        midiInDevice_ = MidiInput::openDevice(midiInDeviceId_, &midiCallback_);
        midiInDevice_ = MidiInput::openDevice(midiInDeviceId_, this);
        if (midiInDevice_ && midiInDevice_->getIdentifier().toStdString() == midiInDeviceId_) {
            midiInDevice_->start();
            Logger::writeToLog("MIDI IN OPEN -> " + id);
        } else {
            Logger::writeToLog("MIDI FAILED TO OPEN -> " + id);
        }
    } else {
        midiInDevice_ = nullptr;
    }
}

void BaseProcessor::setMidiOut(std::string id) {
    midiOutDeviceId_ = id;
    if (!midiOutDeviceId_.empty()) {
        midiOutDevice_ = MidiOutput::openDevice(midiOutDeviceId_);
        if (midiOutDevice_ && midiOutDevice_->getIdentifier().toStdString() == midiOutDeviceId_) {
            midiOutDevice_->startBackgroundThread();
            Logger::writeToLog("MIDI OUT OPEN -> " + id);
        } else {
            Logger::writeToLog("MIDI FAILED TO OPEN -> " + id);
        }
    } else {
        midiOutDevice_ = nullptr;
    }
}

void BaseProcessor::automateParam(int idx, float v) {
    assert(idx >= Percussa::sspLast);

    auto &plist = getParameters();
    if (idx < plist.size()) {
        auto p = plist[idx];
        if (p->getValue() != v) {
            p->beginChangeGesture();
            p->setValueNotifyingHost(v);
            p->endChangeGesture();
        }
    }
}

void BaseProcessor::handleMidi(const MidiMessage &msg) {
    if (midiChannel_ == 0 || msg.getChannel() == midiChannel_) {
        if (midiLearn_) {
            if (lastLearn_.paramIdx_ > 0) {
                if (msg.isController()) {
                    auto &m = lastLearn_.midi_;
                    m.type_ = MidiAutomation::Midi::T_CC;
                    m.num_ = msg.getControllerNumber();

                    bool found = false;
                    for (auto &a : midiAutomation_) {
                        if (a.paramIdx_ == lastLearn_.paramIdx_) {
                            found = true;
                            a = lastLearn_;
                        }
                    }
                    if (!found) {
                        midiAutomation_.push_back(lastLearn_);
                    }

                    lastLearn_.reset();
                }
            }
        }

        for (auto &a : midiAutomation_) {
            if ((msg.isController() && a.midi_.type_ == MidiAutomation::Midi::T_CC)
                && (msg.getControllerNumber() == a.midi_.num_)
                ) {
                float val = float(msg.getControllerValue()) / 127.0f;
                val = (val * a.scale_) + a.offset_;
                automateParam(a.paramIdx_, val);
            }
        }
    }
}


void BaseProcessor::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) {
    Logger::writeToLog("MidiCallback -> " + message.getDescription());
    handleMidi(message);
}


void BaseProcessor::midiLearn(bool b) {
    if (midiLearn_ != b) {
        midiLearn_ = b;
        if (midiLearn_) {
            lastLearn_.reset();
        }
    }
}

void BaseProcessor::audioProcessorParameterChanged(AudioProcessor *processor,
                                                   int parameterIndex,
                                                   float) {
    assert(processor == this);
    if (midiLearn_ && parameterIndex >= Percussa::sspLast) {
        lastLearn_.paramIdx_ = parameterIndex;
    }
}

}
