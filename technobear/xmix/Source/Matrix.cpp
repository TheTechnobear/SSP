#include "Matrix.h"

#include "Track.h"

Matrix::Matrix() : modules_(Track::M_MAX){
}


void Matrix::createDefault(unsigned in1, unsigned in2) {
    // proof of concept only
    modules_[Track::M_PRE].addRoute(SRC_INPUT, in1, 0);
    modules_[Track::M_PRE].addRoute(SRC_INPUT, in2, 1);

    modules_[Track::M_MAIN].addRoute(SRC_WORK, 0, 0);
    modules_[Track::M_MAIN].addRoute(SRC_WORK, 1, 1);

    modules_[Track::M_POST].addRoute(SRC_WORK, 0, 0);
    modules_[Track::M_POST].addRoute(SRC_WORK, 1, 1);

    output_.addRoute(SRC_WORK, 0, 0);
    output_.addRoute(SRC_WORK, 1, 1);
}



//TODO : persist routing data
void Matrix::getStateInformation(juce::MemoryOutputStream& outStream) {

}

void Matrix::setStateInformation(juce::MemoryInputStream& inStream) {

}
