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

    void onRightShiftButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
private:
    static const int MAX_LAYERS = PluginProcessor::MAX_LAYERS;
    juce::Colour clrs_[MAX_LAYERS];
    static constexpr unsigned MAX_DATA = PluginProcessor::MAX_BUF_SIZE;
    static constexpr unsigned MAX_VIEWPOINTS = 600;

    struct {
        float dataBuf_[MAX_DATA];
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

