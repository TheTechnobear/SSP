
#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {
    activeEngine_ = 0;
    upBtn_.label("AG-");
    downBtn_.label("AG+");
    leftBtn_.label("IN-");

    rightBtn_.label("IN+");
    setSize(1600, 480);
}

void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    g.setColour(Colours::grey);
    g.drawSingleLineText("Instance : " + String(activeEngine_), 20, 60);
    processor_.algo(activeEngine_)->paint(g);
}

void PluginEditor::onEncoder(unsigned enc, float v) {
    processor_.algo(activeEngine_)->encoder(enc, v > 0.0f ? 1 : -1);
}

void PluginEditor::onEncoderSwitch(unsigned enc, bool v) {
    processor_.algo(activeEngine_)->encswitch(enc, v);
}

void PluginEditor::onButton(unsigned btn, bool v) {
    processor_.algo(activeEngine_)->button(btn, v);
}

void PluginEditor::onLeftButton(bool v) {
    base_type::onLeftButton(v);
    if (!v) {
        if (activeEngine_ > 0) activeEngine_--;
        else activeEngine_ = PluginProcessor::MAX_ENG - 1;
    }
}

void PluginEditor::onRightButton(bool v) {
    base_type::onRightButton(v);
    if (!v) {
        if (activeEngine_ + 1 < PluginProcessor::MAX_ENG) activeEngine_++;
        else activeEngine_ = 0;
    }
}

void PluginEditor::onUpButton(bool v) {
    base_type::onUpButton(v);
    if (!v) processor_.prevAlgo(activeEngine_);
}

void PluginEditor::onDownButton(bool v) {
    base_type::onDownButton(v);
    if (!v) processor_.nextAlgo(activeEngine_);
}



