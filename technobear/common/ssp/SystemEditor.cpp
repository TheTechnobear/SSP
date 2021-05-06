#include "SystemEditor.h"

#include "BaseProcessor.h"

//#include "SSP.h"

namespace ssp {

//static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


SystemEditor::SystemEditor(BaseProcessor *p) :
    baseProcessor_(p),
    learnBtn_("Learn", [&](bool b) { midiLearn(b); }),
    delBtn_("Delete", [&](bool b) { deleteAutomation(b); }),
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

    setButtonBounds(learnBtn_, 0, 0);
    addAndMakeVisible(learnBtn_);

    setButtonBounds(delBtn_, 0, 1);
    addAndMakeVisible(delBtn_);

    baseProcessor_->midiLearn(false);

    auto in = MidiInput::getAvailableDevices();
    int selIdx = -1;
    midiInStr.push_back("NONE");
    for (int i = 0; i < in.size(); i++) {
        inDevices_.push_back(in[i]);
        midiInStr.push_back(in[i].name.toStdString());
        if (baseProcessor_->isActiveMidiIn(in[i].identifier.toStdString())) selIdx = i + 1; // none
    }
    midiInCtrl.setValues(midiInStr, selIdx);

    selIdx = -1;
    auto out = MidiOutput::getAvailableDevices();
    midiOutStr.push_back("NONE");
    for (int i = 0; i < out.size(); i++) {
        outDevices_.push_back(out[i]);
        midiOutStr.push_back(out[i].name.toStdString());
        if (baseProcessor_->isActiveMidiOut(out[i].identifier.toStdString())) selIdx = i + 1; //none
    }
    midiOutCtrl.setValues(midiOutStr, selIdx);

    addAndMakeVisible(midiInCtrl);
    addAndMakeVisible(midiOutCtrl);
    selIdx_ = 0;
    idxOffset_ = 0;

    startTimer(50);
}


void SystemEditor::midiInCallback(float idx, const std::string &dev) {
//    Logger::writeToLog("midiInCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto dev = inDevices_[i - 1];
        baseProcessor_->setMidiIn(dev.identifier.toStdString());
    } else {
        baseProcessor_->setMidiIn("");
    }
}

void SystemEditor::midiOutCallback(float idx, const std::string &dev) {
//    Logger::writeToLog("midiOutCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto dev = outDevices_[i - 1];
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

    int y = 60;

    g.setColour(Colours::red);
    g.drawSingleLineText("Parameter", 20, y);
    g.drawSingleLineText(String("Ch"), 400, y);
    g.drawSingleLineText("Type", 470, y);
    g.drawSingleLineText(String("Num"), 540, y);

    y += fh;

    auto &plist = baseProcessor_->getParameters();
    auto &am = baseProcessor_->midiAutomation();

    if (am.empty()) return;

    auto ai = am.begin();
    int idx = 0;
    while (idx < idxOffset_) {
        if (ai != am.end()) ai++;
        idx++;
    }

    for (unsigned i = 0; i < MAX_SHOWN && ai != am.end(); i++) {
//        int idx = i + idxOffset_;
//        if (idx > am.size() - 1) break;

        if (selIdx_ == idx) g.setColour(Colours::yellow);
        else g.setColour(Colours::white);

        auto a = ai->second;
        auto p = plist[a.paramIdx_];

        String type = "None";
        switch (a.midi_.type_) {
            case BaseProcessor::MidiAutomation::Midi::T_CC:
                type = "CC";
                break;
            case BaseProcessor::MidiAutomation::Midi::T_NOTE:
                type = "Note";
                break;
            case BaseProcessor::MidiAutomation::Midi::T_PRESSURE:
                type = "AT";
                break;
            default:
                break;
        }
        g.drawSingleLineText(p->getName(40), 20, y);
        g.drawSingleLineText(String(a.midi_.channel_), 400, y);
        g.drawSingleLineText(type, 470, y);
        g.drawSingleLineText(String(a.midi_.num_), 540, y);

        if (ai != am.end()) ai++;
        idx++;
        y += fh;
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
    const int fh = 32;
    midiInCtrl.setBounds(750, 50, 600, fh * 2);
    midiOutCtrl.setBounds(750, 50 + fh * 3, 600, fh * 2);
}

void SystemEditor::midiLearn(bool b) {
    baseProcessor_->midiLearn(b);
}

void SystemEditor::deleteAutomation(bool b) {
    if (!b) {
        if (selIdx_ >= 0) {
            auto &am = baseProcessor_->midiAutomation();
            if (am.empty() || selIdx_ >= am.size()) return;

            int idx=0;
            for(auto ai = am.begin(); ai!=am.end() ; ai++) {
                auto& a=ai->second;
                if(idx==selIdx_) {
                    am.erase(a.paramIdx_);
                    if(selIdx_!=0)  {
                        selIdx_--;
                        if (selIdx_ < idxOffset_) idxOffset_ = selIdx_;
                    }
                    break;
                }
                idx++;
            }
        }
    }
}

void SystemEditor::onEncoder(unsigned enc, float v) {
    switch (enc) {
        case 0: {
            auto amsize = baseProcessor_->midiAutomation().size();
            if (amsize == 0) return;

            if (v > 0) {
                if (selIdx_ < amsize - 1) {
                    selIdx_++;
                    if (selIdx_ >= idxOffset_ + MAX_SHOWN) idxOffset_ = selIdx_ - (MAX_SHOWN - 1);
                }
            } else {
                if (selIdx_ > 0) {
                    selIdx_--;
                    if (selIdx_ < idxOffset_) idxOffset_ = selIdx_;
                }
            }
        }
        case 1 : {
            break;
        }
        case 2 : {
            if (v > 0) midiInCtrl.inc(false);
            else midiInCtrl.dec(false);
            break;
        }
        case 3 : {
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
    switch (btn) {
        case 0: {
            if (!v) learnBtn_.toggle();
            break;
        }
        case 1: {
            delBtn_.value(v);
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
