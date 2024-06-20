#include "PluginEditor.h"

#include "DualView.h"
#include "LoadModuleView.h"
#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "ssp/editors/MultiView.h"

PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    dualView_ = std::make_shared<DualView>(p);
    loadView_ = std::make_shared<LoadModuleView>(p, true);
    dualViewIdx_ = addView(dualView_);
    loadViewIdx_ = addView(loadView_);
    dualView_->moduleIdx(0, 0);
    setView(dualViewIdx_);
}

void PluginEditor::onSSPTimer() {
    base_type::onSSPTimer();
    for (int i = 0; i < Track::M_MAX; i++) {
        auto editor = processor_.getEditor(0, i);
        if (editor) {
            editor->frameStart();
            auto pComponent = editor->editorComponent();
            pComponent->repaint();
        }
    }
}

void PluginEditor::editorShown() {
    base_type::editorShown();
    for (int i = 0; i < Track::M_MAX; i++) {
        auto editor = processor_.getEditor(0, i);
        if (!editor) continue;
        auto pComponent = editor->editorComponent();
        // todo check?
        pComponent->setVisible(true);
    }
}

void PluginEditor::editorHidden() {
    base_type::editorHidden();
    for (int i = 0; i < Track::M_MAX; i++) {
        auto editor = processor_.getEditor(0, i);
        if (!editor) continue;
        auto pComponent = editor->editorComponent();
        // todo check?
        pComponent->setVisible(false);
    }
}

void PluginEditor::eventLeft(bool lp) {
    dualView_->moduleIdx(0, Track::M_LEFT);
    setView(dualViewIdx_);
}

void PluginEditor::eventRight(bool lp) {
    dualView_->moduleIdx(0, Track::M_RIGHT);
    setView(dualViewIdx_);
}

void PluginEditor::eventLeftShift(bool lp) {
    dualView_->moduleIdx(0, Track::M_LEFT);
    loadView_->moduleIdx(0, Track::M_LEFT);
    if (getViewIdx() == dualViewIdx_) {
        setView(loadViewIdx_);
    } else {
        setView(dualViewIdx_);
    }
}

void PluginEditor::eventRightShift(bool lp) {
    dualView_->moduleIdx(0, Track::M_RIGHT);
    loadView_->moduleIdx(0, Track::M_RIGHT);
    if (getViewIdx() == dualViewIdx_) {
        setView(loadViewIdx_);
    } else {
        setView(dualViewIdx_);
    }
}

void PluginEditor::eventButton(unsigned btn, bool longPress) {
    if (!longPress) {
        if (getViewIdx() == loadViewIdx_) {
            loadView_->eventButton(btn, longPress);
            setView(dualViewIdx_);
            return;
        }
    }

    base_type::eventButton(btn, longPress);
}
