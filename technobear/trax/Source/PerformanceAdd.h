#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"
#include "ssp/controls/ListControl.h"

class PerformanceAdd : public ssp::MiniBasicView {
public:
    PerformanceAdd(PluginProcessor &p);
    ~PerformanceAdd() override;

protected:
    void resized() override;
    using base_type = ssp::MiniBasicView;
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void editorShown() override;
    
private:
    void refreshTrackList();
    void refreshModuleList();
    void refreshParamList();

    int curTrackIdx_ = 0;
    int curModuleIdx_ = 0;

    PluginProcessor &processor_;

    ssp::ListControl<> trackList_;
    ssp::ListControl<> moduleList_;
    ssp::ListControl<> paramList_;
    
};

