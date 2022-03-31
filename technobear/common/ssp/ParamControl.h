#pragma once

#include "BaseParameter.h"

namespace ssp {

class BaseParamControl : public juce::Component {
public:
    using Parameter = juce::RangedAudioParameter;

    BaseParamControl(Parameter &p);
    virtual ~BaseParamControl() = default;

    virtual void inc(bool coarse) = 0;
    virtual void dec(bool coarse) = 0;
    virtual void reset() = 0;

    virtual void paramChanged(float);

    void active(bool b) { active_ = b; }

    bool active() { return active_; }

    void fg(juce::Colour c) { fg_ = c; }

    virtual String getTextValue();
protected:
    Parameter &param_;
    juce::ParameterAttachment attachment_;
//    void paint(Graphics &g);
    bool active_;
    juce::Colour fg_ = juce::Colours::red;
    juce_UseDebuggingNewOperator
};


class SimpleParamControl : public BaseParamControl {
public:
    SimpleParamControl(Parameter &p, float coarse = 1.0f, float fine = 0.01f);
    void inc(bool coarse) override;
    void dec(bool coarse) override;
    void reset() override;

protected:
    void paint(juce::Graphics &g) override;

private:
    float coarseInc_ = 0.0f;
    float fineInc_ = 0.0f;
    juce_UseDebuggingNewOperator
};

class LineParamControl : public SimpleParamControl {
public:
    LineParamControl(Parameter &p, float coarse = 1.0f, float fine = 0.01f);
protected:
    void paint(juce::Graphics &g) override;
    juce_UseDebuggingNewOperator
};


class BarParamControl : public SimpleParamControl {
public:
    BarParamControl(Parameter &p, float coarse = 1.0f, float fine = 0.01f);
protected:
    void paint(juce::Graphics &g) override;
    juce_UseDebuggingNewOperator
};


} // namespace