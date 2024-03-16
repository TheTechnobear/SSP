#pragma once

#include "PluginProcessor.h"
#include "ssp/BaseView.h"


// provides a look at the track
// note: TrackView is its parent, which provides switching to other views
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView
class TrackOverviewView : public ssp::BaseView {
public:
    TrackOverviewView(PluginProcessor &p);
    ~TrackOverviewView() override;

    void drawView(Graphics &g) override;

}

