#include "ParamButton.h"


namespace ssp {

/// ButtonControl

ParamButton::ParamButton(Parameter &p, unsigned fh, const juce::Colour fg, const juce::Colour bg, bool momentary)
    : attachment_(p, [this](float f) { paramChanged(f); }), param_(p),
      fh_(fh), fg_(fg), bg_(bg),
      momentary_(momentary) {
}

void ParamButton::paramChanged(float) {
    repaint();
}


void ParamButton::onUp() {
    auto &p = param_;
    bool v = p.getValue() > 0.5f;
    if (momentary_) {
        v = !v;
    } else {
        v = 0.0f;
    }
//    std::cerr << "ParamButton onUp " <<  v << std::endl;
    p.beginChangeGesture();
    p.setValueNotifyingHost(v ? 1.0f : 0.0f);
    p.endChangeGesture();
}

void ParamButton::onDown() {
    auto &p = param_;
    if (momentary_) {
        // toggle on up
        return;
    } else {
        p.beginChangeGesture();
        p.setValueNotifyingHost(1.0f);
        p.endChangeGesture();
    }
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

}