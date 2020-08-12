
#include "SSPChannel.h"

#include <iostream>


constexpr inline float rescale(float in, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (in - inMin) / (inMax - inMin) * (outMax - outMin);
}

void SSPChannel::paint(Graphics &g) {
    if (data_ != nullptr) {
    // if (active_ && data_ != nullptr) {
        static constexpr float dbMin        = -70.0f, dbMax = 6.0f; // db range for meter
        static constexpr float dbRed        = 0.0f, dbYellow = -6.0f;
        static constexpr float lvlRed       = rescale(dbRed, dbMin, dbMax, 0.0f, 1.0f);
        static constexpr float lvlYellow    = rescale(dbYellow, dbMin, dbMax, 0.0f, 1.0f);

        static constexpr int fh = 16;
        int h = getHeight();
        int w = getWidth();
        int tbh = h - (fh * 10);
        int barbase = tbh + (2 * fh);
        int bw = w / 2;
        int bx = w / 4;


        g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
        g.setColour(Colours::lightgrey);
        g.drawText(label_, 0 , 0 , w, fh, Justification::centred);

        bool  mute = data_->mute_;
        float lvl = data_->lvl_;


        float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
        float db = constrain(dbS, dbMin, dbMax);
        float f = rescale(db, dbMin, dbMax, 0.0f, 1.0f);

        g.setColour(Colours::darkgrey);
        int bl = tbh + 2;
        g.fillRect(bx - 1, barbase - bl, bw + 2, bl);


        int bh = int (f * float(tbh));
        int ypos = int (lvlYellow * float(tbh));
        int rpos = int (lvlRed * float(tbh));

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
                g.fillRect(bx, barbase - (rpos + bl), bw, bl );
            }
        }


        // 0db line
        g.setColour(Colours::darkgrey);
        g.fillRect(bx - 5, barbase - rpos, bw + 10, 2 );

        // lvl
        float lvlSlider = data_->level_[0];
        dbS = lvlSlider > 0.0f ? std::log10(lvlSlider) * 20.0f : -200.f;
        db = constrain(dbS, dbMin, dbMax);
        f = rescale(db, dbMin, dbMax, 0.0f, 1.0f);
        g.setColour(Colours::lightgrey);
        g.fillRoundedRectangle (bx - 5, barbase - (f * tbh), bw + 10, 5, 5);



        int y = barbase;
        y += 2 * fh;

        // g.setColour(Colours::lightgrey);
        // String val = String::formatted("%4.2f", dbS);
        // g.drawText(val, 0 , y  , w, fh, Justification::centred);

        // pan
        g.setColour(Colours::lightgrey);
        g.drawLine(bx, y, bx + bw, y, 2);
        unsigned poff = unsigned( float(bw) * ((data_->pan_ + 1.0) / 2.0));
        g.fillRect(bx + poff - 5, y - 5, 10, 10);

        // solo
        y += 2 * fh;
        g.setColour(Colours::lightgrey);
        g.drawRect(bx - 1 , y - 1 , bw + 2 , fh + 2);
        if (data_->solo_) {
            g.setColour(Colours::yellow);
            g.fillRect(bx , y  , bw , fh );
            g.setColour(Colours::black);
        }
        g.drawText("S", bx , y , bw, fh, Justification::centred);

        // mute
        y += 2 * fh;
        g.setColour(Colours::lightgrey);
        g.drawRect(bx - 1 , y - 1 , bw + 2 , fh + 2);
        if (mute) {
            g.setColour(Colours::red);
            g.fillRect(bx , y , bw , fh );
            g.setColour(Colours::black);
        }
        g.drawText("M", bx , y, bw, fh, Justification::centred);
    }
}
bool SSPChannel::button(unsigned i) {
    switch (butMode_) {
    case BM_SOLOMUTE:
        if (i) return data_->mute_;
        else  return data_->solo_;
    case BM_CUEAC:
        if (i) return data_->ac_;
        else  return data_->cue_;
        break;
    default:
        ;
    }
    return false;
}

void SSPChannel::button(unsigned i, bool b) {
    // Logger::writeToLog("buttonA: " + String(b));
    switch (butMode_) {
    case BM_SOLOMUTE:
        if (i) {
            data_->mute_ =  !data_->mute_;
        } else {
            data_->solo_ = !data_->solo_ ;
        }
        break;
    case BM_CUEAC:
        if (i) {
            data_->ac_ = !data_->ac_ ;
        } else {
            data_->cue_ =  !data_->cue_;
        }
        break;
    default:
        ;
    }
}

void SSPChannel::encoder(float v) {
    float e=v;
//        gain_=1.0f;

    switch (encMode_) {
    case EM_LEVEL:
        e= v / 100.0f ;
        data_->level_[0] = constrain(data_->level_[0] + e, 0.0f, 4.0f);
        break;
    case EM_PAN:
        e= v / 100.0f ;
        data_->pan_ = constrain(data_->pan_ + e, -1.0f, 1.0f);
        break;
    case EM_AUX1:
        e= v / 100.0f ;
        data_->level_[1] = constrain(data_->level_[1] + e, 0.0f, 4.0f);
        break;
    case EM_AUX2:
        e= v / 100.0f ;
        data_->level_[2] = constrain(data_->level_[2] + e, 0.0f, 4.0f);
        break;
    case EM_AUX3:
        e= v / 100.0f ;
        data_->level_[3] = constrain(data_->level_[3] + e, 0.0f, 4.0f);
        break;
    case EM_GAIN:
        e= v / 100.0f ;
        data_->gain_ = constrain(data_->gain_ + e, 0.0f, 3.0f);
        break;
    default:
        ;
    }
    updateParam();
}


void SSPChannel::encbutton(bool b) {
    switch (encMode_) {
    case EM_LEVEL:
        data_->level_[0] = data_->level_[0] == 1.0 ? 0.0f : 1.0f;
        break;
    case EM_PAN:
        data_->pan_ = 0.0f;
        break;
    case EM_AUX1:
        data_->level_[1] = data_->level_[1] == 1.0 ? 0.0f : 1.0f;
        break;
    case EM_AUX2:
        data_->level_[2] = data_->level_[2] == 1.0 ? 0.0f : 1.0f;
        break;
    case EM_AUX3:
        data_->level_[3] = data_->level_[3] == 1.0 ? 0.0f : 1.0f;
        break;
    case EM_GAIN:
        break;
        data_->gain_ = 0.0f;
    default:
        ;
    }
    updateParam();        
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
        updateParam(true);
    }
}

void SSPChannel::updateParam(bool newMode) {
    if (data_ && param_ && active_) {
        switch (encMode_) {
        case EM_LEVEL:
            if (newMode) param_->label("Level");
            param_->value(data_->level_[0]);
            break;
        case EM_PAN:
            if (newMode) param_->label("Pan");
            param_->value(data_->pan_);
            break;
        case EM_AUX1:
            if (newMode) param_->label("Aux1");
            param_->value(data_->level_[1]);
            break;
        case EM_AUX2:
            if (newMode) param_->label("Aux2");
            param_->value(data_->level_[2]);
            break;
        case EM_AUX3:
            if (newMode) param_->label("Aux3");
            param_->value(data_->level_[3]);
            break;
        case EM_GAIN:
            if (newMode) param_->label("Gain");
            param_->value(data_->gain_);
            break;
        default:
            ;
        }
        param_->repaint();
    }
}

void SSPChannel::active(bool a) {
    if (active_ != a) {
        active_ = a;
        if (active_) {
            updateParam(true);
        }
        repaint();
    }
}


