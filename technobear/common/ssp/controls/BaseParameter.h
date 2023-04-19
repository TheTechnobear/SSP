#pragma once


#include <juce_audio_processors/juce_audio_processors.h>

#include "SSP.h"

namespace ssp {

class BaseFloatParameter : public juce::AudioParameterFloat {
public:
    typedef std::function<void(const juce::String &id, float v)> ParamCallback;

    BaseFloatParameter(juce::String parameterID,
                       juce::String parameterName,
                       float minValue,
                       float maxValue,
                       float defaultValue,
                       ParamCallback callback = nullptr)
        : juce::AudioParameterFloat(parameterID, parameterName, minValue, maxValue, defaultValue),
          valueCallback_(callback) {
    }

    BaseFloatParameter(juce::String parameterID,
                       juce::String parameterName,
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
//    unsigned PID_;
    ParamCallback valueCallback_;
};


class BaseBoolParameter : public juce::AudioParameterBool {
public:
    typedef std::function<void(const juce::String &id, bool b)> ParamCallback;

    BaseBoolParameter(juce::String parameterID,
                      juce::String parameterName,
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
    typedef std::function<void(const juce::String &id, int v)> ParamCallback;

    BaseChoiceParameter(const juce::String &parameterID, const juce::String &parameterName,
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

class BaseIntParameter : public juce::AudioParameterInt {
public:
    typedef std::function<void(const juce::String &id, bool b)> ParamCallback;


    BaseIntParameter(juce::String parameterID,
                     juce::String parameterName,
                    float minValue,
                    float maxValue,
                    float defaultValue,
        ParamCallback callback = nullptr)
    : juce::AudioParameterInt(parameterID, parameterName, minValue, maxValue, defaultValue),
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