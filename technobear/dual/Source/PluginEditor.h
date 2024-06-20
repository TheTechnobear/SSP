#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"


class DualView;
class LoadModuleView;


using namespace juce;

class PluginEditor : public ssp::MultiView<ssp::BaseView> {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::MultiView<ssp::BaseView>;

    void onSSPTimer() override;
    void editorShown() override;
    void editorHidden() override;

    void eventLeft(bool lp) override;
    void eventRight(bool lp) override;
    void eventLeftShift(bool lp) override;
    void eventRightShift(bool lp) override;
    void eventButton(unsigned btn, bool longPress) override;

private:
    PluginProcessor &processor_;

    std::shared_ptr<DualView> dualView_;
    int dualViewIdx_ = -1;
    std::shared_ptr<LoadModuleView> loadView_;
    int loadViewIdx_ = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
