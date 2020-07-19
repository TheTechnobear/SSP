
#include "SSPChannel.h"

#include <iostream>
void SSPChannel::paint(Graphics &g) {
    if (active_ && data_ != nullptr) {
        // unsigned h = getHeight();
        // unsigned w = getWidth();
    }
}


void SSPChannel::buttonA(bool b) {
    switch (butMode_) {
    case BM_MUTESOLO:
        data_->mute_=b;
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
        data_->solo_=b;
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
