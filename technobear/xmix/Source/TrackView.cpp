#include "TrackView.h"

#include "LoadView.h"
#include "ModuleView.h"
#include "TrackOverviewView.h"


TrackView::TrackView(PluginProcessor &p) : base_type(&p, false), processor_(p) {

    trackOverviewView_ = std::make_shared<TrackOverviewView>(processor_);
    trackOverviewView_->setBounds(0, 0, pluginWidth, pluginHeight);
    trackOverviewView_->resized();
    trackOverviewViewIdx_ = addView(trackOverviewView_);

    moduleView_ = std::make_shared<ModuleView>(processor_);
    moduleView_->setBounds(0, 0, pluginWidth, pluginHeight);
    moduleView_->resized();
    moduleViewIdx_ = addView(moduleView_);

    loadView_ = std::make_shared<LoadView>(processor_,true);
    loadView_->setBounds(0, 0, pluginWidth, pluginHeight);
    loadView_->resized();
    loadViewIdx_ = addView(loadView_);
}

TrackView::~TrackView() {
}

void TrackView::editorShown() {
    trackOverviewView_->trackIdx(trackIdx_);
    setView(trackOverviewViewIdx_);
}


void TrackView::eventButton(unsigned btn,bool longPress) {
    auto v = getViewIdx();
    if(v == trackOverviewViewIdx_) {
        if(btn < Track::M_MAX) {
            auto modIdx = btn;
            moduleView_->moduleIdx(trackIdx_, modIdx);
            loadView_->moduleIdx(trackIdx_, modIdx);
            if(longPress==false) {
                moduleView_->moduleIdx(trackIdx_, modIdx);
                setView(moduleViewIdx_);
            } else {
                setView(loadViewIdx_);
            }
            return;
        }
    }
    if(v==loadViewIdx_ && longPress==false) {
        // return view on load and clear
        switch(btn) {
            case 3 :  { // clear 
                // carry out action 
                loadView_->eventButton(btn, longPress);
                // return to overview 
                setView(moduleViewIdx_);
                return;
            }
            case 7 :  { // clear 
                // carry out action 
                loadView_->eventButton(btn, longPress);
                // return to overview 
                trackOverviewView_->trackIdx(trackIdx_);
                setView(trackOverviewViewIdx_);
                return;
            }
        }
    }

    base_type::eventButton(btn,longPress);
}

    void TrackView::trackIdx(unsigned t) {
        trackIdx_ = t;
        trackOverviewView_->trackIdx(trackIdx_);
    }
