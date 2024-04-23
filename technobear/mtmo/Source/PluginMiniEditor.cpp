#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p,true), processor_(p) {
}


void PluginMiniEditor::onSSPTimer() {
    base_type::onSSPTimer();
    clock_ = false;
    MidiMessage msg;
    while (processor_.messageQueue().try_dequeue(msg)) {
        auto &data = dataBuf_[wrPos_];

        if (msg.isNoteOn(false)) {
            data.channel_ = String(msg.getChannel());
            data.active_ = true;
            data.type_ = "NoteOn";
            data.num_ = String(msg.getNoteNumber());
            data.value_ = String(msg.getVelocity());
            data.dispNum_ = true;
            wrPos_ = (wrPos_ + 1) % MAX_DATA;
        } else if (msg.isNoteOff(true)) {
            data.channel_ = String(msg.getChannel());
            data.active_ = true;
            data.type_ = "NoteOff";
            data.num_ = String(msg.getNoteNumber());
            data.value_ = String(msg.getVelocity());
            data.dispNum_ = true;
            wrPos_ = (wrPos_ + 1) % MAX_DATA;
        } else if (msg.isController()) {
            data.channel_ = String(msg.getChannel());
            data.active_ = true;
            data.type_ = "CC";
            data.num_ = String(msg.getControllerNumber());
            data.value_ = String(msg.getControllerValue());
            data.dispNum_ = true;
            wrPos_ = (wrPos_ + 1) % MAX_DATA;
        } else if (msg.isAftertouch()) {
            data.channel_ = String(msg.getChannel());
            data.active_ = true;
            data.type_ = "AT";
            data.num_ = String(msg.getNoteNumber());
            data.value_ = String(msg.getAfterTouchValue());
            data.dispNum_ = true;
            wrPos_ = (wrPos_ + 1) % MAX_DATA;
        } else if (msg.isPitchWheel()) {
            data.channel_ = String(msg.getChannel());
            data.active_ = true;
            data.type_ = "PB";
            data.value_ = String(msg.getPitchWheelValue());
            data.dispNum_ = false;
            wrPos_ = (wrPos_ + 1) % MAX_DATA;
        } else if (msg.isChannelPressure()) {
            data.channel_ = String(msg.getChannel());
            data.active_ = true;
            data.type_ = "C Pr";
            data.value_ = String(msg.getChannelPressureValue());
            data.dispNum_ = false;
            wrPos_ = (wrPos_ + 1) % MAX_DATA;
        } else if (msg.isMidiClock()) {
            clock_ = true;
        }
    }
}

void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);

    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 10 * COMPACT_UI_SCALE, Font::plain));

    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + ":" + String(JucePlugin_Desc) + String(" @ thetechnobear"), gap,
                         gap * 2);


    const int fh = 12 * COMPACT_UI_SCALE;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    int y = fh * 3;
    g.setColour(Colours::green);
    int chX = 5 * COMPACT_UI_SCALE;
    int msgX = chX + (fh * 2);
    int numX = msgX + (fh * 5);
    int valX = numX + (fh * 4);

    g.drawSingleLineText("Ch", chX, y);
    g.drawSingleLineText("Msg", msgX, y);
    g.drawSingleLineText("Num", numX, y);
    g.drawSingleLineText("Value", valX, y);

    int x = SSP_COMPACT_WIDTH - fh * 10;
    g.drawSingleLineText("Clock", x, y);
    g.setColour(Colours::yellow);
    if (clock_) {
        g.fillRect(x, y + fh / 2, 50, fh);
    } else {
        g.drawRect(x, y + fh / 2, 50, fh, 2);
    }

    y += fh;

    g.setColour(Colours::white);

    static const int MAX_LINES = 15;
    for (int i = MAX_LINES; i > 0; i--) {
        int idx = (wrPos_ + MAX_DATA - i) % MAX_DATA;
        auto data = dataBuf_[idx];
        if (data.active_) {
            g.drawSingleLineText(data.channel_, chX, y);
            g.drawSingleLineText(data.type_, msgX, y);
            if (data.dispNum_) g.drawSingleLineText(data.num_, numX, y);
            g.drawSingleLineText(data.value_, valX, y);
            y += fh;
        }
    }
}