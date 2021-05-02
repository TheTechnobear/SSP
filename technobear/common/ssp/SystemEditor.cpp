#include "SystemEditor.h"

#include "BaseProcessor.h"

//#include "SSP.h"

namespace ssp {

//static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


SystemEditor::SystemEditor(BaseProcessor *p) :
    baseProcessor_(p),
    learnBtn_("Learn",[&](bool b) { midiLearn(b); }),
    leftBtn_("PG-", nullptr, Colours::red), rightBtn_("PG+", nullptr, Colours::red),
    upBtn_("EN-", nullptr, Colours::red), downBtn_("EN+", nullptr, Colours::red),
    leftShiftBtn_("LS", nullptr, Colours::grey, Colours::black), rightShiftBtn_("RS", nullptr, Colours::grey, Colours::black),
    midiInCtrl("Midi IN", [&](float idx, const std::string &str) { midiInCallback(idx, str); }),
    midiOutCtrl("Midi OUT", [&](float idx, const std::string &str) { midiOutCallback(idx, str); }) {

    setButtonBounds(leftShiftBtn_, 0, 4);
    setButtonBounds(upBtn_, 0, 5);
    setButtonBounds(rightShiftBtn_, 0, 6);
    setButtonBounds(downBtn_, 1, 5);
    setButtonBounds(leftBtn_, 1, 4);
    setButtonBounds(rightBtn_, 1, 6);

    addAndMakeVisible(upBtn_);
    addAndMakeVisible(downBtn_);

    setButtonBounds(learnBtn_,0,0);
    addAndMakeVisible(learnBtn_);

    baseProcessor_->midiLearn(false);

    auto in = MidiInput::getAvailableDevices();
    midiInStr.push_back("NONE");
    for (int i = 0; i < in.size(); i++) {
        inDevices_.push_back(in[i]);
        midiInStr.push_back(in[i].name.toStdString());
    }
    midiInCtrl.setValues(midiInStr);

    auto out = MidiOutput::getAvailableDevices();
    midiOutStr.push_back("NONE");
    for (int i = 0; i < out.size(); i++) {
        outDevices_.push_back(out[i]);
        midiOutStr.push_back(out[i].name.toStdString());
    }
    midiOutCtrl.setValues(midiOutStr);

    addAndMakeVisible(midiInCtrl);
    addAndMakeVisible(midiOutCtrl);

    startTimer(50);
}


void SystemEditor::midiInCallback(float idx, const std::string &dev) {
    Logger::writeToLog("midiInCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto dev = inDevices_[i - 1];
        baseProcessor_->setMidiIn(dev.identifier.toStdString());
    } else {
        baseProcessor_->setMidiOut("");
    }
}

void SystemEditor::midiOutCallback(float idx, const std::string &dev) {
    Logger::writeToLog("midiOutCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto dev = outDevices_[i - 1 ];
        baseProcessor_->setMidiOut(dev.identifier.toStdString());
    } else {
        baseProcessor_->setMidiOut("");
    }
}


SystemEditor::~SystemEditor() {
    baseProcessor_->midiLearn(false);
    stopTimer();
}

void SystemEditor::timerCallback() {
    repaint();
}

void SystemEditor::paint(Graphics &g) {
    const int fh = 24;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + " : " + String(JucePlugin_Desc), 20, 30);

    drawView(g);
}

void SystemEditor::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)

    const int fh = 24;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.setColour(Colours::white);

    int x = 20;
    int y = 50;

    auto& plist=baseProcessor_->getParameters();
    for(auto a : baseProcessor_->midiAutomation()) {
        assert(a.paramIdx_ < plist.size());
        auto p = plist[a.paramIdx_];
        g.drawSingleLineText(p->getName(40), x, y);
        y+= fh *2;
    }
}

void SystemEditor::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (c * w);
    unsigned y = btnTopY + (r * h);
    btn.setBounds(x, y, w, h);
}


void SystemEditor::resized() {
    const int fh = 36;
    midiInCtrl.setBounds(700, 50, 600, fh * 2);
    midiOutCtrl.setBounds(700, 50 + fh * 3, 600, fh * 2);
}

void SystemEditor::midiLearn(bool b) {
    baseProcessor_->midiLearn(b);
}

void SystemEditor::onEncoder(unsigned enc, float v) {
    switch (enc) {
        case 0 : {
            if (v > 0) midiInCtrl.inc(false);
            else midiInCtrl.dec(false);
            break;
        }
        case 1 : {
            if (v > 0) midiOutCtrl.inc(false);
            else midiOutCtrl.dec(false);
            break;
        }
        default: { ;
        }
    }
}

void SystemEditor::onEncoderSwitch(unsigned enc, bool v) {
}

void SystemEditor::onButton(unsigned btn, bool v) {
    switch(btn) {
        case 0: {
            if(!v) learnBtn_.toggle();
            break;
        }
        default:
            break;
    }
}


void SystemEditor::onLeftButton(bool v) {
    leftBtn_.value(v);
}

void SystemEditor::onRightButton(bool v) {
    rightBtn_.value(v);
}

void SystemEditor::onUpButton(bool v) {
    upBtn_.value(v);
}

void SystemEditor::onDownButton(bool v) {
    downBtn_.value(v);
}

void SystemEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.value(v);
}

void SystemEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.value(v);
}


}
