#include "BaseView.h"
#include "BaseProcessor.h"

namespace ssp {

BaseView::BaseView(BaseProcessor *p) :
    baseProcessor_(p) {
    startTimer(50);
}


BaseView::~BaseView() {
    stopTimer();
}

void BaseView::timerCallback() {
    repaint();
}

void BaseView::paint(Graphics &g) {
//    drawBasicPanel(g);
    drawView(g);
}

void BaseView::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)
}


void BaseView::resized() {

}


void BaseView::onSSPTimer() {

}


}
