#include "MiniControl.h"

namespace ssp {

MiniControl::MiniControl(Parameter &p, float coarse, float fine, juce::Colour fg)
    : SimpleParamControl(p, coarse, fine, fg) {

};

void MiniControl::paint(juce::Graphics &g) {
    int h = getHeight();
    int w = getWidth();
    unsigned tfh = h / 4;
    unsigned vfh = h / 3;

    unsigned gap = scale;

    auto &p = param_;
    const auto &titleFont = juce::Font(juce::Font::getDefaultMonospacedFontName(), tfh, juce::Font::plain);
    const auto &valueFont = juce::Font(juce::Font::getDefaultMonospacedFontName(), vfh, juce::Font::plain);

    g.setFont(titleFont);
    g.setColour(active() ? fg_ : juce::Colours::grey);
    g.drawText(p.getName(32), 0, scale, w, tfh, juce::Justification::left);


    unsigned bposY = h / 3 + 1;
    unsigned bH = (h * 2) / 3;

    g.setColour(active() ? juce::Colours::darkcyan : juce::Colours::grey);

    if (!p.isDiscrete()) {
        int ve = (w - 1) * p.getValue();
        bool bipolar = p.getNormalisableRange().start < 0.0f && p.getNormalisableRange().end > 0.0f;

        if (!bipolar) {
            g.fillRect(0, bposY, ve, bH);
        } else {
            float v = p.getValue() - 0.5f;
            int bl = (w - 1) * v;
            int bs = (v > 0.0f ? w / 2 : (w / 2) + bl);
            int be = bs + bl;
            g.fillRect(bs, bposY, (bl > 0 ? bl : -bl), bH);
        }
        g.setColour(juce::Colours::white);
        g.drawVerticalLine(ve, bposY, bposY + bH);

    } else {
        int bl = (w - 1) / p.getNumSteps();
        int s = p.getNumSteps() * (p.getValue() - 0.001);
        int bs = s * bl;
        g.fillRoundedRectangle(bs, bposY, bl, bH, 5.0f * scale);
    }


    if (active()) {
        g.setColour(active() ? juce::Colours::white : juce::Colours::lightgrey);
        g.setFont(valueFont);
        auto val = getTextValue();
        g.drawText(val, 1, bposY + (vfh / 2), w - 2, bposY - 2, juce::Justification::centred);
    }
}

}