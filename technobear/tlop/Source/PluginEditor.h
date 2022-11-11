#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BarParamEditor.h"
#include "ssp/LineScope.h"


class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::BarParamEditor;

private:
    static const int MAX_LAYER=4;
    juce::Colour clrs_[MAX_LAYER];
    static constexpr unsigned MAX_DATA = 4000;
    float dataBuf_[MAX_LAYER][MAX_DATA];
    ssp::LineScope<1> scopes_[MAX_LAYER] = { false, false, false, false};

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

