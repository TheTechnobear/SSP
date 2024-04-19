#include "ParamControl.h"

namespace ssp {

BaseParamControl::BaseParamControl(Parameter &p, juce::Colour fg)
    : attachment_(p, [this](float f) { paramChanged(f); }), param_(p), active_(false), fg_(fg) {
}

void BaseParamControl::paramChanged(float) {
    repaint();
}

juce::String BaseParamControl::getTextValue() {
    auto &p = param_;
    auto val = p.getCurrentValueAsText();
    if (p.getLabel().length()) { val = val + " " + p.getLabel(); }
    return val;
}


void BaseParamControl::mouseDoubleClick(const juce::MouseEvent &event) {
    reset();
}

void BaseParamControl::mouseDown(const juce::MouseEvent &event) {
    lastMouseX_ = event.getScreenX();
}


void BaseParamControl::mouseDrag(const juce::MouseEvent &event) {
    const int sen = 1;
    auto x = event.getScreenX();
    auto delta = x - lastMouseX_;
    if (delta > sen) {
        inc(false);
        lastMouseX_ = x;
    } else if (delta < -sen) {
        dec(false);
        lastMouseX_ = x;
    }
}

/// SimpleParamControl

SimpleParamControl::SimpleParamControl(Parameter &p, float coarse, float fine, juce::Colour fg)
    : BaseParamControl(p, fg) {
    coarseInc_ = p.convertTo0to1(p.getNormalisableRange().start + coarse);
    fineInc_ = p.convertTo0to1(p.getNormalisableRange().start + fine);
}


void SimpleParamControl::inc(bool fine) {
    auto &p = param_;
    p.beginChangeGesture();
    float inc = fine ? fineInc_ : coarseInc_;
    float v = p.getValue();
    float nv = v + (inc != 0 ? inc : 0.01f);
    nv = std::min(nv, 1.0f);
    p.setValueNotifyingHost(nv);
    p.endChangeGesture();
}

void SimpleParamControl::dec(bool fine) {
    auto &p = param_;
    p.beginChangeGesture();
    float inc = fine ? fineInc_ : coarseInc_;
    float v = p.getValue();
    float nv = v - (inc != 0 ? inc : 0.01f);
    nv = std::max(nv, 0.0f);
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

void SimpleParamControl::paint(juce::Graphics &g) {
    static constexpr unsigned fh = 18 * COMPACT_UI_SCALE;
    int h = getHeight();
    int w = getWidth();

    auto &p = param_;

    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain));

    g.setColour(fg_);
    g.drawText(p.getName(32), 0, 0, w, fh, juce::Justification::centred);

    g.setColour(juce::Colours::white);
    auto val = getTextValue();
    g.drawText(val, 0, h / 2, w, fh, juce::Justification::centred);
}

LineParamControl::LineParamControl(Parameter &p, float coarse, float fine, juce::Colour fg)
    : SimpleParamControl(p, coarse, fine, fg){

      };

void LineParamControl::paint(juce::Graphics &g) {
    static constexpr unsigned fh = 16 * COMPACT_UI_SCALE;
    int h = getHeight();
    int w = getWidth();

    auto &p = param_;

    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain));

    g.setColour(active() ? fg_ : juce::Colours::grey);
    g.drawText(p.getName(32), 0, 0, w, fh, juce::Justification::centred);

    g.setColour(active() ? juce::Colours::white : juce::Colours::grey);
    auto val = getTextValue();
    g.drawText(val, 0, h / 2, w, fh, juce::Justification::centred);
}


BarParamControl::BarParamControl(Parameter &p, float coarse, float fine, juce::Colour fg)
    : SimpleParamControl(p, coarse, fine, fg){

      };

void BarParamControl::paint(juce::Graphics &g) {
    static constexpr unsigned fh = 14 * COMPACT_UI_SCALE;
    int h = getHeight();
    int w = getWidth();

    auto &p = param_;

    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain));

    g.setColour(active() ? fg_ : juce::Colours::grey);
    g.drawText(p.getName(32), 0, 0, w, fh, juce::Justification::centred);

    g.setColour(juce::Colours::black);
    g.drawRect(0, h / 2, w, fh);  // border

    g.setColour(juce::Colour(0xFF222222));
    g.fillRect(1, (h / 2) + 1, w - 2, fh - 2);  // border

    g.setColour(active() ? juce::Colours::darkcyan : juce::Colours::grey);

    if (!p.isDiscrete()) {
        int be = (w - 4) * p.getValue();
        bool bipolar = p.getNormalisableRange().start < 0.0f && p.getNormalisableRange().end > 0.0f;
        if (!bipolar) {
            g.fillRect(2, h / 2 + 2, be, fh - 4);
        } else {
            float v = p.getValue() - 0.5f;
            int bl = (w - 4) * v;
            int bs = (bl > 0.0f ? 0.5f : 0.5 + v) * (w - 4);
            g.fillRect(bs, int(h / 2) + 2, (bl > 0 ? bl : -bl), fh - 4);
        }
        g.setColour(juce::Colours::white);
        g.drawVerticalLine(be + 2, int(h / 2) + 2, int(h / 2) + 2 + fh - 4);
    } else {
        int bl = ((w - 4) / p.getNumSteps());
        int s = p.getNumSteps() * (p.getValue() - 0.001);
        int bs = (s * bl) + 2;
        g.fillRoundedRectangle(bs, int(h / 2) + 2, bl, fh - 4, 10.0f);
    }


    if (active()) {
        g.setColour(active() ? juce::Colours::white : juce::Colours::lightgrey);
        auto val = getTextValue();
        g.drawText(val, 1, (h / 2) + 1, w - 2, fh - 2, juce::Justification::centred);
    }
}


}  // namespace ssp
