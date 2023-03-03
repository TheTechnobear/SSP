#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/editors/MultiBarEditor.h"
#include "ssp/controls/VuMeter.h"

class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;
    void drawView(Graphics &) override;
    void resized() override;

protected:
    using base_type = ssp::MultiBarEditor;
private:
    ssp::StereoVuMeter inVu_;
    ssp::StereoVuMeter outVu_;

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



