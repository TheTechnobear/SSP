#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

using namespace juce;

class PluginMiniEditor : public ssp::LineMiniEditor {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

    void drawView(Graphics &) override;
protected:
    void drawGrid(Graphics &g);
    using base_type = ssp::LineMiniEditor;
private:
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};



