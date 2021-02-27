#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;

namespace ssp {

class VuMeter : public Component {
public:
    VuMeter() : active_(false) { ; }

    void init(const String &label) {
        label_ = label;
    }

    void active(bool);

    bool active() { return active_; }

    void enabled(bool);

    bool enabled() { return enabled_; }

    void paint(Graphics &g);

    void level(float lvl);
    float level()  {return level_;}

private:
    void updateParam(bool newMode = false);


    String label_;
    bool active_ = false;
    bool enabled_ = false;
    float level_ = 0.0f;

    juce_UseDebuggingNewOperator
};


class MonoVuMeter : public Component {
public:
    MonoVuMeter();

    void init(const String &label) {
        label_ = label;
        channel_.init(label);
    }

    void active(bool b) { channel_.active(b); }

    bool active() { return channel_.active(); }

    void enabled(bool b) { channel_.enabled(b); }

    bool enabled() { return channel_.enabled(); }

    void level(float lvl) { channel_.level(lvl); }


    void paint(Graphics &) override;
    void resized() override;
private:
    String label_;
    VuMeter channel_;
    juce_UseDebuggingNewOperator
};


class StereoVuMeter : public Component {
public:
    StereoVuMeter();

    void init(const String &label) {
        label_ = label;
        lChannel_.init(label);
        rChannel_.init("");
    }

    // note left is considered 'lead' channel, for mono purposes
    void active(bool b) {
        lChannel_.active(b);
        rChannel_.active(b);
    }

    bool active() { return lChannel_.active(); }

    void enabled(bool b) {
        lChannel_.enabled(b);
        rChannel_.enabled(b);
    }

    bool enabled() { return lChannel_.enabled(); }

    void level(float l_lvl, float r_lvl) {
        lChannel_.level(l_lvl);
        rChannel_.level(r_lvl);
    }

    void paint(Graphics &) override;
    void resized() override;
private:
    String label_;
    VuMeter lChannel_;
    VuMeter rChannel_;

    juce_UseDebuggingNewOperator
};


}// ssp
