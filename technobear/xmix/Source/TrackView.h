#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"


// provides a look at the track
// note: TrackView is its parent, which provides switching to other views
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView
class TrackView : public ssp::MiniBasicView {
public:
    TrackView(PluginProcessor &p);
    ~TrackView() override;

    void drawView(Graphics &g) override;

    void trackIdx(unsigned t) { trackIdx_ = t; }

private:
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;
    unsigned trackIdx_;

    PluginProcessor &processor_;
};
