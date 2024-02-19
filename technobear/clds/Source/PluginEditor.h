#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/BarParamEditor.h"
#include "ssp/controls/VuMeter.h"

using namespace juce;


class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::BarParamEditor;

    void onEncoder(unsigned enc, float v) override;
    void onButton(unsigned btn, bool v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

private:
    ssp::StereoVuMeter inVu_;
    ssp::StereoVuMeter outVu_;

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

