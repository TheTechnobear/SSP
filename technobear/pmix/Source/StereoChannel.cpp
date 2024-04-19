
#include "StereoChannel.h"

StereoChannel::StereoChannel() {
    addAndMakeVisible(vuMeter_);
}

inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}

void StereoChannel::paint(Graphics &g) {
    static constexpr int fh = 8 * COMPACT_UI_SCALE;
    int h = getHeight();
    int w = getWidth();

    vuMeter_.level(lData_->rms_.lvl(), rData_->rms_.lvl());

    float gl=normValue(lData_->level[0]);
    float gr=normValue(rData_->level[0]);
    if(rData_->dummy_) gr=gl;
    vuMeter_.gainLevel(gl,gr);

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    int bw = w / 2;
    int bx = w / 4;

    bool mute = lData_->mute.getValue();
    bool solo = lData_->solo.getValue();
    float pan = normValue(lData_->pan);
    float lvl = lData_->rms_.lvl();

    int y = vuMeter_.getHeight();
    y += 5;

    g.setColour(Colours::lightgrey);
    float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
    if (dbS > -100.0f) {
        String val = String::formatted("%4.2f", dbS);
        g.drawText(val, 0, y, w, fh, Justification::centred);
    } else {
        g.drawText("Inf", 0, y, w, fh, Justification::centred);
    }

    y += 2 * fh;

    // pan
    g.setColour(Colours::lightgrey);
    g.drawLine(bx, y, bx + bw, y, 2);
    unsigned poff = unsigned(float(bw) * ((pan + 1.0) / 2.0));
    g.setColour(Colours::white);
    g.fillRect(bx + poff - 5, y - 5, 10, 10);

    // solo
    y += 2 * fh;
    g.setColour(Colours::lightgrey);
    g.drawRect(bx - 1, y - 1, bw + 2, fh + 2);
    if (solo) {
        g.setColour(Colours::yellow);
        g.fillRect(bx, y, bw, fh);
        g.setColour(Colours::black);
    }
    g.drawText("S", bx, y, bw, fh, Justification::centred);

    // mute
    y += 2 * fh;
    g.setColour(Colours::lightgrey);
    g.drawRect(bx - 1, y - 1, bw + 2, fh + 2);
    if (mute) {
        g.setColour(Colours::red);
        g.fillRect(bx, y, bw, fh);
        g.setColour(Colours::black);
    }
    g.drawText("M", bx, y, bw, fh, Justification::centred);
}

void StereoChannel::resized() {
    static constexpr int fh = 16;
    int w = getWidth() / 4;
    vuMeter_.setBounds(w, 0, 2 * w, getHeight() - (8 * fh));
}
