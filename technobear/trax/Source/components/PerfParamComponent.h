#pragma once

#include "../PluginProcessor.h"

class PerfParamComponent : public juce::Component {
public:
    PerfParamComponent(PluginProcessor &p, PluginProcessor::PerformanceParam& param);

    void onSSPTimer();
    void onEncoder(int enc,float v);
    void onEncoderSwitch(int enc, bool v);
protected:
    void paint(juce::Graphics &g) override;
    
private:
    PluginProcessor &processor_;
    PluginProcessor::PerformanceParam& param_;
};