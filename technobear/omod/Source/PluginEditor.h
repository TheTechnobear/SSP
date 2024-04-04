#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiBarEditor.h"

using namespace juce;

class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;
protected:
    using base_type = ssp::MultiBarEditor;


protected:
    void onButton(unsigned btn, bool v) override;

    enum ButtonNum {
        BN_RESET = 0,
    };

private:
    ssp::ValueButton resetButton_;
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



