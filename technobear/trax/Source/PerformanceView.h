#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class PerfParamComponent;

class PerformanceView : public ssp::MiniBasicView {
public:
    PerformanceView(PluginProcessor &p);
    ~PerformanceView() override;

protected:
    using base_type = ssp::MiniBasicView;
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void eventButton(unsigned btn, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;

    void editorShown() override;
    void resized() override;
    void drawView(juce::Graphics &g) override;
    
    void onSSPTimer() override;

private:
    void changePage(int delta); 
    std::vector<std::shared_ptr<PerfParamComponent>> params_;
    static constexpr unsigned paramsPerPage = 4;
    unsigned page_ = 0;

    PluginProcessor &processor_;
};
