#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BarParamEditor.h"
//#include "ssp/LineScope.h"
#include "ssp/WaveDisp.h"


class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::BarParamEditor;

private:
    static const int MAX_LAYERS = PluginProcessor::MAX_LAYERS;
    juce::Colour clrs_[MAX_LAYERS];
    static constexpr unsigned MAX_DATA = PluginProcessor::MAX_BUF_SIZE;
    static constexpr unsigned MAX_VIEWPOINTS = 600;
    float dataBuf_[MAX_LAYERS][MAX_DATA];
    ssp::WaveDisp<1> scopes_[MAX_LAYERS] = {false, false, false, false};

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

