#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"

class ModuleView;
class LoadModuleView;
class TrackView;
class MatrixView;
class AddRouteView;


// this is a 'controller view'
// TrackOverview displays the overview of track, this provides switching to other tracks
/// e.g. TrackOverview/LoadView/ModuleView/ModulationView

class TrackEditor : public ssp::MultiView<ssp::BaseView> {
public:
    TrackEditor(PluginProcessor &p);
    ~TrackEditor() override;

    void editorShown() override;

    void trackIdx(unsigned t);

    void onEncoderSwitch(unsigned enc, bool v) override;
    void eventUp(bool longpress) override;
    void eventButton(unsigned btn, bool longPress) override;

    void resized() override;
private:
    using base_type = ssp::MultiView<ssp::BaseView>;

    unsigned trackIdx_;
    PluginProcessor &processor_;

    std::shared_ptr<TrackView> trackView_;
    std::shared_ptr<ModuleView> moduleView_;
    std::shared_ptr<LoadModuleView> loadModuleView_;
    std::shared_ptr<MatrixView> matrixView_;
    std::shared_ptr<AddRouteView> addRouteView_;

    unsigned loadViewIdx_, moduleViewIdx_, trackViewIdx_, matrixViewIdx_, addRouteViewIdx_;
};
