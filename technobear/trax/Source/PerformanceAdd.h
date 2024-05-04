#pragma once

#include "PluginProcessor.h"
#include "ssp/controls/ListControl.h"
#include "ssp/editors/BaseMiniView.h"

class PerformanceAdd : public ssp::MiniBasicView {
public:
    PerformanceAdd(PluginProcessor &p);
    ~PerformanceAdd() override;

protected:
    using base_type = ssp::MiniBasicView;
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;

    void resized() override;
    void editorShown() override;
    void drawView(juce::Graphics &g) override;

private:
    static constexpr int fh = 12 * COMPACT_UI_SCALE;
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
