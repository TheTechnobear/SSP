#include "VuMeter.h"


#include <iostream>

namespace ssp {

constexpr inline float rescale(float in, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (in - inMin) / (inMax - inMin) * (outMax - outMin);
}

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

void VuMeter::paint(Graphics &g) {
    float lvl = level();
    bool mute = false;

    static constexpr float dbMin = -70.0f, dbMax = 6.0f; // db range for meter
    static constexpr float dbRed = 0.0f, dbYellow = -6.0f;
    static constexpr float lvlRed = rescale(dbRed, dbMin, dbMax, 0.0f, 1.0f);
    static constexpr float lvlYellow = rescale(dbYellow, dbMin, dbMax, 0.0f, 1.0f);

    static constexpr int fh = 16;
    int h = getHeight();
    int w = getWidth();
    int tbh = h - (fh * 2);
    int barbase = tbh + (2 * fh);
    int bw = w - 10;
    int bx = 5;


    float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
    float db = constrain(dbS, dbMin, dbMax);
    float f = rescale(db, dbMin, dbMax, 0.0f, 1.0f);

    g.setColour(Colours::darkgrey);
    int bl = tbh + 2;
    g.fillRect(bx - 1, barbase - bl, bw + 2, bl);


    int bh = int(f * float(tbh));
    int ypos = int(lvlYellow * float(tbh));
    int rpos = int(lvlRed * float(tbh));

    g.setColour(mute ? Colours::lightgrey : Colours::green);
    int gb = barbase;
    int gh = constrain(bh, 0, ypos - 1);
    bl = gh;
    g.fillRect(bx, gb - bl, bw, bl);

    if (bh > ypos) {
        g.setColour(mute ? Colours::lightgrey : Colours::yellow);
        int yh = constrain(bh, 0, rpos - 1);
        bl = yh - ypos;
        g.fillRect(bx, barbase - (ypos + bl), bw, bl);
        if (bh > rpos) {
            g.setColour(mute ? Colours::lightgrey : Colours::red);
            int rh = constrain(bh, 0, tbh);
            bl = rh - rpos;
            g.fillRect(bx, barbase - (rpos + bl), bw, bl);
        }
    }


    // 0db line
    g.setColour(Colours::darkgrey);
    g.fillRect(bx - 5, barbase - rpos - 2 , bw + 10, 2);

    if (drawGainLevel_) {
        float lvlSlider = gainLevel_;
        dbS = lvlSlider > 0.0f ? std::log10(lvlSlider) * 20.0f : -200.f;
        db = constrain(dbS, dbMin, dbMax);
        f = rescale(db, dbMin, dbMax, 0.0f, 1.0f);
        g.setColour(Colours::white);
        g.fillRoundedRectangle(bx - 5, barbase - (f * tbh) - 3, bw + 10, 5, 5);
    }
}

void VuMeter::active(bool a) {
    if (active_ != a) {
        active_ = a;
        repaint();
    }
}

void VuMeter::enabled(bool e) {
    if (enabled_ != e) {
        enabled_ = e;
        if (active()) repaint();
    }
}

void VuMeter::level(float lvl) {
    if (level_ != lvl) {
        level_ = lvl;
        if (active()) repaint();
    }
};

void VuMeter::gainLevel(float lvl) {
    if (gainLevel_ != lvl) {
        gainLevel_ = lvl;
        if (active()) repaint();
    }
}


MonoVuMeter::MonoVuMeter() {
    addAndMakeVisible(channel_);
}

void MonoVuMeter::paint(Graphics &g) {
    static constexpr int fh = 16;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    if (enabled()) {
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::lightgrey);
    }
    g.drawText(label_, 0, 0, w, fh, Justification::centred);

    // int tbh = h - (fh * 4);
    // int barbase = tbh + (2 * fh);
    // int bw = w / 2;
    // int bx = w / 4;

    // int y = barbase;
    // y += 5;

    // float lvl = channel_.level();

    // g.setColour(Colours::lightgrey);
    // float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
    // if (dbS > -100.0f) {
    //     String val = String::formatted("%4.2f", dbS);
    //     g.drawText(val, 0, y, w, fh, Justification::centred);
    // } else {
    //     g.drawText("Inf", 0, y, w, fh, Justification::centred);
    // }
}

void MonoVuMeter::resized() {
    int w = getWidth();
    int bw = w / 2;
    int bx = w / 4;
    channel_.setBounds(bx, 0, bw, getHeight());
}


StereoVuMeter::StereoVuMeter() {
    addAndMakeVisible(lChannel_);
    addAndMakeVisible(rChannel_);
}

void StereoVuMeter::paint(Graphics &g) {
    static constexpr int fh = 16;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    if (enabled()) {
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::lightgrey);
    }
    g.drawText(label_, 0, 0, w, fh, Justification::centred);

    // int tbh = h - (fh * 4);
    // int barbase = tbh + (2 * fh);
    // int bw = w / 2;
    // int bx = w / 4;

    // int y = barbase;
    // y += 5;

    // float lvl = lChannel_.level();
    // g.setColour(Colours::lightgrey);
    // float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
    // if (dbS > -100.0f) {
    //     String val = String::formatted("%4.2f", dbS);
    //     g.drawText(val, 0, y, w, fh, Justification::centred);
    // } else {
    //     g.drawText("Inf", 0, y, w, fh, Justification::centred);
    // }
}

void StereoVuMeter::resized() {
    int w = getWidth();
    int sp = 0;
    int bw = (w - sp) / 2;
    // int bx = w / 4;
    int bx = 0;
    lChannel_.setBounds(bx, 0, bw, getHeight());
    rChannel_.setBounds(bx + bw + sp, 0, bw, getHeight());
}


} // namespace

