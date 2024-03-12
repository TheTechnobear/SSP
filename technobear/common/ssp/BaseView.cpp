#include "BaseView.h"
#include "BaseProcessor.h"

namespace ssp {

BaseView::BaseView(BaseProcessor *p,bool compactUI) :
    baseProcessor_(p),
    compactUI_(compactUI) {
}


BaseView::~BaseView() {
}

void BaseView::paint(juce::Graphics &g) {
//    drawBasicPanel(g);
    drawView(g);
}

void BaseView::drawView(juce::Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)
}


void BaseView::resized() {

}


void BaseView::onSSPTimer() {

}


}
