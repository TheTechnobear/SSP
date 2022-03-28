#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/MultiBarEditor.h"

class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::MultiBarEditor;

    void drawView(Graphics &) override;
    void resized() override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
private:

    PluginProcessor &processor_;

    static constexpr unsigned L_CLRS = 3;

    juce::Colour clrs_[L_CLRS];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



