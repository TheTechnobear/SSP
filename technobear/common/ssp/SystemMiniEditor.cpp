#include <juce_audio_devices/juce_audio_devices.h>

#include "BaseProcessor.h"
#include "SystemEditor.h"


namespace ssp {

SystemMiniEditor::SystemMiniEditor(BaseProcessor *p) : SystemEditor(p) {
    midiInCtrl_.setFontHeight(14 * COMPACT_UI_SCALE);
    midiOutCtrl_.setFontHeight(14 * COMPACT_UI_SCALE);
    midiChannelCtrl_.setFontHeight(14 * COMPACT_UI_SCALE);
    mode(M_PARAM);
}


SystemMiniEditor::~SystemMiniEditor() {
}

void SystemMiniEditor::paint(Graphics &g) {
    drawView(g);
}

void SystemMiniEditor::drawButtonBox(Graphics &g) {
    g.setColour(Colours::darkgrey);
    unsigned singleButtonH = buttonBarH / 2;
    g.drawHorizontalLine(SSP_COMPACT_HEIGHT - buttonBarH - 1, gap, SSP_COMPACT_WIDTH - gap);
    g.drawHorizontalLine(SSP_COMPACT_HEIGHT - singleButtonH - 1, gap, SSP_COMPACT_WIDTH - gap);
    g.drawHorizontalLine(SSP_COMPACT_HEIGHT - 1, gap, SSP_COMPACT_WIDTH - gap);

    for (int i = 0; i < nParamsPerPage + 1; i++) {
        unsigned x = gap + (i * gridW);
        g.drawVerticalLine(x, SSP_COMPACT_HEIGHT - buttonBarH, SSP_COMPACT_HEIGHT - 1);
    }
}

void SystemMiniEditor::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
    int offset = gap + (gap / 2);
    int w = paramWidth;
    int h = buttonBarH / 2;
    int x = offset + (c * gridW);
    int y = SSP_COMPACT_HEIGHT - buttonBarH + (r * h);
    btn.setBounds(x, y, w, h);
}


void SystemMiniEditor::drawView(Graphics &g) {
    drawButtonBox(g);
    if (SystemEditor::mode() == M_PARAM) {
        drawLabel(g, "Param", 0);
        drawLabel(g, "Scale", 1);
        drawLabel(g, "Offset", 2);
    } else {
        drawLabel(g, "Channel", 0);
        drawLabel(g, "Midi In", 1);
        drawLabel(g, "Midi Out", 2);
    }

    if (SystemEditor::mode() == M_DEVICE) return;

    const int fh = 12 * COMPACT_UI_SCALE;
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, Font::plain)));
    g.setColour(Colours::white);

    int y = fh * 3;
    int xparam = 5 * COMPACT_UI_SCALE;
    int xch = xparam + (8 * fh);
    int xtype = xch + (3 * fh);
    int xnum = xtype + (3 * fh);
    int xscale = xnum + (3 * fh);

    g.setColour(Colours::red);
    g.drawSingleLineText("Parameter", xparam, y);
    g.drawSingleLineText(String("Ch"), xch, y);
    g.drawSingleLineText("Type", xtype, y);
    g.drawSingleLineText(String("Num"), xnum, y);
    g.drawSingleLineText(String("Scaling"), xscale, y);


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
        g.drawSingleLineText(p->getName(40), xparam, y);
        g.drawSingleLineText(String(a.midi_.channel_), xch, y);
        g.drawSingleLineText(type, xtype, y);
        g.drawSingleLineText(String(a.midi_.num_), xnum, y);
        String scaling = String("x ") + String(a.scale_, 2, false) + " + " + String(a.offset_, 2, false);
        g.drawSingleLineText(scaling, xscale, y);

        if (ai != am.end()) ai++;
        idx++;
        y += fh;
    }
}


void SystemMiniEditor::drawLabel(Graphics &g, const std::string &str, unsigned int idx) {
    static constexpr int fh = 14 * COMPACT_UI_SCALE;
    const int btnTop = SSP_COMPACT_HEIGHT - (fh * 2);

    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, Font::plain)));
    g.setColour(Colours::red);

    unsigned w = unsigned(SSP_COMPACT_WIDTH / 4.0f);
    unsigned x = idx * w + (5 * COMPACT_UI_SCALE);
    unsigned y = btnTop - (fh);

    g.drawSingleLineText(str, x, y);
}



void SystemMiniEditor::resized() {
    const int fh = 12 * COMPACT_UI_SCALE;
    int y = fh * 2;
    int w = SSP_COMPACT_WIDTH / 3;
    int x = 5 * COMPACT_UI_SCALE;

    midiChannelCtrl_.setBounds(x, y, w, fh * 2);
    y += fh * 2;
    midiInCtrl_.setBounds(x, y, w, fh * 2);
    y += fh * 2;
    midiOutCtrl_.setBounds(x, y, w, fh * 2);

    setButtonBounds(learnBtn_, 0, 0);
    setButtonBounds(noteInputBtn_, 0, 1);
    setButtonBounds(delBtn_, 1, 0);
    setButtonBounds(deviceMode_, 0, 3);
    setButtonBounds(paramMode_, 0, 3);
}

void SystemMiniEditor::mode(UI_Mode m) {
    SystemEditor::mode(m);
    if (m == M_PARAM) {
        midiChannelCtrl_.setVisible(false);
        midiInCtrl_.setVisible(false);
        midiOutCtrl_.setVisible(false);
    } else {
        midiChannelCtrl_.setVisible(true);
        midiInCtrl_.setVisible(true);
        midiOutCtrl_.setVisible(true);
    }
}


}  // namespace ssp
