#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

#include "ssp/controls/ListControl.h"

class PerformanceEdit : public ssp::MiniBasicView {
public:
    PerformanceEdit(PluginProcessor &p);
    ~PerformanceEdit() override;

protected:
    using base_type = ssp::MiniBasicView;
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void deleteParam();

    void resized() override;
    void editorShown() override;

    void refreshParamList();
private: 
    PluginProcessor &processor_;
    ssp::ListControl<> paramList_;
};

