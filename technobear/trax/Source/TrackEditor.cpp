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
}

void TrackEditor::eventUp(bool longpress) {
    auto v = getViewIdx();

    if (!longpress) {
        if (v == matrixViewIdx_ || v == loadViewIdx_) { setView(trackViewIdx_); }
    }
    base_type::eventUp(longpress);
}


void TrackEditor::onEncoderSwitch(unsigned enc, bool v) {
    if (v) return;

    if (getViewIdx() == trackViewIdx_) {
        auto modIdx = enc;
        moduleView_->moduleIdx(trackIdx_, modIdx + 1);
        setView(moduleViewIdx_);
        return;
    }

    base_type::onEncoderSwitch(enc, v);
}

void TrackEditor::eventButton(unsigned btn, bool longPress) {
    auto v = getViewIdx();
    if (v == trackViewIdx_) {
        if (btn < 4) {
            auto modIdx = btn;
            loadModuleView_->moduleIdx(trackIdx_, modIdx + 1);
            matrixView_->moduleIdx(trackIdx_, modIdx + 1);
            moduleView_->moduleIdx(trackIdx_, modIdx + 1);
            setView(loadViewIdx_);
            return;
        } else if (btn >= 4) {
            auto modIdx = btn - 4;
            loadModuleView_->moduleIdx(trackIdx_, modIdx + 1);
            matrixView_->moduleIdx(trackIdx_, modIdx + 1);
            moduleView_->moduleIdx(trackIdx_, modIdx + 1);
            setView(matrixViewIdx_);
            return;
        }
    }

    if (v == loadViewIdx_ && longPress == false) {
        // return view on load and clear
        switch (btn) {
            case 3: {  // clear
                // carry out action
                loadModuleView_->eventButton(btn, longPress);
                // return to overview
                setView(moduleViewIdx_);
                return;
            }
            case 7: {  // clear
                // carry out action
                loadModuleView_->eventButton(btn, longPress);
                // return to overview
                trackView_->trackIdx(trackIdx_);
                setView(trackViewIdx_);
                return;
            }
        }
    }

    base_type::eventButton(btn, longPress);
}

void TrackEditor::trackIdx(unsigned t) {
    trackIdx_ = t;
    trackView_->trackIdx(trackIdx_);
}
