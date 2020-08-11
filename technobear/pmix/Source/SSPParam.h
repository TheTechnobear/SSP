
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"

class SSPParam: public Component
{
public:
    SSPParam() {;}
    void init(const String& label, const String& fmt = "%4.2f", const String& unit = "")  {
        label_ = label;
        unit_ = unit;
        fmt_ = fmt;
        unit_ = unit;
    }

    void active(unsigned var) {
        if (active_ != var) {
            active_ = var;
            repaint();
        }
    }

    bool active() { return active_;}

    void value(float v) {
        if (v_ != v)  {
            v_ = v;
            repaint();
        }
    }

    void label(const String& label) {
        if (label_ != label) {
            label_ = label;
            repaint();
        }
    }

    float value() { return v_;}
    const String fmt() { return fmt_;}
    const String unit() { return unit_;}

private:
    void paint(Graphics &g);
    juce_UseDebuggingNewOperator

    String label_;
    String unit_;
    String fmt_;
    unsigned active_ = 0;
    float v_ = 0.0f;
};
