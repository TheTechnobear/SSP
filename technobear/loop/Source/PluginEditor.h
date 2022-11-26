#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BaseViewEditor.h"
#include "ssp/WaveDisp.h"


class PluginEditor : public ssp::BaseViewEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::BaseViewEditor;

    void onRightShiftButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
private:
    static const int MAX_LAYERS = PluginProcessor::MAX_LAYERS;
    juce::Colour clrs_[MAX_LAYERS];
    static constexpr unsigned DATA_POINTS = 600;

    struct {
        float dataBuf_[DATA_POINTS];
        float beginPos_ = 0.0f;
        float endPos_ = 1.0f;
        float curPos_ = 0.0;
        bool isRec_ = false;
        float recPos_ = 0.0;
    } layer_[MAX_LAYERS];

    enum E_ViewMode {
        M_LAYER,
        M_REC,
        M_MAX
    };

    unsigned viewMode_ = M_LAYER;

    ssp::WaveDisp<1> scopes_[MAX_LAYERS] = {false, false, false, false};

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

