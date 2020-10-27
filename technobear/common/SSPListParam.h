
#pragma once

#include <assert.h>
#include <string>
#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

class SSPListParam: public Component
{
public:
    SSPListParam() {;}
    void init(const String& label, const std::vector<std::string> &list)  {
        label_ = label;
        list_ = list;
    }

    void active(unsigned var) {
        if (active_ != var) {
            active_ = var;
            repaint();
        }
    }

    bool active() { return active_;}

    // void encbutton(bool b);
    void encoder(float v) {
        if (v > 0.0f) inc();
        else dec();
    }

    void inc() {
        if ((v_ + 1 ) < list_.size()) v_++;
    }
    void dec() {
        if (v_ > 0) v_--;
    }

    void value(unsigned v) {
        if (v_ != v && v < list_.size())  {
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

    unsigned  value() { return v_;}
    const std::string& fmtValue();

private:
    void paint(Graphics &g);
    juce_UseDebuggingNewOperator

    String label_;
    std::vector<std::string> list_;
    unsigned active_ = 0;
    unsigned v_ = 0.0f;
};
