
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Channel.h"
#include "ssp/VuMeter.h"

class MonoChannel : public Component {
public:
    MonoChannel();

    void init(SSPParam *param, const String &label, TrackData *data) {
        vuMeter_.init(label, true);
        data_ = data;
        channel_.init(param, label, data);
    }

    void active(bool b) { vuMeter_.active(b); }

    bool active() { return vuMeter_.active(); }

    void enabled(bool b) { vuMeter_.enabled(b); }

    bool enabled() { return vuMeter_.enabled(); }

    void button(unsigned i, bool b) { channel_.button(i, b); }

    void encbutton(bool b) { channel_.encbutton(b); }

    void encoder(float e) { channel_.encoder(e); }

    bool button(unsigned i) { return channel_.button(i); }

    void buttonMode(Channel::ButMode m) { channel_.buttonMode(m); }

    void encoderMode(Channel::EncMode m) { channel_.encoderMode(m); }


    void paint(Graphics &) override;
    void resized() override;
private:
    ssp::MonoVuMeter vuMeter_;
    Channel channel_;
    TrackData *data_;

    juce_UseDebuggingNewOperator
};
