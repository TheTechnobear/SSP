#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class SSPParam : public Component {
public:
    SSPParam() { ; }

    void label(const char *) { ; }

    void value(float) { ; }
};

class Channel {
public:
    Channel() : data_(nullptr),  param_(nullptr) { ; }

    void init(SSPParam *param, const String &label, TrackData *data) {
        param_ = param;
        data_ = data;
    }

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
    TrackData *data_ = nullptr;
    SSPParam *param_ = nullptr;

    juce_UseDebuggingNewOperator
};
