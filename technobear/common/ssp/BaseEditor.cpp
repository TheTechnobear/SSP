#include "BaseEditor.h"

#include "BaseProcessor.h"

#include "SSP.h"

#include "BaseParamControl.h"
#include "ButtonControl.h"

namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


BaseEditor::BaseEditor(BaseProcessor *p) :
    baseProcessor_(p) {

    upBtn_.init("EN-", Colours::red);
    setButtonBounds(upBtn_, 0, 5);
    addAndMakeVisible(upBtn_);

    downBtn_.init("EN+", Colours::red);
    setButtonBounds(downBtn_, 1, 5);
    addAndMakeVisible(downBtn_);

//    leftBtn_.init("PG-", Colours::red);
//    setButtonBounds(leftBtn_, 	1, 4);
//    addAndMakeVisible(leftBtn_);
//
//    rightBtn_.init("PG+", Colours::red);
//    setButtonBounds(rightBtn_, 	1, 6);
//    addAndMakeVisible(rightBtn_);
//

//    leftShiftBtn_.init("LS", Colours::grey, Colours::black);
//    setButtonBounds(leftShiftBtn_,	0, 4);
//    addAndMakeVisible(leftShiftBtn_);
//
//    rightShiftBtn_.init("RS", Colours::grey, Colours::black);
//    setButtonBounds(rightShiftBtn_,	0, 6);
    addAndMakeVisible(rightShiftBtn_);

    startTimer(50);
}


BaseEditor::~BaseEditor() {
    stopTimer();
}

void BaseEditor::timerCallback() {
    repaint();
}

void BaseEditor::paint(Graphics &g) {
//    drawBasicPanel(g);
    drawView(g);
}

void BaseEditor::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)
}

void BaseEditor::setButtonBounds(SSPButton &btn, unsigned r, unsigned c) {
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (c * w);
    unsigned y = btnTopY + (r * h);
    btn.setBounds(x, y, w, h);
}

void BaseEditor::resized() {

}


void BaseEditor::setButtonBounds(unsigned idx, std::shared_ptr<ButtonControl> c) {
    if (c == nullptr) return;
    unsigned col = idx % 4;
    unsigned row = idx / 4;
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (col * w);
    unsigned y = btnTopY + (row * h);

    c->setBounds(x, y, w, h);
}


void BaseEditor::onEncoder(unsigned enc, float v) {
}

void BaseEditor::onEncoderSwitch(unsigned enc, bool v) {
    encoderState_[enc] = v;
}

void BaseEditor::onButton(unsigned btn, bool v) {
}


void BaseEditor::onLeftButton(bool v) {
    leftBtn_.active(v);
}

void BaseEditor::onRightButton(bool v) {
    rightBtn_.active(v);
}

void BaseEditor::onUpButton(bool v) {
    upBtn_.active(v);
}

void BaseEditor::onDownButton(bool v) {
    downBtn_.active(v);
}

void BaseEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.active(v);
}

void BaseEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.active(v);
}


}
