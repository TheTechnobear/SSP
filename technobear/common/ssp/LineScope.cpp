#include "LineScope.h"

namespace ssp {

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

void LineScope::paint(Graphics &g) {
    float w = (float) getWidth();
    float h = (float) getHeight();

    juce::Colour border_clr = juce::Colours::grey;

    if (border_) {
        Font f(Font::getDefaultMonospacedFontName(), 16.f, Font::plain);
        g.setFont(f);
        g.setColour(border_clr);
        g.drawRect(0.0f, 0.0f, w, h);
        if (!title_.empty()) g.drawText(title_, 0, 16, w, 24, Justification::centred);
    }


    // draw scope
    g.setColour(colour_);
    float val = 0.00f, y = 0.0f, x = 0.0f;
    float lastX = 0.0f, lastY = 0.5f * h;
    float stepW = (float) w / (float) n_;
    for (int t = 0; t < n_; t++) {
        val = constrain(buffer_[t] * scale_ + offset_, -1.0f, 1.0f);

        y = (1.0f - (val + 1.0f) * 0.5f) * h;
        x = t * stepW;

        g.drawLine(lastX, lastY, x, y, 2.0f);
        lastX = x;
        lastY = y;

    }
}

}
