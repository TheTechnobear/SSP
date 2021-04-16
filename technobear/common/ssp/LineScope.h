#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace ssp {

class LineScope : public Component {
public:
    LineScope() {
    }

    void init(const std::string &title, float *buf, unsigned n, Colour c = juce::Colours::red, bool border = false) {
        title_ = title;
        buffer_ = buf;
        n_ = n;
        colour_ = c;
        border_ = border;
    }

    void scaleOffset(float s, float o) {
        scale_ = s;
        offset_ = o;
    }

    void paint(Graphics &g) override;

private:
    juce_UseDebuggingNewOperator
    std::string title_;

    float *buffer_;
    unsigned n_;
    juce::Colour colour_ = juce::Colours::red;
    bool border_ = false;

    float scale_ = 1.0f;
    float offset_ = 0.0f;
};

}