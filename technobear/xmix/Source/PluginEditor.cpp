#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "ssp/editors/MultiView.h"

#include "LoadView.h"
#include "ModuleView.h"

// TODO
//  - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed



PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    activeModule_ = 0;
    moduleView_ = std::make_shared<ModuleView>(p);
    loadView_ = std::make_shared<LoadView>(p);
    addView(moduleView_);
    addView(loadView_);
    setView(0);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}


void PluginEditor::onSSPTimer() {
    base_type::onSSPTimer();
    if (upDownCounter_[0] > 0) { upDownCounter_[0]--; }
    if (upDownCounter_[1] > 0) { upDownCounter_[1]--; }

    auto editor = processor_.getEditor(PluginProcessor::M_MAIN);
    if (!editor) return;
    editor->frameStart();
}

void PluginEditor::editorShown() {
    base_type::editorShown();
    auto editor = processor_.getEditor(PluginProcessor::M_MAIN);
    if (!editor) return;
    editor->visibilityChanged(true);
}

void PluginEditor::editorHidden() {
    base_type::editorHidden();
    auto editor = processor_.getEditor(PluginProcessor::M_MAIN);
    if (!editor) return;
    editor->visibilityChanged(false);
}


void PluginEditor::onEncoderSwitch(unsigned int enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    if (v) return;

    if (getViewIdx() == 1) {
        // load view
        setView(0);
    }
}

void PluginEditor::onButton(unsigned btn, bool v) {
    base_type::onButton(btn, v);
    if (v) return;

    if (getViewIdx() == 1) {
        switch (btn) {
            case 3:
            case 7: {
                setView(0);
            }
        }
    }
}


void PluginEditor::onUpButton(bool v) {
    static constexpr int UP_BTN = 0;

    int viewIdx = getViewIdx();
    if (viewIdx == 0) {
        // module view
        if (upDownState_[UP_BTN] != v) {
            // transition state
            if (v) {
                // on press
                upDownCounter_[UP_BTN] = MAX_COUNTER;
                // base_type::onUpButton(v);
            }
            if (!v) {
                // on release
                if (upDownCounter_[UP_BTN > 0]) {
                    // short press
                    base_type::onUpButton(true);
                    base_type::onUpButton(false);
                } else {
                    // long press
                    viewIdx = !viewIdx;
                    setView(viewIdx);
                }
            }
            upDownState_[UP_BTN] = v;
        }
    } else {
        // load view
        base_type::onUpButton(v);
    }
}


void PluginEditor::onDownButton(bool v) {
    base_type::onDownButton(v);
}

void PluginEditor::resized() {
    base_type::resized();
}

