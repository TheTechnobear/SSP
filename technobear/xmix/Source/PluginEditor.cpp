#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "SSPApiEditor.h"
#include "ssp/editors/MultiView.h"

#include "MixerView.h"
#include "TrackView.h"

// TODO
//  - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed



PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {

    mixerView_= std::make_shared<MixerView>(p);
    mixerViewIdx_ = addView(mixerView_);
    trackView_= std::make_shared<TrackView>(p);
    trackViewIdx_ = addView(trackView_);
    setView(mixerViewIdx_);
}



void PluginEditor::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn,comboBtn,longPress);

}

