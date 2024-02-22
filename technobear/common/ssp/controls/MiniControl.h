#pragma once

#include "ParamControl.h"

namespace ssp {
// this is very similar to BarParamControl but more compact
class MiniControl : public ssp::SimpleParamControl {
public:
    MiniControl(Parameter &p, float coarse = 1.0f, float fine = 0.01f, juce::Colour fg = juce::Colours::red);
protected:
    void paint(juce::Graphics &g) override;
    juce_UseDebuggingNewOperator

    static constexpr unsigned scale = COMPACT_UI_SCALE;
};

}
