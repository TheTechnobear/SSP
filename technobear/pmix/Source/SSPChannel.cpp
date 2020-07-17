
#include "SSPChannel.h"

#include <iostream>
void SSPChannel::paint(Graphics &g) {
    if (active_ && data_ != nullptr) {
        unsigned h = getHeight();
        unsigned w = getWidth();
    }
}


void SSPChannel::button(bool b) {
    switch (butMode_) {
    case BM_MUTE:
        break;
    case BM_SOLO:
        break;
    // case BM_HPF:
    //     break;
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
    }
}



void SSPChannel::buttonMode(ButMode m) {
    if (m != butMode_) {
        butMode_ = m;
        // probably no repaint necessary?!
    }
}

void SSPChannel::encoderMode_(EncMode m) {
    if (m != encMode_) {
        encMode_ = m;
        // redraw value, to reflect new mode
    }
}
