
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"


void PluginEditor::timerCallback() {
    base_type::timerCallback();
    clock_ = false;
    MidiMessage msg;
    while (processor_.messageQueue().try_dequeue(msg)) {
//        Logger::writeToLog("timerCallback -> " + msg.getDescription());
        auto &data = dataBuf_[wrPos_];

        data.channel_ = String(msg.getChannel());
        data.active_ = true;
        if (msg.isNoteOnOrOff()) {
            data.type_ = "Note";
            data.num_ = String(msg.getNoteNumber());
            data.value_ = String(msg.getVelocity());
            data.dispNum_ = true;
        } else if (msg.isController()) {
            data.type_ = "CC";
            data.num_ = String(msg.getControllerNumber());
            data.value_ = String(msg.getControllerValue());
            data.dispNum_ = true;
        } else if (msg.isAftertouch()) {
            data.type_ = "AT";
            data.num_ = String(msg.getNoteNumber());
            data.value_ = String(msg.getAfterTouchValue());
            data.dispNum_ = true;
        } else if (msg.isChannelPressure()) {
            data.type_ = "PB";
            data.value_ = String(msg.getPitchWheelValue());
            data.dispNum_ = false;
        } else if (msg.isChannelPressure()) {
            data.type_ = "C Pr";
            data.value_ = String(msg.getAfterTouchValue());
            data.dispNum_ = false;
        } else if (msg.isMidiClock()) {
            clock_ = true;
        }
        wrPos_ = (wrPos_ + 1) % MAX_DATA;
    }
}


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    setSize(1600, 480);
}

void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);

    const int fh = 24;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    int y = 60;
    g.setColour(Colours::green);
    g.drawSingleLineText("Ch", 20, y);
    g.drawSingleLineText("Msg", 80, y);
    g.drawSingleLineText("Num", 140, y);
    g.drawSingleLineText("Value", 200, y);

    int x = 1400;
    g.drawSingleLineText("Clock", x, y);
    g.setColour(Colours::yellow);
    if (clock_) {
        g.fillRect(x, y + fh / 2, 50, fh);
    } else {
        g.drawRect(x, y + fh / 2, 50, fh, 2);
    }

    y += fh;

    g.setColour(Colours::white);

    for (int i = 16; i > 0; i--) {
        int idx = (wrPos_ + MAX_DATA - i) % MAX_DATA;
        auto data = dataBuf_[idx];
        if (data.active_) {
            g.drawSingleLineText(data.channel_, 20, y);
            g.drawSingleLineText(data.type_, 80, y);
            if (data.dispNum_) g.drawSingleLineText(data.num_, 140, y);
            g.drawSingleLineText(data.value_, 200, y);
            y += fh;
        }
    }


}



