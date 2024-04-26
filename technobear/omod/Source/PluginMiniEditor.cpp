#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p), processor_(p), resetButton_("Reset", nullptr, 16 * COMPACT_UI_SCALE, Colours::yellow) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    addParam(std::make_shared<pcontrol_type>(processor_.params_.freq, 50.0f, 0.01f));
    addParam(std::make_shared<pcontrol_type>(processor_.params_.wave, 1.0, 1.0f));
    addParam(std::make_shared<pcontrol_type>(processor_.params_.amp, 0.1, 0.01));
    addParam(std::make_shared<pcontrol_type>(processor_.params_.phase, 10.0f, 0.1));

    for (unsigned oid = 0; oid < PluginProcessor::MAX_S_OSC; oid++) {
        auto &op = processor_.getSlaveOscParam(oid);
        addParam(std::make_shared<pcontrol_type>(op.ratio, 0.1f, 0.01f));
        addParam(std::make_shared<pcontrol_type>(op.wave, 1.0, 1.0f));
        addParam(std::make_shared<pcontrol_type>(op.amp, 0.1, 0.01));
        addParam(std::make_shared<pcontrol_type>(op.phase, 10.0f, 0.1));
    }

    addButton(nullptr);
    addButton(std::make_shared<bcontrol_type>(processor_.params_.lfo, 12 * COMPACT_UI_SCALE, Colours::lightskyblue));

    setButtonBounds(0, &resetButton_);
    addAndMakeVisible(resetButton_);
}


void PluginMiniEditor::resized() {
    base_type::resized();
    setButtonBounds(0, &resetButton_);
}

void PluginMiniEditor::onButton(unsigned int id, bool v) {
    base_type::onButton(id, v);
    if (id == 0) resetButton_.onButton(v);
}


void PluginMiniEditor::eventButton(unsigned int id, bool longPress) {
    base_type::eventButton(id, longPress);
    if (id == 0) processor_.reset();
}