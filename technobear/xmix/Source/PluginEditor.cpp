#include "PluginEditor.h"

#include "MixerView.h"
#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "TrackEditor.h"
#include "ssp/editors/MultiView.h"

// TODO - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed

PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    mixerView_ = std::make_shared<MixerView>(p);
    mixerViewIdx_ = addView(mixerView_);
    trackEditor_ = std::make_shared<TrackEditor>(p);
    trackEditorIdx_ = addView(trackEditor_);
    setView(mixerViewIdx_);
}


void PluginEditor::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    auto v = getViewIdx();
    if (v == mixerViewIdx_) {
        if (comboBtn == SSP_Up) {
            if (btn < PluginProcessor::MAX_TRACKS) {
                trackEditor_->trackIdx(btn);
                setView(trackEditorIdx_);
                return;
            }
        }
    }
    base_type::eventButtonCombo(btn, comboBtn, longPress);
}


void PluginEditor::eventUp(bool longPress) {
    auto v = getViewIdx();
    if (v == trackEditorIdx_ && longPress) {
        setView(mixerViewIdx_);
        return;
    }

    base_type::eventUp(longPress);
}
