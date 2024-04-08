#include "BaseEditor.h"

#include "ssp/BaseProcessor.h"

#include "SSP.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


BaseEditor::BaseEditor(BaseProcessor *p)  :
    BaseView(p,false),
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
}


BaseEditor::~BaseEditor() {
}

void BaseEditor::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (c * w);
    unsigned y = btnTopY + (r * h);
    btn.setBounds(x, y, w, h);
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
    leftBtn_.onButton(v);
}

void BaseEditor::onRightButton(bool v) {
    rightBtn_.onButton(v);
}

void BaseEditor::onUpButton(bool v) {
    upBtn_.onButton(v);
}

void BaseEditor::onDownButton(bool v) {
    downBtn_.onButton(v);
}

void BaseEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.onButton(v);
}

void BaseEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.onButton(v);
}

}
