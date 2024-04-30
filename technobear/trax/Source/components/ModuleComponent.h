#pragma once

#include "../PluginProcessor.h"

class ModuleComponent : public juce::Component {
public:
    ModuleComponent(PluginProcessor &p, int trackIdx, int slotIdx);
    void trackIdx(int t) { trackIdx_ = t; }
    int trackIdx() const { return trackIdx_; }
    void slotIdx(int s) { slotIdx_ = s; }
    int slotIdx() const { return slotIdx_; }

protected:
    void paint(juce::Graphics &g) override;

private:
    PluginProcessor &processor_;
    int trackIdx_;
    int slotIdx_;
};
