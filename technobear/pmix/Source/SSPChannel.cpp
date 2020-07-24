
#include "SSPChannel.h"

#include <iostream>


constexpr inline float rescale(float in, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (in - inMin) / (inMax - inMin) * (outMax - outMin);
}

void SSPChannel::paint(Graphics &g) {
    if (active_ && data_ != nullptr) {
        static constexpr float dbMin        = -70.0f, dbMax = 6.0f; // db range for meter
        static constexpr float dbRed        = 0.0f, dbYellow = -6.0f;
        static constexpr float lvlRed       = rescale(dbRed, dbMin, dbMax, 0.0f, 1.0f);
        static constexpr float lvlYellow    = rescale(dbYellow, dbMin, dbMax, 0.0f, 1.0f);

        static constexpr int fh = 12;
        int h = getHeight();
        int w = getWidth();
        int tbh = h - (fh * 10);
        int barbase = tbh + (2 * fh);
        int bw = w / 3;


        g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
        g.setColour(Colours::lightgrey);
        g.drawText(label_, 0 , 0 , w, fh, Justification::centred);

        float lvl = data_->lvl_;
        bool mute = data_->mute_;


        float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
        float db = constrain(dbS, dbMin, dbMax);
        float f = rescale(db, dbMin, dbMax, 0.0f, 1.0f);

        g.setColour(Colours::darkgrey);
        g.fillRect(bw - 1, barbase + 1, bw + 2, -(tbh + 2));


        int bh = int (f * float(tbh));
        int ypos = int (lvlYellow * float(tbh));
        int rpos = int (lvlRed * float(tbh));

        g.setColour(mute ? Colours::lightgrey : Colours::green);
        int gb = barbase;
        int gh = constrain(bh, 0, ypos - 1);
        g.fillRect(bw, gb, bw, -gh);

        if (bh > ypos) {
            g.setColour(mute ? Colours::lightgrey : Colours::yellow);
            int yh = constrain(bh, 0, rpos - 1);
            g.fillRect(bw, barbase - ypos, bw, -(yh - ypos));
            if (bh > rpos) {
                g.setColour(mute ? Colours::lightgrey : Colours::red);
                int rh = constrain(bh, 0, tbh);
                g.fillRect(bw, barbase - rpos, bw, -(rh - rpos));
            }
        }

        int y = barbase;
        y += 2 * fh;

        g.setColour(Colours::lightgrey);
        String val = String::formatted("%4.2f", dbS);
        g.drawText(val, 0 , y  , w, fh, Justification::centred);

        // solo
        y += 2 * fh;
        g.setColour(Colours::lightgrey);
        g.drawRect(bw - 1 , y - 1 , bw + 2 , fh + 2);
        if (data_->solo_) {
            g.setColour(Colours::yellow);
            g.fillRect(bw , y  , bw , fh );
            g.setColour(Colours::black);
        }
        g.drawText("S", bw , y , bw, fh, Justification::centred);

        // mute
        y += 2 * fh;
        g.setColour(Colours::lightgrey);
        g.drawRect(bw - 1 , y - 1 , bw + 2 , fh + 2);
        if (mute) {
            g.setColour(Colours::red);
            g.fillRect(bw , y , bw , fh );
            g.setColour(Colours::black);
        }
        g.drawText("M", bw ,y, bw, fh, Justification::centred);
    }
}
bool SSPChannel::buttonA() {
    switch (butMode_) {
    case BM_MUTESOLO:
        return data_->solo_;
    // case BM_HPF:
    //     break;
    default:
        ;
    }
    return false;
}

bool SSPChannel::buttonB() {
    switch (butMode_) {
    case BM_MUTESOLO:
        return data_->mute_;
    // case BM_HPF:
    //     break;
    default:
        ;
    }
    return false;
}


void SSPChannel::buttonA(bool b) {
    // Logger::writeToLog("buttonA: " + String(b));
    switch (butMode_) {
    case BM_MUTESOLO:
        data_->solo_ = !data_->solo_ ;
        break;
    // case BM_HPF:
    //     break;
    default:
        ;
    }
}


void SSPChannel::buttonB(bool b) {
    // Logger::writeToLog("buttonB: " + String(b));
    switch (butMode_) {
    case BM_MUTESOLO:
        data_->mute_ =  !data_->mute_;
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
