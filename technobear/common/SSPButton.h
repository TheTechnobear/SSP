
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"

class SSPButton: public Component
{
public:
    SSPButton() {;}
    void init(const String& label, const Colour fg=Colours::white,const Colour bg=Colours::black) {
        label_ = label;
        fg_=fg;
        bg_=bg;
    }
    void label(const String& label) {
        label_ = label;
    }
    void active(bool a) {active_ = a;}
    bool active() { return active_;}

private:
    void paint(Graphics &g);
    juce_UseDebuggingNewOperator

    String label_;
    bool active_ = false;
    Colour fg_=Colours::white;
    Colour bg_=Colours::black;
};
