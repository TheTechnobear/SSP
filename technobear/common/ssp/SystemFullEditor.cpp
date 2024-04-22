#include <juce_audio_devices/juce_audio_devices.h>

#include "BaseProcessor.h"
#include "SystemEditor.h"

// #include "SSP.h"

namespace ssp {

// static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;

SystemFullEditor::SystemFullEditor(BaseProcessor *p)
    : SystemEditor(p),
      leftBtn_("PG-", nullptr, 32, Colours::red),
      rightBtn_("PG+", nullptr, 16 * COMPACT_UI_SCALE, Colours::red),
      upBtn_("EN-", nullptr, 32, Colours::red),
      downBtn_("EN+", nullptr, 16 * COMPACT_UI_SCALE, Colours::red),
      leftShiftBtn_("LS", nullptr, 32, Colours::grey, Colours::black),
      rightShiftBtn_("RS", nullptr, 16 * COMPACT_UI_SCALE, Colours::grey, Colours::black) {

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

}


SystemFullEditor::~SystemFullEditor() {
}

void SystemFullEditor::paint(Graphics &g) {
    drawView(g);
}


void SystemFullEditor::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)

    drawLabel(g, "Param", 0);
    drawLabel(g, "Channel", 1);
    drawLabel(g, "Midi In", 2);
    drawLabel(g, "Midi Out", 3);


    const int fh = 12 * COMPACT_UI_SCALE;
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

        if (selIdx_ == idx)
            g.setColour(Colours::yellow);
        else
            g.setColour(Colours::white);

        auto a = ai->second;
        auto p = plist[a.paramIdx_];

        String type = "None";
        switch (a.midi_.type_) {
            case BaseProcessor::MidiAutomation::Midi::T_CC: type = "CC"; break;
            case BaseProcessor::MidiAutomation::Midi::T_NOTE: type = "Note"; break;
            case BaseProcessor::MidiAutomation::Midi::T_PRESSURE: type = "CHP"; break;
            default: break;
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


void SystemFullEditor::drawLabel(Graphics &g, const std::string &str, unsigned int idx) {
    const int fh = 16 * COMPACT_UI_SCALE;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.setColour(Colours::red);

    //    unsigned h = 2 * paramSpaceY;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = idx * w + 10;
    unsigned y = paramTopY + 5 + fh * 2;
    //    unsigned sp = 2;
    g.drawSingleLineText(str, x, y);
}

void SystemFullEditor::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
    const int w = 100;
    const int h = btnSpaceY;
    unsigned x = 900 + (c * w);
    unsigned y = btnTopY + (r * h);
    btn.setBounds(x, y, w, h);
}


void SystemFullEditor::resized() {
    const int fh = 32;
    int y = 50;
    midiChannelCtrl_.setBounds(750, y, 600, fh * 2);
    y += fh * 3;
    midiInCtrl_.setBounds(750, y, 600, fh * 2);
    y += fh * 3;
    midiOutCtrl_.setBounds(750, y, 600, fh * 2);
}


void SystemFullEditor::onLeftButton(bool v) {
    leftBtn_.onButton(v);
}

void SystemFullEditor::onRightButton(bool v) {
    rightBtn_.onButton(v);
}

void SystemFullEditor::onUpButton(bool v) {
    upBtn_.onButton(v);
}

void SystemFullEditor::onDownButton(bool v) {
    downBtn_.onButton(v);
}

void SystemFullEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.onButton(v);
}

void SystemFullEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.onButton(v);
}

}  // namespace ssp
