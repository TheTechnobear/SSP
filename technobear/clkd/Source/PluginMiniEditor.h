#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class PluginMiniEditor : public ssp::BaseMiniView {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    using base_type = ssp::BaseMiniView;

private:

    PluginProcessor &processor_;
    ssp::ValueButton runButton_, resetButton_, useTrigsButton_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};
