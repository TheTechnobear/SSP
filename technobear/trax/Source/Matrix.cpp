#include "Matrix.h"

#include "Track.h"
#include "ssp/Log.h"

Matrix::Matrix() {
}

void Matrix::getStateInformation(juce::XmlElement& outStream) {
    for (auto& w : connections_) {
        std::unique_ptr<juce::XmlElement> xmlWire = std::make_unique<juce::XmlElement>("Wire");
        xmlWire->setAttribute("srcMod", (int)w.src_.modIdx_);
        xmlWire->setAttribute("srcCh", (int)w.src_.chIdx_);
        xmlWire->setAttribute("destMod", (int)w.dest_.modIdx_);
        xmlWire->setAttribute("destCh", (int)w.dest_.chIdx_);
        outStream.addChildElement(xmlWire.release());
    }
}

void Matrix::setStateInformation(juce::XmlElement& inStream) {
    connections_.clear();
    

    for (auto xmlWire : inStream.getChildIterator()) {
        int srcMod = xmlWire->getIntAttribute("srcMod");
        int srcCh = xmlWire->getIntAttribute("srcCh");
        int destMod = xmlWire->getIntAttribute("destMod");
        int destCh = xmlWire->getIntAttribute("destCh");
        connect(Jack(srcMod, srcCh), Jack(destMod, destCh));
    }
}
