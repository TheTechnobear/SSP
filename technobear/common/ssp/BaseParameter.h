#pragma once


#include <juce_audio_processors/juce_audio_processors.h>

//using namespace juce;

using String=juce::String;

#include "SSP.h"

namespace ssp {

class BaseFloatParameter : public juce::AudioParameterFloat {
public:
    typedef std::function<void(const String &id, float v)> ParamCallback;

    BaseFloatParameter(String parameterID,
                       String parameterName,
                       float minValue,
                       float maxValue,
                       float defaultValue,
                       ParamCallback callback = nullptr)
        : juce::AudioParameterFloat(parameterID, parameterName, minValue, maxValue, defaultValue),
          valueCallback_(callback) {
    }

    BaseFloatParameter(String parameterID,
                       String parameterName,
                       float minValue,
                       float maxValue,
                       float defaultValue,
                       float inc,
                       ParamCallback callback = nullptr)
        : juce::AudioParameterFloat(parameterID, parameterName, juce::NormalisableRange<float>(minValue, maxValue,inc), defaultValue),
          valueCallback_(callback) {
    }


protected:
    void valueChanged(float newValue) override {
        if (valueCallback_) valueCallback_(paramID, newValue);
    }

private:
    unsigned PID_;
    ParamCallback valueCallback_;
};

class BaseBoolParameter : public juce::AudioParameterBool {
public:
    typedef std::function<void(const String &id, bool b)> ParamCallback;

    BaseBoolParameter(String parameterID,
                      String parameterName,
                      bool defaultValue,
                      ParamCallback callback = nullptr)
        : juce::AudioParameterBool(parameterID, parameterName, defaultValue),
          valueCallback_(callback) {
    }

protected:
    void valueChanged(bool newValue) override {
        if (valueCallback_) valueCallback_(paramID, newValue);
    }

private:
    ParamCallback valueCallback_;
};


class BaseChoiceParameter : public juce::AudioParameterChoice {
public:
    typedef std::function<void(const String &id, int v)> ParamCallback;

    BaseChoiceParameter(const String &parameterID, const String &parameterName,
                        const juce::StringArray &choices,
                        int defaultItemIndex,
                        ParamCallback callback = nullptr)
        : juce::AudioParameterChoice(parameterID, parameterName, choices, defaultItemIndex),
          valueCallback_(callback) {
    }

protected:
    void valueChanged(int newValue) override {
        if (valueCallback_) valueCallback_(paramID, newValue);
    }

private:
    ParamCallback valueCallback_;
};


} // namespace