#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class PluginMiniEditor : public ssp::MiniParamView {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    using base_type = ssp::MiniParamView;

    void onButton(unsigned int id, bool v) override;
    void eventButton(unsigned int id, bool longPress) override;
    void resized() override;

private:

    PluginProcessor &processor_;
    ssp::ValueButton resetButton_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};
