
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SSPChannel.h"

class SSPStereoChannel: public Component
{
public:
    SSPStereoChannel();
    void init(SSPParam* param, const String& label, TrackData* lData, TrackData* rData)  {
        label_=label;
        lData_=lData;
        rData_=rData;
        lChannel_.init(param, label, lData);
        rChannel_.init(param, "", rData_);
    }

    // note left is considered 'lead' channel, for mono purposes
    void active(bool b ) { lChannel_.active(b); rChannel_.active(b);}
    bool active() { return lChannel_.active();}

    void button(unsigned i, bool b) { lChannel_.button(i, b); rChannel_.button(i, b);}
    void encbutton(bool b) { lChannel_.encbutton(b); rChannel_.encbutton(b);}
    void encoder(float e) { lChannel_.encoder(e); rChannel_.encoder(e);}

    bool button(unsigned i) { return lChannel_.button(i);}

    void buttonMode(SSPChannel::ButMode m) { lChannel_.buttonMode(m); rChannel_.buttonMode(m);}
    void encoderMode(SSPChannel::EncMode m) { lChannel_.encoderMode(m); rChannel_.encoderMode(m);}


    void paint (Graphics&) override;
    void resized() override;
private:
    String     label_;
    SSPChannel lChannel_;
    SSPChannel rChannel_;
    TrackData* lData_;
    TrackData* rData_;

    juce_UseDebuggingNewOperator
};
