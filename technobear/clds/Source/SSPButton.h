
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"

class SSPButton: public Component
{
public:
    SSPButton() {;}
    void init(const String& label) {
        label_=label;
    }
    void active(bool a) {active_=a;}
    bool active() { return active_;}

private:
    void paint(Graphics &g);
    juce_UseDebuggingNewOperator

    String label_;
    bool active_=false;
};
