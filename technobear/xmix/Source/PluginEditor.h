#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"
#include <memory>


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

    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    static constexpr int LOAD_VIEW = PluginProcessor::M_MAX; // not M_MAX is view +1!
    std::shared_ptr<LoadView> loadView_=nullptr;

    PluginProcessor &processor_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
