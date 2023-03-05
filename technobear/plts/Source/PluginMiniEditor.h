#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class PluginMiniEditor : public ssp::BaseMiniView {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;

protected:
//    using base_type = ssp::BarParamEditor;

private:

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};
