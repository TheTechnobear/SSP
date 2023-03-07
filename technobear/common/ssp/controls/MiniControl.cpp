#include "MiniControl.h"

using namespace juce;

namespace ssp {

MiniControl::MiniControl(Parameter &p, float coarse, float fine, juce::Colour fg)
    : SimpleParamControl(p, coarse, fine, fg) {

};

void MiniControl::paint(Graphics &g) {
    int h = getHeight();
    int w = getWidth();
    unsigned tfh = h / 4;
    unsigned vfh = h / 3;

    unsigned gap = scale;

    auto &p = param_;
    const auto &titleFont = Font(Font::getDefaultMonospacedFontName(), tfh, Font::plain);
    const auto &valueFont = Font(Font::getDefaultMonospacedFontName(), vfh, Font::plain);

    g.setFont(titleFont);
    g.setColour(active() ? fg_ : Colours::grey);
    g.drawText(p.getName(32), 0, scale, w, tfh, Justification::left);


    unsigned bposY = h / 3 + 1;
    unsigned bH = (h * 2) / 3;

    g.setColour(active() ? Colours::darkcyan : Colours::grey);

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
        g.setColour(Colours::white);
        g.drawVerticalLine(ve, bposY, bposY + bH);

    } else {
        int bl = (w - 1) / p.getNumSteps();
        int s = p.getNumSteps() * (p.getValue() - 0.001);
        int bs = s * bl;
        g.fillRoundedRectangle(bs, bposY, bl, bH, 5.0f * scale);
    }


    if (active()) {
        g.setColour(active() ? Colours::white : Colours::lightgrey);
        g.setFont(valueFont);
        String val = getTextValue();
        g.drawText(val, 1, bposY + (vfh / 2), w - 2, bposY - 2, Justification::centred);
    }
}

}