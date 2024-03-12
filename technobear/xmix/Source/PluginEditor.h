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

protected:
    using base_type = ssp::MultiView;

    void eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) override;

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
