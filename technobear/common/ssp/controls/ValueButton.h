
#pragma once

#include <assert.h>
#include <juce_gui_basics/juce_gui_basics.h>

using namespace juce;


namespace ssp {

class ValueButton : public juce::Component {
public:
    ValueButton(const String &label, std::function<void(bool v)> cb, unsigned fh = 16 * COMPACT_UI_SCALE,
                const juce::Colour fg = juce::Colours::white, const juce::Colour bg = juce::Colours::black,
                bool def = false);

    void label(const juce::String &str) { label(str, fh_); }

    void label(const juce::String &str, int fh) {
        label_ = str;
        fh_ = fh;
    }


    void onButton(bool v) {
        if (isToggle_) {
            if(!v) value(!value_);
        } else {
            value(v);
        }
    }

    void value(bool v);
    bool value() { return value_; }
    void setToggle(bool b) { isToggle_ = b; }

private:
    void valueChanged(bool b);

    void paint(juce::Graphics &g);
    std::function<void(bool value)> callback_;

    bool isToggle_ = false;
    String label_;
    bool active_ = false;
    int fh_ = 16 * COMPACT_UI_SCALE;
    juce::Colour fg_ = juce::Colours::white;
    juce::Colour bg_ = juce::Colours::black;

    bool value_ = false;

    juce_UseDebuggingNewOperator
};

}  // namespace ssp