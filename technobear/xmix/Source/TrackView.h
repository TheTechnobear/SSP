#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"

class ModuleView;
class LoadView;
class TrackOverviewView;


// this is a 'controller view'
// TrackOverview displays the overview of track, this provides switching to other tracks
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView

class TrackView : public ssp::MultiView {
public:
    TrackView(PluginProcessor &p);
    ~TrackView() override;

    void drawView(Graphics &g) override;
    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;

    void editorShown() override;

    void eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) override;

private:
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    unsigned trackIdx_;
    PluginProcessor &processor_;

    std::shared_ptr<TrackOverviewView> trackOverviewView_;
    std::shared_ptr<ModuleView> moduleView_;
    std::shared_ptr<LoadView> loadView_;

    unsigned loadViewIdx_, moduleViewIdx_, trackOverviewViewIdx_;
};
