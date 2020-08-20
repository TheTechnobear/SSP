
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SSPParam.h"

class SSPChannel: public Component
{
public:
    SSPChannel() : data_(nullptr), active_(false), param_(nullptr) {;}
    void init(SSPParam* param, const String& label, TrackData* data)  {
        param_ = param;
        label_ = label;
        data_ = data;
    }

    void active(bool);
    bool active() { return active_;}

    void button(unsigned i, bool b);
    void encbutton(bool b);
    void encoder(float e);

    bool button(unsigned i);

    enum EncMode {
        EM_LEVEL,
        EM_PAN,
        EM_AUX1,
        EM_AUX2,
        EM_AUX3,
        EM_GAIN,
        EM_MAX
    };

    enum ButMode {
        BM_SOLOMUTE,
        BM_CUEAC,
        BM_MAX
    };

    void buttonMode(ButMode m);
    void encoderMode(EncMode m);


    void paint(Graphics &g);
private:
    void updateParam(bool newMode = false);


    EncMode encMode_ = EM_LEVEL;
    ButMode butMode_ = BM_SOLOMUTE;
    String label_;
    TrackData* data_ = nullptr;
    bool active_ = false;
    SSPParam* param_ = nullptr;

    juce_UseDebuggingNewOperator
};
