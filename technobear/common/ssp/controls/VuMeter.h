#pragma once


#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

namespace ssp {

class VuMeter : public juce::Component {
public:
    VuMeter() : active_(false) { ; }

    void init(const juce::String &label, bool drawGainLevel=false) {
        label_ = label;
        drawGainLevel_=drawGainLevel;
    }

    void active(bool);

    bool active() { return active_; }

    void enabled(bool);

    bool enabled() { return enabled_; }

    void paint(juce::Graphics &g);

    void level(float lvl);
    float level()  {return level_;}

    void gainLevel(float lvl);
    float gainLevel()  {return gainLevel_;}

private:
    String label_;
    bool active_ = false;
    bool enabled_ = false;
    float level_ = 0.0f;
    float gainLevel_=0.0f;
    bool drawGainLevel_=false;

    juce_UseDebuggingNewOperator
};


class MonoVuMeter : public juce::Component {
public:
    MonoVuMeter();

    void init(const juce::String &label,bool drawGainLevel=false) {
        label_ = label;
        channel_.init(label,drawGainLevel);
    }

    void active(bool b) { channel_.active(b); }

    bool active() { return channel_.active(); }

    void enabled(bool b) { channel_.enabled(b); }

    bool enabled() { return channel_.enabled(); }

    void level(float lvl) { channel_.level(lvl); }
    void gainLevel(float lvl) {channel_.gainLevel(lvl);}

    void paint(juce::Graphics &) override;
    void resized() override;
private:
    String label_;
    VuMeter channel_;
    juce_UseDebuggingNewOperator
};


class StereoVuMeter : public juce::Component {
public:
    StereoVuMeter();

    void init(const juce::String &label,bool drawGainLevel=false) {
        label_ = label;
        lChannel_.init(label,drawGainLevel);
        rChannel_.init("",drawGainLevel);
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

    void gainLevel(float l_lvl, float r_lvl) {
        lChannel_.gainLevel(l_lvl);
        rChannel_.gainLevel(r_lvl);
    }

    void paint(juce::Graphics &) override;
    void resized() override;
private:
    juce::String label_;
    VuMeter lChannel_;
    VuMeter rChannel_;

    juce_UseDebuggingNewOperator
};


}// ssp
