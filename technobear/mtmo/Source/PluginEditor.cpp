
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

void PluginEditor::timerCallback() {
    MidiMessage msg;
    while (processor_.messageQueue().try_dequeue(msg)) {
        Logger::writeToLog("handleIncomingMidiMessage" + String(msg.getDescription()));
    }
}


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    setSize(1600, 480);
}

void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}


void PluginEditor::resized() {
}


