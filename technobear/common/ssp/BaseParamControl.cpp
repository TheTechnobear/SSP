#include "BaseParamControl.h"

using namespace juce;
namespace ssp {

BaseParamControl::BaseParamControl(Parameter &p)
    : attachment_(p, [this](float f) { paramChanged(f); }),
      param_(p),
      active_(false) {
}

void BaseParamControl::paramChanged(float) {
    repaint();
}


SimpleParamControl::SimpleParamControl(Parameter &p, float coarse, float fine)
    : BaseParamControl(p) {
    coarseInc_ = p.convertTo0to1(p.getNormalisableRange().start + coarse);
    fineInc_ = p.convertTo0to1(p.getNormalisableRange().start + fine);
}


/// SimpleParamControl

void SimpleParamControl::inc(bool fine) {
    auto &p = param_;
    p.beginChangeGesture();
    float inc = fine ? fineInc_ : coarseInc_;
    float v = p.getValue();
    float nv = v + (inc != 0 ? inc : 0.01f);
    p.setValueNotifyingHost(nv);
    p.endChangeGesture();
}

void SimpleParamControl::dec(bool fine) {
    auto &p = param_;
    p.beginChangeGesture();
    float inc = fine ? fineInc_ : coarseInc_;
    float v = p.getValue();
    float nv = v - (inc != 0 ? inc : 0.01f);
    p.setValueNotifyingHost(nv);
    p.endChangeGesture();
}

void SimpleParamControl::reset() {
    auto &p = param_;
    p.beginChangeGesture();
    float v = p.getDefaultValue();
    p.setValueNotifyingHost(v);
    p.endChangeGesture();
}

void SimpleParamControl::paint(Graphics &g) {
    static constexpr unsigned fh = 36;
    unsigned h = getHeight();
    unsigned w = getWidth();

    auto &p = param_;

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(fg_);
    g.drawText(p.getName(32), 0, 0, w, fh, Justification::centred);

    String val = p.getCurrentValueAsText();
    if (p.getLabel().length()) { val = val + " " + p.getLabel(); }
    g.setColour(Colours::white);
    g.drawText(val, 0, h / 2, w, fh, Justification::centred);
}

LineParamControl::LineParamControl(Parameter &p, float coarse, float fine)
    : SimpleParamControl(p, coarse, fine) {

};

void LineParamControl::paint(Graphics &g) {
    static constexpr unsigned fh = 32;
    unsigned h = getHeight();
    unsigned w = getWidth();

    auto &p = param_;

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(active() ? fg_ : Colours::grey);
    g.drawText(p.getName(32), 0, 0, w, fh, Justification::centred);

    String val = p.getCurrentValueAsText();
    if (p.getLabel().length()) { val = val + " " + p.getLabel(); }
    g.setColour(active() ? Colours::white : Colours::grey);
    g.drawText(val, 0, h / 2, w, fh, Justification::centred);
}


BarParamControl::BarParamControl(Parameter &p, float coarse, float fine)
    : SimpleParamControl(p, coarse, fine) {

};

void BarParamControl::paint(Graphics &g) {
    static constexpr unsigned fh = 28;
    unsigned h = getHeight();
    unsigned w = getWidth();

    auto &p = param_;

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(active() ? fg_ : Colours::grey);
    g.drawText(p.getName(32), 0, 0, w, fh, Justification::centred);

    g.setColour(Colours::black);
    g.drawRect(0, h / 2, w, fh); // border

    g.setColour(Colour(0xFF222222));
    g.fillRect(1, (h / 2) + 1, w - 2, fh - 2); // border

    g.setColour(active() ? Colours::darkcyan : Colours::grey);

    if (!p.isDiscrete()) {
        int be = (w - 4) * p.getValue();
        if (p.getNormalisableRange().start >= 0.0f) {
            g.fillRect(2, h / 2 + 2, be, fh - 4);
        } else {
            float v = p.getValue() - 0.5f;
            int bl = (w - 4) * v;
            int bs = (bl > 0.0f ? 0.5f : 0.5 + v) * (w - 4);
            g.fillRect(bs, h / 2 + 2, (bl > 0.0f ? bl : -bl), fh - 4);
        }
        g.setColour(Colours::white);
        g.drawVerticalLine(be + 2, h / 2 + 2, (h / 2 + 2) + fh - 4);
    } else {
        int bl = ((w - 4) / p.getNumSteps());
        int s = p.getNumSteps() * (p.getValue() - 0.001);
        int bs = (s * bl) + 2;
        g.fillRoundedRectangle(bs, h / 2 + 2, bl, fh - 4, 10.0f);
    }


    if (active()) {
        g.setColour(active() ? Colours::white : Colours::lightgrey);
        String val = p.getCurrentValueAsText();
        if (p.getLabel().length()) { val = val + " " + p.getLabel(); }
        int vfh = fh - 10;
        g.drawText(val, 2, h - vfh - 10, w - 4, vfh, Justification::centred);
    }
}


} //namespace ssp
