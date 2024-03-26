#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"

class ModuleView;
class LoadView;
class TrackView;
class MatrixView;


// this is a 'controller view'
// TrackOverview displays the overview of track, this provides switching to other tracks
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView

class TrackEditor : public ssp::MultiView<ssp::BaseView> {
public:
    TrackEditor(PluginProcessor &p);
    ~TrackEditor() override;

    void editorShown() override;

    void trackIdx(unsigned t);
    void eventButton(unsigned btn, bool longPress) override;


private:
    using base_type = ssp::MultiView<ssp::BaseView>;

    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    unsigned trackIdx_;
    PluginProcessor &processor_;

    std::shared_ptr<TrackView> trackView_;
    std::shared_ptr<ModuleView> moduleView_;
    std::shared_ptr<LoadView> loadView_;
    std::shared_ptr<MatrixView> matrixView_;

    unsigned loadViewIdx_, moduleViewIdx_, trackViewIdx_, matrixViewIdx_;
};
