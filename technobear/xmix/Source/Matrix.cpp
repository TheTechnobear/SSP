#include "Matrix.h"

#include "Track.h"

Matrix::Matrix() {
}


void Matrix::createDefault(unsigned in1, unsigned in2) {
    // proof of concept only
    connect(Jack(Track::M_IN, in1), Jack(Track::M_USER_1, 0));
    connect(Jack(Track::M_IN, in2), Jack(Track::M_USER_1, 1));
    connect(Jack(Track::M_USER_1, 0), Jack(Track::M_USER_2, 0));
    connect(Jack(Track::M_USER_1, 1), Jack(Track::M_USER_2, 1));
    connect(Jack(Track::M_USER_2, 0), Jack(Track::M_USER_3, 0));
    connect(Jack(Track::M_USER_2, 1), Jack(Track::M_USER_3, 1));
    connect(Jack(Track::M_USER_3, 0), Jack(Track::M_OUT, 0));
    connect(Jack(Track::M_USER_3, 1), Jack(Track::M_OUT, 1));
}


// TODO : persist routing data
void Matrix::getStateInformation(juce::MemoryOutputStream& outStream) {
}

void Matrix::setStateInformation(juce::MemoryInputStream& inStream) {
}
