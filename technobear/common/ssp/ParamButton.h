#pragma once

#include "BaseParameter.h"

namespace ssp {

class ParamButton : public juce::Component {
public:
    using Parameter = juce::RangedAudioParameter;
    ParamButton(Parameter &p,
                  unsigned fh = 36,
                  const juce::Colour fg = juce::Colours::white,
                  const juce::Colour bg = juce::Colours::black,
                  bool momentary = true);
    virtual ~ParamButton() = default;

    void onUp();
    void onDown();

protected:
    Parameter &param_;
    juce::ParameterAttachment attachment_;
    virtual void paramChanged(float);
    void paint(juce::Graphics &g) override;

private:
    unsigned fh_ = 36;
    juce::Colour fg_ = juce::Colours::white;
    juce::Colour bg_ = juce::Colours::black;
    bool momentary_ = true;

    juce_UseDebuggingNewOperator

};

}