
#pragma once

#include "PluginProcessor.h"

class SeqCellComponent : public juce::Component {
public:
    using Parameter = juce::RangedAudioParameter;

    SeqCellComponent(PluginProcessor::PluginParams &params, unsigned step);
    void activeStep(unsigned xP, unsigned yP, unsigned cP);

    void paint(Graphics &g);

private:
    static constexpr unsigned L_CLRS = 3;
    static juce::Colour LAYER_COLOURS[L_CLRS];

    unsigned step_;
    bool activeX_, activeY_, activeC_;
    PluginProcessor::PluginParams &params_;
};
