#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/LineParamEditor.h"

class PluginEditor : public ssp::LineParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
protected:
    void drawGrid(Graphics &g);
    using base_type = ssp::LineParamEditor;
private:
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



