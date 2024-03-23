#pragma once

#include "ssp/BaseView.h"
#include "PluginProcessor.h"


// provides a look at the track
// note: TrackView is its parent, which provides switching to other views
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView
class TrackOverviewView : public ssp::BaseView {
public:
    TrackOverviewView(PluginProcessor &p);
    ~TrackOverviewView() override;

    void drawView(Graphics &g) override;

private:
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    PluginProcessor &processor_;

};

