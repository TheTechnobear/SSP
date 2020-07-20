
#include "SSPChannel.h"

#include <iostream>


inline float rescale(float in, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (in - inMin) / (inMax - inMin) * (outMax - outMin);
}

void SSPChannel::paint(Graphics &g) {
    if (active_ && data_ != nullptr) {
        int h = getHeight();
        int w = getWidth();
        static constexpr int fh = 12;
        int tbh = h - (fh * 3);
        int bw = w / 3;

        float rms = data_->rms_;

        g.setColour(Colours::green);

        float dbMin = -70.0f, dbMax = 6.0f; // db range for meter
        float dbS = rms > 0.0f ? std::log10(rms) * 20.0f : -200.f;
        float db = constrain(dbS, dbMin, dbMax);
        float f = rescale(db, dbMin, dbMax, 0.0f, 1.0f);

        int bh = int (f * float(tbh));
        g.fillRect(bw, fh + (tbh - bh), bw, bh);

        g.setColour(Colours::grey);
        g.drawRect(bw - 1, fh - 1, bw + 2, tbh + 2);

        g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
        String val = String::formatted("%4.2f", dbS);
        g.drawText(label_, 0 , 0 , w, fh, Justification::centred);
        g.drawText(val, 0 , h - fh - 2  , w, fh, Justification::centred);
    }
}


void SSPChannel::buttonA(bool b) {
    switch (butMode_) {
    case BM_MUTESOLO:
        data_->mute_ = b;
        break;
    // case BM_HPF:
    //     break;
    default:
        ;
    }
}


void SSPChannel::buttonB(bool b) {
    switch (butMode_) {
    case BM_MUTESOLO:
        data_->solo_ = b;
        break;
    // case BM_HPF:
    //     break;
    default:
        ;
    }
}

void SSPChannel::encoder(float e) {
    switch (encMode_) {
    case EM_LEVEL:
        break;
    case EM_PAN:
        break;
    case EM_AUX1:
        break;
    case EM_AUX2:
        break;
    case EM_AUX3:
        break;
    default:
        ;
    }
}



void SSPChannel::buttonMode(ButMode m) {
    if (m != butMode_) {
        butMode_ = m;
        // probably no repaint necessary?!
    }
}

void SSPChannel::encoderMode(EncMode m) {
    if (m != encMode_) {
        encMode_ = m;
        // redraw value, to reflect new mode
    }
}
