#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"

class PerformanceView;
class PerformanceEdit;
class PerformanceAdd;


class PerformanceEditor : public ssp::MultiView<ssp::BaseView> {
public:
    PerformanceEditor(PluginProcessor &p);
    ~PerformanceEditor() override;

protected:
    void resized() override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;
    void eventButton(unsigned btn, bool longPress) override;
    void eventButtonHeld(unsigned btn) override;

    void editorShown() override;

    void onEncoderSwitch(unsigned id, bool v) override;
private: 
    using base_type = ssp::MultiView<ssp::BaseView>;

    PluginProcessor &processor_;
    std::shared_ptr<PerformanceView> perfView_;
    std::shared_ptr<PerformanceEdit> editView_;
    std::shared_ptr<PerformanceAdd> addView_;

    unsigned perfViewIdx_ =0, editViewIdx_=0, addViewIdx_=0;
};
