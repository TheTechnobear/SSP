#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"
#include "ssp/controls/LineScope.h"
#include "ssp/controls/XYScope.h"

using namespace juce;


class PluginMiniEditor : public ssp::LineMiniEditor {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    using base_type = ssp::LineMiniEditor;

    void drawView(Graphics &) override;
    void onSSPTimer() override;
    void resized() override;

    void addParamPage(
        std::shared_ptr<ssp::BaseParamControl> c1,
        std::shared_ptr<ssp::BaseParamControl> c2,
        std::shared_ptr<ssp::BaseParamControl> c3,
        std::shared_ptr<ssp::BaseParamControl> c4,
        juce::Colour clr
    );


private:
    PluginProcessor &processor_;

    static constexpr unsigned MAX_SIG = PluginProcessor::MAX_SIG_IN;
    ssp::LineScope<MAX_SIG> mainScope_;
    ssp::LineScope<2> miniScope_[2];
    ssp::XYScope xyScope_[2];

    juce::Colour clrs_[MAX_SIG];
    static constexpr unsigned MAX_DATA = PluginProcessor::MAX_MSGS;
    static constexpr unsigned MAX_DISP = PluginProcessor::MAX_ENTRY;
    float dataBuf_[MAX_SIG + 1 ][MAX_DATA]; // trig in sig+1
    unsigned wrPos_ = 0;
    unsigned syncPos_ = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};



