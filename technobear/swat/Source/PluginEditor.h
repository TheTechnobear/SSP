#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BaseEditor.h"

class PluginEditor : public ssp::BaseEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void paint(Graphics &) override;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    // user buttons
    void onButton(unsigned btn, bool v) override;

    // nav buttons
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
//    void onLeftShiftButton(bool v) override;
//    void onRightShiftButton(bool v) override;

protected:
    using base_type = ssp::BaseEditor;
private:
    unsigned activeEngine_ = 0;
    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};


