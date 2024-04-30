#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"
#include "ssp/controls/WaveDisp.h"

using namespace juce;


class PluginMiniEditor : public ssp::LineMiniEditor {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    using base_type = ssp::LineMiniEditor;

    void drawView(Graphics &) override;
    void resized() override;
    void onSSPTimer() override;

private:
    static const constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;

    juce::Colour clrs_[MAX_LAYERS];
    static constexpr unsigned DATA_POINTS = 300 * COMPACT_UI_SCALE;

    struct {
        float dataBuf_[DATA_POINTS];
        float beginPos_ = 0.0f;
        float endPos_ = 1.0f;
        float curPos_ = 0.0;
        bool isRec_ = false;
        float recPos_ = 0.0;
    } layer_[MAX_LAYERS];

    ssp::WaveDisp<1> scopes_[MAX_LAYERS] = {
        ssp::WaveDisp<1>(false, true),
        ssp::WaveDisp<1>(false, true),
        ssp::WaveDisp<1>(false, true),
        ssp::WaveDisp<1>(false, true)
    };


    PluginProcessor &processor_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};



