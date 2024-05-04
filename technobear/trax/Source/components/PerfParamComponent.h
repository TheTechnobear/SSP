#pragma once

#include "../PluginProcessor.h"

class PerfParamComponent : public juce::Component {
public:
    PerfParamComponent(PluginProcessor &p, PluginProcessor::PerformanceParam &param,
                       juce::Colour fg = juce::Colours::red);

    void onSSPTimer();
    void onEncoder(float v);
    void onEncoderSwitch(bool v);
    void active(bool v) { active_ = v; }
    bool active() const { return active_; }

protected:
    void paint(juce::Graphics &g) override;
    void sendToPlugin(float V);

private:
    void inc(bool fine);
    void dec(bool fine);
    void reset();
    void set(float v);

    float coarseInc_ = 0.1f;
    float fineInc_ = 0.01f;
    bool encState_ = false;
    bool encReset = false;
    bool active_ = false;
    juce::Colour fg_ = juce::Colours::red;
    PluginProcessor &processor_;
    PluginProcessor::PerformanceParam &param_;
};