#include "TrackView.h"

#include "LoadView.h"
#include "ModuleView.h"
#include "TrackOverviewView.h"


TrackView::TrackView(PluginProcessor &p) : ssp::MultiView(&p, false), processor_(p) {

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

void TrackView::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn, comboBtn, longPress);

    // if(longPress) returm;

    if (comboBtn == SSP_Up) {
        if (btn < Track::M_MAX) {
            int moduleIdx = btn;
            if (moduleIdx == getViewIdx()) {
                if (loadView_) {
                    loadView_->moduleIdx(trackIdx_, moduleIdx);
                    setView(loadViewIdx_);
                }
            } else {
                moduleView_->moduleIdx(trackIdx_, moduleIdx);
                setView(moduleViewIdx_);
            }

        } else if (loadViewIdx_ == getViewIdx()) {
            if (loadView_) {
                int moduleIdx = loadView_->moduleIdx();
                if (moduleIdx < Track::M_MAX) {
                    moduleView_->moduleIdx(trackIdx_, moduleIdx);
                    setView(moduleViewIdx_);
                }
            }
        }
    }
}
