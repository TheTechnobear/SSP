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


