#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/editors/LineParamEditor.h"
#include "ssp/controls/LineScope.h"
#include "ssp/controls/XYScope.h"

class PluginEditor : public ssp::LineParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::LineParamEditor;

    void drawView(Graphics &) override;
    void timerCallback() override;
    void resized() override;

    ssp::BaseEditor::ControlPage addParamPage(
        std::shared_ptr<ssp::BaseParamControl> c1,
        std::shared_ptr<ssp::BaseParamControl> c2,
        std::shared_ptr<ssp::BaseParamControl> c3,
        std::shared_ptr<ssp::BaseParamControl> c4,
        juce::Colour clr
    );
    void onSSPTimer() override {
        base_type::onSSPTimer();
    }


private:

    void drawValueDisplay(Graphics &);

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



