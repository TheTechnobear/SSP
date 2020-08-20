
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SSPChannel.h"

class SSPMonoChannel: public Component
{
public:
    SSPMonoChannel();
    void init(SSPParam* param, const String& label, TrackData* data)  {
        label_=label;
        data_=data;
        channel_.init(param, label, data);
    }

    void active(bool b ) { channel_.active(b);}
    bool active() { return channel_.active();}

    void button(unsigned i, bool b) { channel_.button(i, b);}
    void encbutton(bool b) { channel_.encbutton(b);}
    void encoder(float e) { channel_.encoder(e);}

    bool button(unsigned i) { return channel_.button(i);}

    void buttonMode(SSPChannel::ButMode m) { channel_.buttonMode(m);}
    void encoderMode(SSPChannel::EncMode m) { channel_.encoderMode(m);}


    void paint (Graphics&) override;
    void resized() override;
private:
    String     label_;
    SSPChannel channel_;
    TrackData* data_;

    juce_UseDebuggingNewOperator
};
