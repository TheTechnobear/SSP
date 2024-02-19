#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BarParamEditor.h"


using namespace juce;

class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    void onButton(unsigned btn, bool v) override;

    using base_type = ssp::BarParamEditor;

    enum ButtonNum {
        BN_RUN = 0,
        BN_RESET = 1,
        BN_USETRIGS = 4
    };
    void drawView(Graphics &) override;

private:
    ValueButton runButton_, resetButton_, useTrigsButton_;
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



