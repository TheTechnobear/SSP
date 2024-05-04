#pragma once

#include "PluginProcessor.h"
#include "ssp/controls/ListControl.h"
#include "ssp/editors/BaseMiniView.h"

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
    void drawView(juce::Graphics &g) override;

private:
    static constexpr int fh = 12 * COMPACT_UI_SCALE;
    void refreshParamList();
    PluginProcessor &processor_;
    ssp::ListControl<> paramList_;
};
