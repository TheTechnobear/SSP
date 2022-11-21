#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/MultiBarEditor.h"
#include "ssp/WaveDisp.h"


class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &, unsigned maxviews);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::MultiBarEditor;

private:
    static const int MAX_LAYERS = PluginProcessor::MAX_LAYERS;
    juce::Colour clrs_[MAX_LAYERS];
    static constexpr unsigned MAX_DATA = PluginProcessor::MAX_BUF_SIZE;
    static constexpr unsigned MAX_VIEWPOINTS = 600;

    struct {
        float beginPos = 0.0f;
        float endPos = 1.0f;
        float curPos = 0.0;
        float dataBuf_[MAX_DATA];
    } layer_[MAX_LAYERS];

    ssp::WaveDisp<1> scopes_[MAX_LAYERS] = {false, false, false, false};

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

