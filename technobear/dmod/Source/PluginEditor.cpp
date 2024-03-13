#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "ssp/editors/MultiView.h"

#include "DualView.h"
#include "LoadView.h"

PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    dualView_ = std::make_shared<DualView>(p);
    loadView_ = std::make_shared<LoadView>(p);
    addView(dualView_);
    addView(loadView_);
    dualView_->moduleIdx(0);
    setView(0);
}

void PluginEditor::onSSPTimer() {
    base_type::onSSPTimer();
    for (int i = 0; i < PluginProcessor::M_MAX; i++) {
        //        auto editor = processor_.createEditorIfNeeded(i);
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->frameStart();
    }
}

void PluginEditor::editorShown() {
    base_type::editorShown();
    for (int i = 0; i < PluginProcessor::M_MAX; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->visibilityChanged(true);
    }
}

void PluginEditor::editorHidden() {
    base_type::editorHidden();
    for (int i = 0; i < PluginProcessor::M_MAX; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->visibilityChanged(false);
    }
}

void  PluginEditor::eventLeft(bool lp) {
    dualView_->moduleIdx(PluginProcessor::M_LEFT);
    setView(0);
}

void  PluginEditor::eventRight(bool lp) {
    dualView_->moduleIdx(PluginProcessor::M_RIGHT);
    setView(0);
}

void  PluginEditor::eventLeftShift(bool lp) {
    dualView_->moduleIdx(PluginProcessor::M_LEFT);
    loadView_->moduleIdx(PluginProcessor::M_LEFT);
    if(getViewIdx()==0) {
        // module -> load 
        setView(1);
    } else {
        setView(0);
    }
}

void  PluginEditor::eventRightShift(bool lp) {
    dualView_->moduleIdx(PluginProcessor::M_RIGHT);
    loadView_->moduleIdx(PluginProcessor::M_RIGHT);
    if(getViewIdx()==0) {
        // module -> load 
        setView(1);
    } else {
        setView(0);
    }
}