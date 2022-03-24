#include "BaseEditor.h"

#include "BaseProcessor.h"

#include "SSP.h"

#include "ParamControl.h"
#include "ParamButton.h"

namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


BaseEditor::BaseEditor(BaseProcessor *p) :
    baseProcessor_(p),
    leftBtn_("PG-", nullptr, 32, Colours::red), rightBtn_("PG+", nullptr, 32, Colours::red),
    upBtn_("EN-", nullptr, 32, Colours::red), downBtn_("EN+", nullptr, 32, Colours::red),
    leftShiftBtn_("LS", nullptr, 32, Colours::grey, Colours::black), rightShiftBtn_("RS", nullptr, 32, Colours::grey, Colours::black) {

    setButtonBounds(leftShiftBtn_, 0, 4);
    setButtonBounds(upBtn_, 0, 5);
    setButtonBounds(rightShiftBtn_, 0, 6);
    setButtonBounds(downBtn_, 1, 5);
    setButtonBounds(leftBtn_, 1, 4);
    setButtonBounds(rightBtn_, 1, 6);

    addAndMakeVisible(upBtn_);
    addAndMakeVisible(downBtn_);

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

void BaseEditor::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (c * w);
    unsigned y = btnTopY + (r * h);
    btn.setBounds(x, y, w, h);
}

void BaseEditor::resized() {

}


void BaseEditor::setButtonBounds(unsigned idx, std::shared_ptr<ParamButton> c) {
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
    leftBtn_.value(v);
}

void BaseEditor::onRightButton(bool v) {
    rightBtn_.value(v);
}

void BaseEditor::onUpButton(bool v) {
    upBtn_.value(v);
}

void BaseEditor::onDownButton(bool v) {
    downBtn_.value(v);
}

void BaseEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.value(v);
}

void BaseEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.value(v);
}

void BaseEditor::onSSPTimer() {

}


}
