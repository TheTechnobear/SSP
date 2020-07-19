
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class SSPChannel: public Component
{
public:
    SSPChannel() : data_(nullptr),active_(false) {;}
    void init(const String& label, TrackData* data)  {
        label_=label;
        data_=data;
    }

    void active(bool a) {
        if(active_!=a) repaint();
        active_=a;
    }
    bool active() { return active_;}

    void buttonA(bool b);
    void buttonB(bool b);
    void encoder(float e);

    enum EncMode {
        EM_LEVEL,
        EM_PAN,
        EM_AUX1,
        EM_AUX2,
        EM_AUX3,
        EM_MAX
    };

    enum ButMode {
        BM_MUTESOLO,
        // BM_HPF,
        BM_MAX
    };

    void buttonMode(ButMode m);
    void encoderMode(EncMode m); 

private:

    EncMode encMode_=EM_LEVEL;
    ButMode butMode_=BM_MUTESOLO;
    String label_;
    TrackData* data_=nullptr;
    bool active_=false;

    void paint(Graphics &g);
    juce_UseDebuggingNewOperator
};
