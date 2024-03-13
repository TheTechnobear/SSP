#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "ssp/editors/MultiView.h"

#include "LoadView.h"
#include "ModuleView.h"

// TODO
//  - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed



PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {

    for(int i = 0; i< PluginProcessor::M_MAX;i++) {
        auto mV = std::make_shared<ModuleView>(p, i);
        addView(mV);
    }
    loadView_ = std::make_shared<LoadView>(processor_);
    addView(loadView_);

    setView(PluginProcessor::M_MAIN);
}



void PluginEditor::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn, comboBtn, longPress);

    // if(longPress) returm;

    if(comboBtn == SSP_Up) {
        if(btn < PluginProcessor::M_MAX) {
           int moduleIdx = btn;
            if(moduleIdx == getViewIdx()) {
                if(loadView_ ) {
                    loadView_->moduleIdx(moduleIdx);
                    setView(LOAD_VIEW);
                }
            } else {
                setView(moduleIdx);
            }
            
        } else if(LOAD_VIEW == getViewIdx()) {
                if(loadView_ ) {
                    int moduleIdx = loadView_->moduleIdx();
                    if(moduleIdx < PluginProcessor::M_MAX) {
                        setView(moduleIdx);
                    }
                }
        }
    }
}
