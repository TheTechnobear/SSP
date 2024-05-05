#include "PerformanceEditor.h"

#include "PerformanceAdd.h"
#include "PerformanceEdit.h"
#include "PerformanceView.h"

PerformanceEditor::PerformanceEditor(PluginProcessor &p) : base_type(&p, true), processor_(p) {
    perfView_ = std::make_shared<PerformanceView>(p);
    perfViewIdx_ = addView(perfView_);
    editView_ = std::make_shared<PerformanceEdit>(p);
    editViewIdx_ = addView(editView_);
    addView_ = std::make_shared<PerformanceAdd>(p);
    addViewIdx_ = addView(addView_);
    setView(perfViewIdx_);
}

PerformanceEditor::~PerformanceEditor() {
}

void PerformanceEditor::resized() {
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    base_type::resized();
    auto r = juce::Rectangle<int>(0, 0, pluginWidth, pluginHeight);
    perfView_->setBounds(r);
    editView_->setBounds(r);
    addView_->setBounds(r);
}

void PerformanceEditor::editorShown() {
    setView(perfViewIdx_);
    base_type::editorShown();
}

void PerformanceEditor::eventUp(bool longPress) {
    if (!longPress) {
        auto v = getViewIdx();
        if (v == editViewIdx_) {
            setView(perfViewIdx_);
            return;
        } else if (v == addViewIdx_) {
            setView(editViewIdx_);
            return;
        }
    }
    base_type::eventUp(longPress);
}

void PerformanceEditor::eventDown(bool longPress) {
    base_type::eventDown(longPress);
}

void PerformanceEditor::eventButtonHeld(unsigned btn) {
    auto v = getViewIdx();
    if (v == perfViewIdx_) {
        setView(editViewIdx_);
        return;
    } else {
        setView(perfViewIdx_);
        return;
    }
    base_type::eventButtonHeld(btn);
}


void PerformanceEditor::eventButton(unsigned btn, bool longPress) {
    auto v = getViewIdx();
    if (v == editViewIdx_) {
        if (btn == 0) {
            setView(addViewIdx_);
            return;
        }
    } else if (v == addViewIdx_) {
        if (btn == 3) {
            setView(editViewIdx_);
            return;
        }
    }
    base_type::eventButton(btn, longPress);
}


void PerformanceEditor::onEncoderSwitch(unsigned id, bool v) {
    base_type::onEncoderSwitch(id, v);

    auto view = getViewIdx();
    if (!v && view == addViewIdx_) {
        setView(editViewIdx_);
        return;
    }
}
