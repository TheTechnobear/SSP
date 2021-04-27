#include "SystemEditor.h"

#include "BaseProcessor.h"

//#include "SSP.h"

namespace ssp {

//static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


SystemEditor::SystemEditor(BaseProcessor *p) :
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
//    addAndMakeVisible(rightShiftBtn_);

    auto in = MidiInput::getAvailableDevices();
    for (int i = 0; i < in.size(); i++)inDevices_.push_back(in[i]);
    auto out = MidiOutput::getAvailableDevices();
    for (int i = 0; i < out.size(); i++)outDevices_.push_back(out[i]);


    startTimer(50);
}


SystemEditor::~SystemEditor() {
    stopTimer();
}

void SystemEditor::timerCallback() {
    repaint();
}

void SystemEditor::paint(Graphics &g) {
//    drawBasicPanel(g);
    drawView(g);
}

void SystemEditor::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)

    const int fh = 24;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + " : " + String(JucePlugin_Desc), 20, 30);

    for (int i = 0; i < inDevices_.size(); i++) {
        g.drawSingleLineText(inDevices_[i].name, 40, (i + 2 ) * (fh + 5));
    }
}

void SystemEditor::setButtonBounds(SSPButton &btn, unsigned r, unsigned c) {
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (c * w);
    unsigned y = btnTopY + (r * h);
    btn.setBounds(x, y, w, h);
}

void SystemEditor::resized() {

}


void SystemEditor::onEncoder(unsigned enc, float v) {
}

void SystemEditor::onEncoderSwitch(unsigned enc, bool v) {
}

void SystemEditor::onButton(unsigned btn, bool v) {
}


void SystemEditor::onLeftButton(bool v) {
    leftBtn_.active(v);
}

void SystemEditor::onRightButton(bool v) {
    rightBtn_.active(v);
}

void SystemEditor::onUpButton(bool v) {
    upBtn_.active(v);
}

void SystemEditor::onDownButton(bool v) {
    downBtn_.active(v);
}

void SystemEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.active(v);
}

void SystemEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.active(v);
}


}
