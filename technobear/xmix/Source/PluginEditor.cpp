#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "ssp/editors/MultiView.h"

#include "LoadView.h"
#include "ModuleView.h"

// TODO
//  - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed



PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    // activeModule_ = 0;
    // moduleView_ = std::make_shared<ModuleView>(p);
    // loadView_ = std::make_shared<LoadView>(p);
    // addView(moduleView_);
    // addView(loadView_);

    // processor_.requestModuleChange(PluginProcessor::M_PRE, "comp");
    // processor_.requestModuleChange(PluginProcessor::M_MAIN, "plts");
    // processor_.requestModuleChange(PluginProcessor::M_POST, "clds");

    for(int i = 0; i< PluginProcessor::M_MAX;i++) {
        auto mV = std::make_shared<ModuleView>(p, i);
        addView(mV);
    }

    setView(PluginProcessor::M_MAIN);
}



void PluginEditor::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn, comboBtn, longPress);

    // if(longPress) returm;
    if(comboBtn == SSP_Up) {
        if(btn < PluginProcessor::M_MAX) {
            setView(btn);
        }
    }
}
