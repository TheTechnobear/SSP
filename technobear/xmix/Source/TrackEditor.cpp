#include "TrackEditor.h"

#include "LoadModuleView.h"
#include "ModuleView.h"
#include "TrackView.h"
#include "MatrixView.h"
#include "AddRouteView.h"


TrackEditor::TrackEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {

    trackView_ = std::make_shared<TrackView>(processor_);
    trackView_->setBounds(0, 0, pluginWidth, pluginHeight);
    trackView_->resized();
    trackViewIdx_ = addView(trackView_);

    moduleView_ = std::make_shared<ModuleView>(processor_);
    moduleView_->setBounds(0, 0, pluginWidth, pluginHeight);
    moduleView_->resized();
    moduleViewIdx_ = addView(moduleView_);

    loadModuleView_ = std::make_shared<LoadModuleView>(processor_,true);
    loadModuleView_->setBounds(0, 0, pluginWidth, pluginHeight);
    loadModuleView_->resized();
    loadViewIdx_ = addView(loadModuleView_);


    matrixView_ = std::make_shared<MatrixView>(processor_);
    matrixView_->setBounds(0, 0, pluginWidth, pluginHeight);
    matrixView_->resized();
    matrixViewIdx_ = addView(matrixView_);

    addRouteView_ = std::make_shared<AddRouteView>(processor_);
    addRouteView_->setBounds(0, 0, pluginWidth, pluginHeight);
    addRouteView_->resized();
    addRouteViewIdx_ = addView(addRouteView_);
}

TrackEditor::~TrackEditor() {
}

void TrackEditor::editorShown() {
    trackView_->trackIdx(trackIdx_);
    matrixView_->trackIdx(trackIdx_);
    addRouteView_->trackIdx(trackIdx_);
    setView(trackViewIdx_);
}


void TrackEditor::eventButton(unsigned btn,bool longPress) {
    auto v = getViewIdx();
    if(v == trackViewIdx_) {
        if(btn < 4) {
            auto modIdx = btn;
            moduleView_->moduleIdx(trackIdx_, modIdx + 1);
            loadModuleView_->moduleIdx(trackIdx_, modIdx + 1);
            if(longPress==false) {
                moduleView_->moduleIdx(trackIdx_, modIdx + 1);
                setView(moduleViewIdx_);
            } else {
                setView(loadViewIdx_);
            }
            return;
        } else if(btn==4) {
            setView(matrixViewIdx_);
        } else if(btn==5) {
            setView(addRouteViewIdx_);
        }
    } 

    if(v==loadViewIdx_ && longPress==false) {
        // return view on load and clear
        switch(btn) {
            case 3 :  { // clear 
                // carry out action 
                loadModuleView_->eventButton(btn, longPress);
                // return to overview 
                setView(moduleViewIdx_);
                return;
            }
            case 7 :  { // clear 
                // carry out action 
                loadModuleView_->eventButton(btn, longPress);
                // return to overview 
                trackView_->trackIdx(trackIdx_);
                setView(trackViewIdx_);
                return;
            }
        }
    }

    base_type::eventButton(btn,longPress);
}

    void TrackEditor::trackIdx(unsigned t) {
        trackIdx_ = t;
        trackView_->trackIdx(trackIdx_);
        matrixView_->trackIdx(trackIdx_);
        addRouteView_->trackIdx(trackIdx_);
    }
