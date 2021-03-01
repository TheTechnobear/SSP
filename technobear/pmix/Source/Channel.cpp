
#include "Channel.h"

#include <iostream>

constexpr inline float rescale(float in, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (in - inMin) / (inMax - inMin) * (outMax - outMin);
}


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}


bool Channel::button(unsigned i) {
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

void Channel::button(unsigned i, bool b) {
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

void Channel::encoder(float v) {
    float e = v;

    switch (encMode_) {
    case EM_LEVEL:
        e = v / 100.0f ;
        data_->level_[0] = constrain(data_->level_[0] + e, 0.0f, 4.0f);
        break;
    case EM_PAN:
        e = v / 100.0f ;
        data_->pan_ = constrain(data_->pan_ + e, -1.0f, 1.0f);
        break;
    case EM_AUX1:
        e = v / 100.0f ;
        data_->level_[1] = constrain(data_->level_[1] + e, 0.0f, 4.0f);
        break;
    case EM_AUX2:
        e = v / 100.0f ;
        data_->level_[2] = constrain(data_->level_[2] + e, 0.0f, 4.0f);
        break;
    case EM_AUX3:
        e = v / 100.0f ;
        data_->level_[3] = constrain(data_->level_[3] + e, 0.0f, 4.0f);
        break;
    case EM_GAIN:
        e = v / 100.0f ;
        data_->gain_ = constrain(data_->gain_ + e, 0.0f, 3.0f);
        break;
    default:
        ;
    }
    updateParam();
}


void Channel::encbutton(bool b) {
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
        data_->gain_ = 1.0f;
        break;
    default:
        ;
    }
    updateParam();
}




void Channel::buttonMode(ButMode m) {
    if (m != butMode_) {
        butMode_ = m;
        // probably no repaint necessary?!
    }
}

void Channel::encoderMode(EncMode m) {
    if (m != encMode_) {
        encMode_ = m;
        updateParam(true);
    }
}

void Channel::updateParam(bool newMode) {
    if (data_ && param_ ) {
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
            if (newMode) param_->label("Cue");
            param_->value(data_->level_[1]);
            break;
        case EM_AUX2:
            if (newMode) param_->label("Aux1");
            param_->value(data_->level_[2]);
            break;
        case EM_AUX3:
            if (newMode) param_->label("Aux2");
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
