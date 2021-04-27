#include "BaseEditor.h"

#include "BaseProcessor.h"

#include "SSP.h"

#include "BaseParamControl.h"
#include "ButtonControl.h"

namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


BaseEditor::BaseEditor(BaseProcessor *p, String t, String v)
    : AudioProcessorEditor(p),
      baseProcessor_(p),
      leftAtt_(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSwLeft)), [this](float f) { onLeftButton(f); }),
      rightAtt_(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSwRight)), [this](float f) { onRightButton(f); }),
      upAtt_(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSwUp)), [this](float f) { onUpButton(f); }),
      downAtt_(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSwDown)), [this](float f) { onDownButton(f); }),
      LSAtt_(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSwShiftL)), [this](float f) { onLeftShiftButton(f); }),
      RSAtt_(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSwShiftR)), [this](float f) { onRightShiftButton(f); }),
      title_(t),
      version_(v) {


    for (int i = 0; i < BaseProcessor::sspParams::numEnc; i++) {
        encAtt_.push_back(std::make_unique<ParameterAttachment>(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspEnc1 + i)),
                                                                [this, i](float f) { onEncoder(i, f); }));
    }
    for (int i = 0; i < BaseProcessor::sspParams::numEnc; i++) {
        encSwAtt_.push_back(std::make_unique<ParameterAttachment>(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspEncSw1 + i)),
                                                                  [this, i](float f) { onEncoderSwitch(i, f); }));
    }
    for (int i = 0; i < BaseProcessor::sspParams::numSw; i++) {
        btnAtt_.push_back(std::make_unique<ParameterAttachment>(*p->getParameter(BaseProcessor::sspParams::getId(Percussa::sspSw1 + i)),
                                                                [this, i](float f) { onButton(i, f); }));
    }

//    setSize(1600, 480);

    globalBtn_.init("G", Colours::red);
    networkBtn_.init("N", Colours::red);
    plugInBtn_.init("P", Colours::red);
    recBtn_.init("R", Colours::red);
    addAndMakeVisible(globalBtn_);
    addAndMakeVisible(networkBtn_);
    addAndMakeVisible(plugInBtn_);
    addAndMakeVisible(recBtn_);


    setMenuBounds(globalBtn_, 0);
    setMenuBounds(networkBtn_, 1);
    setMenuBounds(plugInBtn_, 2);
    setMenuBounds(recBtn_, 3);

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
    drawView(g);
}


void BaseEditor::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/botton (0..479)

    g.fillAll(Colour(0xff111111));

    // title
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
    g.setColour(Colours::yellow);
    g.drawSingleLineText(title_, 20, 30);
    g.setColour(Colours::grey);
    g.drawSingleLineText("version : " + version_, 1400, 30);

    drawMenuBox(g);
    drawButtonBox(g);
}


void BaseEditor::drawMenuBox(Graphics &g) {
    unsigned y = menuTopY - 1;
    unsigned x = 1530 - 1;
    unsigned butTopY = btnTopY;
    g.setColour(Colours::grey);
    g.drawVerticalLine(x, y, butTopY);
    g.drawVerticalLine(1600 - 1, y, butTopY);
    for (int i = 0; i < 5; i++) {
        g.drawHorizontalLine(y + i * 45, x, 1600 - 1);
    }
}

void BaseEditor::drawButtonBox(Graphics &g) {
    unsigned butTopY = btnTopY;
    unsigned butLeftX = 900 - 1;
    float x = butLeftX;
    float y = butTopY;
    g.setColour(Colours::grey);
    g.drawHorizontalLine(y, x, 1600 - 1);
    g.drawHorizontalLine(y + btnSpaceY, x, 1600 - 1);
    g.drawHorizontalLine(480 - 1, x, 1600 - 1);
    for (int i = 0; i < 8; i++) {
        g.drawVerticalLine(x + i * 100, butTopY, 480 - 1);
    }
}

void BaseEditor::setMenuBounds(SSPButton &btn, unsigned r) {
    const int w = 70;
    const int h = 45;
    unsigned x = 1530 + 1;
    unsigned y = menuTopY + (r * h);
    btn.setBounds(x, y, w, h);
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
