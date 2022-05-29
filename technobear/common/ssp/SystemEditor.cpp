#include "SystemEditor.h"

#include "BaseProcessor.h"

//#include "SSP.h"

namespace ssp {

//static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;

inline bool isInternalMidi(const String &name) {
    return name.contains("Juce") || name.contains("Midi Through Port");
}

SystemEditor::SystemEditor(BaseProcessor *p) :
    baseProcessor_(p),
    learnBtn_("Learn", [&](bool b) { midiLearn(b); }, 24),
    delBtn_("Delete", [&](bool b) { deleteAutomation(b); }, 24, Colours::yellow),
    noteInputBtn_("Note In", [&](bool b) { noteInput(b); }, 24, Colours::lightskyblue),
    leftBtn_("PG-", nullptr, 32, Colours::red), rightBtn_("PG+", nullptr, 32, Colours::red),
    upBtn_("EN-", nullptr, 32, Colours::red), downBtn_("EN+", nullptr, 32, Colours::red),
    leftShiftBtn_("LS", nullptr, 32, Colours::grey, Colours::black), rightShiftBtn_("RS", nullptr, 32, Colours::grey, Colours::black),
    midiInCtrl_("Midi IN", [&](float idx, const std::string &str) { midiInCallback(idx, str); }),
    midiOutCtrl_("Midi OUT", [&](float idx, const std::string &str) { midiOutCallback(idx, str); }),
    midiChannelCtrl_("Midi Channel", [&](float idx, const std::string &str) { midiChannelCallback(idx, str); }) {

    setButtonBounds(leftShiftBtn_, 0, 4);
    setButtonBounds(upBtn_, 0, 5);
    setButtonBounds(rightShiftBtn_, 0, 6);
    setButtonBounds(downBtn_, 1, 5);
    setButtonBounds(leftBtn_, 1, 4);
    setButtonBounds(rightBtn_, 1, 6);

    addAndMakeVisible(upBtn_);
    addAndMakeVisible(downBtn_);

    setButtonBounds(learnBtn_, 0, 0);
    setButtonBounds(noteInputBtn_, 0, 1);
    setButtonBounds(delBtn_, 1, 0);

    addAndMakeVisible(noteInputBtn_);
    addAndMakeVisible(learnBtn_);
    addAndMakeVisible(delBtn_);

    baseProcessor_->midiLearn(false);

    auto in = MidiInput::getAvailableDevices();
    int selIdx = -1;
    midiInStr_.push_back("NONE");
    int idx = 0;
    for (int i = 0; i < in.size(); i++) {
        if (!isInternalMidi(in[i].name)) {
            inDevices_.push_back(in[i]);
            midiInStr_.push_back(std::to_string(idx) + ":" + in[i].name.toStdString());
            if (baseProcessor_->isActiveMidiIn(in[i].name.toStdString())) selIdx = idx + 1; // none
            idx++;
        }
    }
    midiInCtrl_.setValues(midiInStr_, selIdx);

    selIdx = -1;
    idx = 0;
    auto out = MidiOutput::getAvailableDevices();
    midiOutStr_.push_back("NONE");
    for (int i = 0; i < out.size(); i++) {
        if (!isInternalMidi(out[i].name)) {
            outDevices_.push_back(out[i]);
            midiOutStr_.push_back(std::to_string(idx) + ":" + out[i].name.toStdString());
            if (baseProcessor_->isActiveMidiOut(out[i].name.toStdString())) selIdx = idx + 1; //none
            idx++;
        }
    }

    midiOutCtrl_.setValues(midiOutStr_, selIdx);


    midiChStr_.push_back("OMNI");
    for (int i = 0; i < 16; i++) { midiChStr_.push_back(String(i + 1).toStdString()); }
    midiChannelCtrl_.setValues(midiChStr_, baseProcessor_->midiChannel());

    addAndMakeVisible(midiInCtrl_);
    addAndMakeVisible(midiOutCtrl_);
    addAndMakeVisible(midiChannelCtrl_);

    selIdx_ = 0;
    idxOffset_ = 0;

    noteInputBtn_.value(baseProcessor_->noteInput_);

    startTimer(50);
}


void SystemEditor::midiInCallback(float idx, const std::string &dev) {
//    Logger::writeToLog("midiInCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto device = inDevices_[i - 1];
        if (!isInternalMidi(device.name)) {
            baseProcessor_->setMidiIn(device.name.toStdString());
            return;
        }
    }
    baseProcessor_->setMidiIn("");
}

void SystemEditor::midiOutCallback(float idx, const std::string &dev) {
//    Logger::writeToLog("midiOutCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto device = outDevices_[i - 1];
        if (!isInternalMidi(device.name)) {
            baseProcessor_->setMidiOut(device.name.toStdString());
            return;
        }
    }
    baseProcessor_->setMidiOut("");
}

void SystemEditor::midiChannelCallback(float idx, const std::string &ch) {
    baseProcessor_->midiChannel(idx);
}


SystemEditor::~SystemEditor() {
    baseProcessor_->midiLearn(false);
    stopTimer();
}

void SystemEditor::timerCallback() {
    repaint();
}

void SystemEditor::paint(Graphics &g) {
    drawView(g);
}



void SystemEditor::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)

    drawLabel(g, "Param", 0);
    drawLabel(g, "Channel", 1);
    drawLabel(g, "Midi In", 2);
    drawLabel(g, "Midi Out", 3);


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
                type = "CHP";
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

static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


void SystemEditor::drawLabel(Graphics &g, const std::string &str, unsigned int idx) {
    const int fh = 32;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.setColour(Colours::red);

    //    unsigned h = 2 * paramSpaceY;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = idx * w + 10;
    unsigned y = paramTopY + 5 + fh * 2;
//    unsigned sp = 2;
    g.drawSingleLineText(str, x, y);
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
    int y = 50;
    midiChannelCtrl_.setBounds(750, y, 600, fh * 2);
    y += fh * 3;
    midiInCtrl_.setBounds(750, y, 600, fh * 2);
    y += fh * 3;
    midiOutCtrl_.setBounds(750, y, 600, fh * 2);
}

void SystemEditor::midiLearn(bool b) {
    baseProcessor_->midiLearn(b);
}

void SystemEditor::noteInput(bool b) {
    baseProcessor_->noteInput(b);
}

void SystemEditor::deleteAutomation(bool b) {
    if (!b) {
        if (selIdx_ >= 0) {
            auto &am = baseProcessor_->midiAutomation();
            if (am.empty() || selIdx_ >= am.size()) return;

            int idx = 0;
            for (auto ai = am.begin(); ai != am.end(); ai++) {
                auto &a = ai->second;
                if (idx == selIdx_) {
                    am.erase(a.paramIdx_);
                    if (selIdx_ != 0) {
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
            break;
        }
        case 1 : {
            if (v > 0) midiChannelCtrl_.inc(false);
            else midiChannelCtrl_.dec(false);
            break;
        }
        case 2 : {
            if (v > 0) midiInCtrl_.inc(false);
            else midiInCtrl_.dec(false);
            break;
        }
        case 3 : {
            if (v > 0) midiOutCtrl_.inc(false);
            else midiOutCtrl_.dec(false);
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
            if (!v) noteInputBtn_.toggle();
            break;
        }
        case 4: {
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
