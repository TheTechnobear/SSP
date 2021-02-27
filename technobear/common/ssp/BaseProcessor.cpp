#include "BaseProcessor.h"

#include <juce_core/juce_core.h>


namespace ssp {


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
    float v_;
};

void BaseProcessor::addBaseParameters(AudioProcessorValueTreeState::ParameterLayout &params) {

#ifdef __APPLE__
    static constexpr bool defIOState = true;
#else
    static constexpr bool defIOState = false;
#endif

    for (unsigned i = 0; i < sspParams::numEnc; i++) {
        params.add(std::make_unique<EncoderParameter>(
            percussaParamsName[Percussa::sspEnc1 + i],
            percussaParamsText[Percussa::sspEnc1 + i],
            -1.0f, 1.0f, 0.0f, 0.5f));
    }

    for (unsigned i = 0; i < sspParams::numEnc; i++) {
        params.add(std::make_unique<BaseBoolParameter>(
            percussaParamsName[Percussa::sspEncSw1 + i],
            percussaParamsText[Percussa::sspEncSw1 + i],
            false));
    }

    for (unsigned i = 0; i < sspParams::numSw; i++) {
        params.add(std::make_unique<BaseBoolParameter>(
            percussaParamsName[Percussa::sspSw1 + i],
            percussaParamsText[Percussa::sspSw1 + i],
            false));
    }

    for (unsigned i = 0; i < sspParams::numCtrlSw; i++) {
        params.add(std::make_unique<BaseBoolParameter>(
            percussaParamsName[Percussa::sspSwLeft + i],
            percussaParamsText[Percussa::sspSwLeft + i],
            false));
    }


    for (unsigned i = 0; i < sspParams::numOut; i++) {
        params.add(std::make_unique<BaseBoolParameter>(
            percussaParamsName[Percussa::sspOutEn1 + i],
            percussaParamsText[Percussa::sspOutEn1 + i],
            defIOState,
            [this, i](const String &id, bool b) { onOutputChanged(i, b); })
        );
        onOutputChanged(i, defIOState);
    }
    for (unsigned i = 0; i < sspParams::numIn; i++) {
        params.add(std::make_unique<BaseBoolParameter>(
            percussaParamsName[Percussa::sspInEn1 + i],
            percussaParamsText[Percussa::sspInEn1 + i],
            defIOState,
            [this, i](const String &id, bool b) { onInputChanged(i, b); })
        );
        onInputChanged(i, defIOState);
    }
}


void BaseProcessor::getStateInformation(MemoryBlock &destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BaseProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}


void BaseProcessor::onInputChanged(unsigned i, bool b) {
    if (i < sspParams::numIn) inputEnabled[i] = b;
}

void BaseProcessor::onOutputChanged(unsigned i, bool b) {
    if (i < sspParams::numOut) outputEnabled[i] = b;
}


}
