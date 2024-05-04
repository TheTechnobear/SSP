#include "PluginEditor.h"

#include "MixerView.h"
#include "OptionEditor.h"
#include "PerformanceEditor.h"
#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "TrackEditor.h"


PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    mixerView_ = std::make_shared<MixerView>(p);
    mixerViewIdx_ = addView(mixerView_);
    trackEditor_ = std::make_shared<TrackEditor>(p);
    trackEditorIdx_ = addView(trackEditor_);
    optionEditor_ = std::make_shared<OptionEditor>(p);
    optionViewIdx_ = addView(optionEditor_);
    perfEditor_ = std::make_shared<PerformanceEditor>(p);
    perfEditorIdx_ = addView(perfEditor_);

    setView(mixerViewIdx_);
}


void PluginEditor::eventButton(unsigned btn, bool longPress) {
    auto v = getViewIdx();
    if (v == mixerViewIdx_) {
        int trackIdx = btn;
        if (btn < PluginProcessor::MAX_TRACKS) {
            trackEditor_->trackIdx(trackIdx);
            setView(trackEditorIdx_);
            return;
        }
    }
    base_type::eventButton(btn, longPress);
}


void PluginEditor::eventUp(bool longPress) {
    auto v = getViewIdx();
    if (longPress) {
        if (v == mixerViewIdx_) {
            setView(optionViewIdx_);
            return;
        } else {
            setView(mixerViewIdx_);
            return;
        }
    }
    base_type::eventUp(longPress);
}

void PluginEditor::eventDown(bool longPress) {
    auto v = getViewIdx();
    if (longPress) {
        if (v != perfEditorIdx_) {
            setView(perfEditorIdx_);
            return;
        }
    } else {
        if (v == mixerViewIdx_) {
            setView(perfEditorIdx_);
            return;
        }
    }
    base_type::eventDown(longPress);
}
