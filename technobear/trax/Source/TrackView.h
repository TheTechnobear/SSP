#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"


// provides a look at the track
// note: TrackView is its parent, which provides switching to other views
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView

#include "components/ModuleComponent.h"

class TrackView : public ssp::MiniBasicView {
public:
    TrackView(PluginProcessor &p);
    ~TrackView() override;

    void trackIdx(unsigned t);
protected:
    void drawView(juce::Graphics &g) override;
    void resized() override;

private:
    using base_type = ssp::MiniBasicView;
    unsigned trackIdx_;
    PluginProcessor &processor_;
    std::vector<std::shared_ptr<ModuleComponent>> modules_;
};
