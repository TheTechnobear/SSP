
#pragma once

#include <assert.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

class ValueButton : public juce::Component {
public:
    ValueButton(const String &label,
                std::function<void(bool v)> cb,
                unsigned fh = 32,
                const juce::Colour fg = juce::Colours::white, const juce::Colour bg = juce::Colours::black,
                bool def = false);

    void label(const juce::String &str) {
        label(str,fh_);
    }

    void label(const juce::String &str, int fh) {
        label_ = str;
        fh_= fh;
    }

    void valueChanged(bool b);

    bool value() { return value_; }

    void value(bool v);
    bool toggle();

private:
    void paint(juce::Graphics &g);
    std::function<void(bool value)> callback_;

    String label_;
    bool active_ = false;
    int fh_=36;
    juce::Colour fg_ = juce::Colours::white;
    juce::Colour bg_ = juce::Colours::black;

    bool value_ = false;

    juce_UseDebuggingNewOperator
};
