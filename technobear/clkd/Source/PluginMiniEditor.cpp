#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p),
      runButton_("Run", nullptr, 16 * COMPACT_UI_SCALE, Colours::green),
      resetButton_("Reset", nullptr, 16 * COMPACT_UI_SCALE, Colours::yellow),
      useTrigsButton_("TrigSync", nullptr, 10 * COMPACT_UI_SCALE, Colours::cyan) {
    addParam(std::make_shared<pcontrol_type>(processor_.params_.source, 1.0f, 1.0f));
    addParam(std::make_shared<pcontrol_type>(processor_.params_.clkindiv, 1.0f, 0.25f));
    addParam(std::make_shared<pcontrol_type>(processor_.params_.bpm, 10.0f, 0.1f));
    addParam(std::make_shared<pcontrol_type>(processor_.params_.midippqn, 1.0f, 1.0f));

    for (int d = 0; d < PluginProcessor::MAX_CLK_OUT; d++) {
        addParam(std::make_shared<pcontrol_type>(processor_.params_.divisions_[d]->val, 1, 0.25));
    }


    setButtonBounds(BN_RUN, &runButton_);
    setButtonBounds(BN_RESET, &resetButton_);
    setButtonBounds(BN_USETRIGS, &useTrigsButton_);

    addAndMakeVisible(runButton_);
    addAndMakeVisible(resetButton_);
    addAndMakeVisible(useTrigsButton_);
}


void PluginMiniEditor::resized() {
    base_type::resized();
    setButtonBounds(BN_RUN, &runButton_);
    setButtonBounds(BN_RESET, &resetButton_);
    setButtonBounds(BN_USETRIGS, &useTrigsButton_);
}

void PluginMiniEditor::onButton(unsigned int btn, bool v) {
    base_type::onButton(btn, v);
    switch (btn) {
        case BN_RUN: {
            runButton_.onButton(v);
            break;
        }
        case BN_RESET: {
            resetButton_.onButton(v);
            break;
        }
        case BN_USETRIGS: {
            useTrigsButton_.onButton(v);
            break;
        }
    }
}


void PluginMiniEditor::eventButton(unsigned int btn, bool longPress) {
    base_type::eventButton(btn, longPress);
    switch (btn) {
        case BN_RUN: {
            processor_.toggleRunRequest();
            break;
        }
        case BN_RESET: {
            processor_.requestReset();
            break;
        }
        case BN_USETRIGS: {
            processor_.toggleUseTrigs();
            break;
        }
    }
}