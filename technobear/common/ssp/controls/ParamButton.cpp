#include "ParamButton.h"


namespace ssp {

/// ButtonControl

ParamButton::ParamButton(Parameter &p, unsigned fh, const juce::Colour fg, const juce::Colour bg, bool momentary)
    : attachment_(p, [this](float f) { paramChanged(f); }),
      param_(p),
      fh_(fh),
      fg_(fg),
      bg_(bg),
      momentary_(momentary) {
}

void ParamButton::paramChanged(float) {
    repaint();
}

void ParamButton::mouseUp(const juce::MouseEvent &event) {
    onUp();
}

void ParamButton::mouseDown(const juce::MouseEvent &event) {
    onDown();
}


void ParamButton::onUp() {
    if(!momentary_) return;

    auto &p = param_;
    p.beginChangeGesture();
    p.setValueNotifyingHost(0.f);
    p.endChangeGesture();
}

void ParamButton::onDown() {
    if(!momentary_) return;

    auto &p = param_;
    p.beginChangeGesture();
    p.setValueNotifyingHost(1.f);
    p.endChangeGesture();
}


void ParamButton::onClick() {
    if(momentary_) return;

    // toggle - on event action
    auto &p = param_;
    bool v = p.getValue() > 0.5f;
    p.beginChangeGesture();
    p.setValueNotifyingHost(!v);
    p.endChangeGesture();
}


void ParamButton::paint(juce::Graphics &g) {
    const int w = getWidth();
    const int h = getHeight();
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh_, juce::Font::plain));

    auto &p = param_;
    bool active = p.getValue() > 0.5f;

    if (!active) {
        g.setColour(bg_);
        g.fillRect(0, 0 + 1, w - 2, h - 2);
        g.setColour(fg_);
    } else {
        g.setColour(fg_);
        g.fillRect(0, 0 + 1, w - 2, h - 2);
        g.setColour(bg_);
    }

    g.drawText(p.getName(32), 0, 0, w, h, juce::Justification::centred);
}

}  // namespace ssp