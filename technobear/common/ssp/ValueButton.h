
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"

class ValueButton : public Component {
public:
    ValueButton(const String &label,
                std::function<void(bool v)> cb,
                unsigned fh = 32,
                const Colour fg = Colours::white, const Colour bg = Colours::black,
                bool def = false);

    void label(const String &label) {
        label_ = label;
    }

    void valueChanged(bool b);

    bool value() { return value_; }

    void value(bool v);
    bool toggle();

private:
    void paint(Graphics &g);
    std::function<void(bool value)> callback_;

    String label_;
    bool active_ = false;
    int fh_=36;
    Colour fg_ = Colours::white;
    Colour bg_ = Colours::black;

    bool value_ = false;

    juce_UseDebuggingNewOperator
};
