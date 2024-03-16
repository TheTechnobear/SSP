


TrackView::TrackView(PluginProcessor &p) ssp::BaseView(&p, false) {
    TrackOverviewView = std::make_shared<TrackOverviewView>(processor_, true);
    TrackOverviewView->setBounds(0, 0, pluginWidth, pluginHeight);
    TrackOverviewView->resized();
    addView(TrackOverviewView);

    moduleView_ = std::make_shared<ModuleView>(processor_, true);
    moduleView_->setBounds(0, 0, pluginWidth, pluginHeight);
    moduleView_->resized();
    addView(moduleView_);

    loadView_ = std::make_shared<LoadView>(processor_, true);
    loadView_->setBounds(0, 0, pluginWidth, pluginHeight);
    loadView_->resized();
    addView(loadView_);
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
                    loadView_->moduleIdx(moduleIdx);
                    setView(LOAD_VIEW);
                }
            } else {
                setView(moduleIdx);
            }

        } else if (LOAD_VIEW == getViewIdx()) {
            if (loadView_) {
                int moduleIdx = loadView_->moduleIdx();
                if (moduleIdx < PluginProcessor::M_MAX) { setView(moduleIdx); }
            }
        }
    }
}


