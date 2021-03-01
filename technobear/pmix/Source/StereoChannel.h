
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Channel.h"
#include "ssp/VuMeter.h"


class StereoChannel : public Component {
public:
    StereoChannel();

    void init(SSPParam *param, const String &label, TrackData *lData, TrackData *rData) {
        vuMeter_.init(label, true);
        lData_ = lData;
        rData_ = rData;
        lChannel_.init(param, label, lData);
        rChannel_.init(param, "", rData_);
    }


    void active(bool b) { vuMeter_.active(b); }

    bool active() { return vuMeter_.active(); }

    void enabled(bool b) { vuMeter_.enabled(b); }

    bool enabled() { return vuMeter_.enabled(); }

    void button(unsigned i, bool b) {
        lChannel_.button(i, b);
        rChannel_.button(i, b);
    }

    void encbutton(bool b) {
        lChannel_.encbutton(b);
        rChannel_.encbutton(b);
    }

    void encoder(float e) {
        lChannel_.encoder(e);
        rChannel_.encoder(e);
    }

    bool button(unsigned i) { return lChannel_.button(i); }

    void buttonMode(Channel::ButMode m) {
        lChannel_.buttonMode(m);
        rChannel_.buttonMode(m);
    }

    void encoderMode(Channel::EncMode m) {
        lChannel_.encoderMode(m);
        rChannel_.encoderMode(m);
    }


    void paint(Graphics &) override;
    void resized() override;
private:
    ssp::StereoVuMeter vuMeter_;

    Channel lChannel_;
    Channel rChannel_;
    TrackData *lData_;
    TrackData *rData_;

    juce_UseDebuggingNewOperator
};
