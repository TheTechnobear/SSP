#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

using namespace juce;
class SeqCellComponent;

class PluginMiniEditor : public ssp::LineMiniEditor {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    using base_type = ssp::LineMiniEditor;

    void drawView(Graphics &) override;
    void resized() override;

private:

    static constexpr unsigned L_CLRS = 3;
    static juce::Colour LAYER_COLOURS[L_CLRS];
    PluginProcessor &processor_;

    std::vector<std::shared_ptr<SeqCellComponent>> cells_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};



