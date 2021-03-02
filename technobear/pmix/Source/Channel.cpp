
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
        if (i) return data_->mute.getValue();
        else  return data_->solo.getValue();
    case BM_CUEAC:
        if (i) return data_->ac.getValue();
        else  return data_->cue.getValue();
        break;
    default:
        ;
    }
    return false;
}

void Channel::button(unsigned i, bool b) {
    // Logger::writeToLog("buttonA: " + String(b));
    //TODO CHECK this
    switch (butMode_) {
    case BM_SOLOMUTE:
        if (i) {
            data_->mute.setValue(!data_->mute.getValue());
        } else {
            data_->solo.setValue(!data_->solo.getValue());
        }
        break;
    case BM_CUEAC:
        if (i) {
            data_->ac.setValue(!data_->ac.getValue());
        } else {
            data_->cue.setValue(!data_->cue.getValue());
        }
        break;
    default:
        ;
    }
}

void Channel::encoder(float v) {
    float e = v;

    // TODO CHECK - need convert0to1?
    switch (encMode_) {
    case EM_LEVEL:
        e = v / 100.0f ;
        data_->level[0].get().setValue(data_->level[0].get().getValue() +e );
        break;
    case EM_PAN:
        e = v / 100.0f ;
        data_->pan.setValue(data_->pan.getValue() + e);
        break;
    case EM_AUX1:
        e = v / 100.0f ;
            data_->level[1].get().setValue(data_->level[1].get().getValue()+e);
        break;
    case EM_AUX2:
        e = v / 100.0f ;
            data_->level[2].get().setValue(data_->level[2].get().getValue()+e);
        break;
    case EM_AUX3:
        e = v / 100.0f ;
        data_->level[3].get().setValue(data_->level[3].get().getValue() +e );
        break;
    case EM_GAIN:
        e = v / 100.0f ;
        data_->gain.setValue(data_->gain.getValue() + e);
        break;
    default:
        ;
    }
    updateParam();
}


void Channel::encbutton(bool b) {
    //TODO CHECK, might need convertTo0to1
    switch (encMode_) {
    case EM_LEVEL:
        data_->level[0].get().setValue(data_->level[0].get().getValue() == 1.0 ? 0.0f : 1.0f);
        break;
    case EM_PAN:
        data_->pan.setValue(data_->pan.convertTo0to1(0.0f));
        break;
    case EM_AUX1:
        data_->level[1].get().setValue(data_->level[1].get().getValue() == 1.0 ? 0.0f : 1.0f);
        break;
    case EM_AUX2:
        data_->level[2].get().setValue(data_->level[2].get().getValue() == 1.0 ? 0.0f : 1.0f);
        break;
    case EM_AUX3:
        data_->level[3].get().setValue(data_->level[3].get().getValue() == 1.0 ? 0.0f : 1.0f);
        break;
    case EM_GAIN:
        data_->gain.setValue(data_->gain.convertTo0to1(0.0f));
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
        case EM_LEVEL: {
            if (newMode) param_->label("Level");
            auto &p = data_->level[0].get();
            param_->value(p.convertFrom0to1(p.getValue()));
            break;
        }
        case EM_PAN: {
            if (newMode) param_->label("Pan");
            auto &p = data_->pan;
            param_->value(p.convertFrom0to1(p.getValue()));
            break;
        }
        case EM_AUX1: {
            if (newMode) param_->label("Cue");
            auto &p = data_->level[1].get();
            param_->value(p.convertFrom0to1(p.getValue()));
            break;
        }
        case EM_AUX2: {
            if (newMode) param_->label("Aux1");
            auto &p = data_->level[2].get();
            param_->value(p.convertFrom0to1(p.getValue()));
            break;
        }
        case EM_AUX3: {
            if (newMode) param_->label("Aux2");
            auto &p = data_->level[3].get();
            param_->value(p.convertFrom0to1(p.getValue()));
            break;
        }
        case EM_GAIN: {
            if (newMode) param_->label("Gain");
            auto &p = data_->gain;
            param_->value(p.convertFrom0to1(p.getValue()));
            break;
        }
        default:
            ;
        }
        param_->repaint();
    }
}
