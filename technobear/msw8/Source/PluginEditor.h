#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BarParamEditor.h"

class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void paint(Graphics &) override;
protected:
    void drawGrid(Graphics &g);
    using base_type = ssp::BarParamEditor;
private:
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



