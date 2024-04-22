#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include <vector>

namespace ssp {

class BaseValueControl : public juce::Component {
public:
    BaseValueControl(const std::string &name, const std::string &label = "",
                     float min = 0.0f, float max = 1.0f, float def = 0.5f);
    virtual ~BaseValueControl() = default;

    virtual void inc(bool coarse) = 0;
    virtual void dec(bool coarse) = 0;
    virtual void reset() = 0;

    virtual void valueChanged(float);

    void active(bool b) { active_ = b; }

    bool active() { return active_; }

    void fg(juce::Colour c) { fg_ = c; }

    void set(float v);

protected:

    std::string name_, label_;
    float value_;
    float min_, max_, default_;

//    void paint(Graphics &g);
    bool active_;
    juce::Colour fg_ = juce::Colours::red;
    juce_UseDebuggingNewOperator
};


class SimpleValueControl : public BaseValueControl {
public:
    SimpleValueControl(const std::string &name, const std::string &label,
                       std::function<void(float v)> cb,
                       float min = 1.0f, float max = 1.0f, float def = 0.5f,
                       float coarse = 1.0f, float fine = 0.01f);
    void inc(bool coarse) override;
    void dec(bool coarse) override;
    void reset() override;

    void valueChanged(float) override;

protected:
    void paint(juce::Graphics &g) override;

private:
    float coarseInc_ = 0.0f;
    float fineInc_ = 0.0f;
    std::function<void(float value)> callback_;
    juce_UseDebuggingNewOperator
};

class LineValueControl : public SimpleValueControl {
public:
    LineValueControl(const std::string &name, const std::string &label,
                     std::function<void(float v)> cb,
                     float min = 1.0f, float max = 1.0f, float def = 0.5f,
                     float coarse = 1.0f, float fine = 0.01f);
protected:
    void paint(juce::Graphics &g) override;
    juce_UseDebuggingNewOperator
};


class BarValueControl : public SimpleValueControl {
public:
    BarValueControl(const std::string &name, const std::string &label,
                    std::function<void(float v)> cb,
                    float min = 1.0f, float max = 1.0f, float def = 0.5f,
                    float coarse = 1.0f, float fine = 0.01f);
protected:
    void paint(juce::Graphics &g) override;
    juce_UseDebuggingNewOperator
};


class ListValueControl : public BaseValueControl {
public:
    ListValueControl(const std::string &name,
                     std::function<void(float idx, const std::string &str)> cb,
                     std::vector<std::string> values = std::vector<std::string>(),
                     float def = 0.0f, 
                     int fh = 18 * COMPACT_UI_SCALE);
    void inc(bool coarse) override;
    void dec(bool coarse) override;
    void reset() override;

    void valueChanged(float) override;

    std::vector<std::string> &values() { return values_; }

    void setValues(std::vector<std::string> &v, int selIdx = -1);
    void setFontHeight(int fh) { fh_ = fh; }
protected:
    void paint(juce::Graphics &g) override;

private:
    int fh_ = 18 * COMPACT_UI_SCALE;
    std::vector<std::string> values_;
    std::function<void(float idx, const std::string &str)> callback_;
    juce_UseDebuggingNewOperator
};


} // namespace