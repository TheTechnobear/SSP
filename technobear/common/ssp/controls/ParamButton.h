#pragma once

#include "BaseParameter.h"

namespace ssp {

class ParamButton : public juce::Component {
public:
    using Parameter = juce::RangedAudioParameter;
    ParamButton(Parameter &p, unsigned fh = 18 * COMPACT_UI_SCALE, const juce::Colour fg = juce::Colours::white,
                const juce::Colour bg = juce::Colours::black, bool momentary = false);
    virtual ~ParamButton() = default;

    void onUp();
    void onDown();

    void onClick();

    void mouseUp(const juce::MouseEvent &event) override;
    void mouseDown(const juce::MouseEvent &event) override;

protected:
    Parameter &param_;
    juce::ParameterAttachment attachment_;
    virtual void paramChanged(float);
    void paint(juce::Graphics &g) override;

private:
    unsigned fh_ = 18 * COMPACT_UI_SCALE;
    juce::Colour fg_ = juce::Colours::white;
    juce::Colour bg_ = juce::Colours::black;
    bool momentary_ = true;

    juce_UseDebuggingNewOperator
};

}  // namespace ssp