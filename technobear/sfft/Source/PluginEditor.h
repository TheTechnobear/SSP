#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiBarEditor.h"

using namespace juce;

class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &, unsigned maxviews);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::MultiBarEditor;

private:
    juce::Colour clrs_[4];
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

