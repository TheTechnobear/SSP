#include "TrackEditor.h"

#include "LoadModuleView.h"
#include "MatrixView.h"
#include "ModuleView.h"
#include "TrackView.h"


TrackEditor::TrackEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    trackView_ = std::make_shared<TrackView>(processor_);
    trackViewIdx_ = addView(trackView_);

    moduleView_ = std::make_shared<ModuleView>(processor_);
    moduleViewIdx_ = addView(moduleView_);

    loadModuleView_ = std::make_shared<LoadModuleView>(processor_, true);
    loadViewIdx_ = addView(loadModuleView_);

    matrixView_ = std::make_shared<MatrixView>(processor_);
    matrixViewIdx_ = addView(matrixView_);
}


TrackEditor::~TrackEditor() {
}


void TrackEditor::resized() {
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    base_type::resized();
    trackView_->setBounds(0, 0, pluginWidth, pluginHeight);
    moduleView_->setBounds(0, 0, pluginWidth, pluginHeight);
    loadModuleView_->setBounds(0, 0, pluginWidth, pluginHeight);
    matrixView_->setBounds(0, 0, pluginWidth, pluginHeight);
}

void TrackEditor::editorShown() {
    trackView_->trackIdx(trackIdx_);
    setView(trackViewIdx_);
    base_type::editorShown();
}

void TrackEditor::eventUp(bool longpress) {
    auto v = getViewIdx();

    if (!longpress) {
        if (v == loadViewIdx_) {
            setView(matrixViewIdx_);
            return;
        } else if (v == matrixViewIdx_) {
            setView(trackViewIdx_);
            return;
        }
    }
    base_type::eventUp(longpress);
}


void TrackEditor::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    if (getViewIdx() == loadViewIdx_ && enc == 1 && loadModuleView_->moduleUpdated()) {
        trackView_->trackIdx(trackIdx_);
        setView(trackViewIdx_);
    }
}

void TrackEditor::eventButton(unsigned btn, bool longPress) {
    if (!longPress) {
        auto v = getViewIdx();
        if (v == trackViewIdx_) {
            if (btn < Track::MAX_USER_MODULES) {
                auto modIdx = btn + Track::M_SLOT_1;
                moduleView_->moduleIdx(trackIdx_, modIdx);
                setView(moduleViewIdx_);
                return;
            }
        } else if (v == loadViewIdx_) {
            // return view on load and clear
            switch (btn) {
                case 3:    // load
                case 7: {  // clear
                    // carry out action
                    loadModuleView_->eventButton(btn, longPress);
                    // return to matrix view
                    matrixView_->moduleIdx(matrixView_->trackIdx(), matrixView_->moduleIdx());
                    setView(matrixViewIdx_);
                    return;
                }
            }
        } else if (v == matrixViewIdx_) {
            switch (btn) {
                case 0: {
                    loadModuleView_->moduleIdx(matrixView_->trackIdx(), matrixView_->moduleIdx());
                    setView(loadViewIdx_);
                    return;
                }
            }
        }
    }
    base_type::eventButton(btn, longPress);
}

void TrackEditor::eventButtonHeld(unsigned btn) {
    if (btn >= SSP_Soft_1 && btn <= SSP_Soft_4) {
        int m = btn - SSP_Soft_1;
        if (getViewIdx() == trackViewIdx_ && btn < Track::MAX_USER_MODULES) {
            auto modIdx = m + Track::M_SLOT_1;
            loadModuleView_->moduleIdx(trackIdx_, modIdx + 1);
            matrixView_->moduleIdx(trackIdx_, modIdx);
            moduleView_->moduleIdx(trackIdx_, modIdx);
            setView(matrixViewIdx_);
            return;
        }
    }
    base_type::eventButtonHeld(btn);
}


void TrackEditor::trackIdx(unsigned t) {
    trackIdx_ = t;
    trackView_->trackIdx(trackIdx_);
}
