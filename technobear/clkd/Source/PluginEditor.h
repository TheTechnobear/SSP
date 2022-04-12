#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BarParamEditor.h"


class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    void onButton(unsigned btn, bool v) override;

    using base_type = ssp::BarParamEditor;

    enum ButtonNum {
        BN_RUN=0,
        BN_RESET=4
    };
    void drawView(Graphics &) override;

private:
    ValueButton runButton_, resetButton_;
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



