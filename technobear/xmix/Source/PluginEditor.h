#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"


class ModuleView;
class LoadView;


using namespace juce;

class PluginEditor : public ssp::MultiView {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    // void drawView(Graphics &) override;
    // void resized() override;

protected:
    using base_type = ssp::MultiView;

    // void onEncoder(unsigned enc, float v) override;
    // void onEncoderSwitch(unsigned enc, bool v) override;
    // void onButton(unsigned btn, bool v) override;
    // void onLeftButton(bool v) override;
    // void onRightButton(bool v) override;
    // void onUpButton(bool v) override;
    // void onDownButton(bool v) override;
    // void onLeftShiftButton(bool v) override;
    // void onRightShiftButton(bool v) override;

    // void onSSPTimer() override;
    // void editorShown() override;
    // void editorHidden() override;

private:
    int MAX_COUNTER = 15;
    int upDownCounter_[2] = { 0, 0 };
    bool upDownState_[2] = { false, false };

    PluginProcessor &processor_;
    // int activeModuleIdx_ = 0;

    // std::shared_ptr<ModuleView> moduleView_;
    // std::shared_ptr<LoadView> loadView_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
