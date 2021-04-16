#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/LineParamEditor.h"
#include "ssp/LineScope.h"

class PluginEditor : public ssp::LineParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::LineParamEditor;

    void paint(Graphics &) override;
    void timerCallback() override;
    void resized() override;

    ssp::BaseEditor::ControlPage addParamPage(
        std::shared_ptr<ssp::BaseParamControl> c1,
        std::shared_ptr<ssp::BaseParamControl> c2,
        std::shared_ptr<ssp::BaseParamControl> c3,
        std::shared_ptr<ssp::BaseParamControl> c4,
        juce::Colour clr
    );

private:
    PluginProcessor &processor_;


    static constexpr unsigned MAX_SIG=PluginProcessor::MAX_SIG_IN;
    ssp::LineScope scopes_[MAX_SIG];
    static constexpr unsigned MAX_DATA=256;
    float dataBuf_[MAX_SIG] [MAX_DATA];
    unsigned wrPos_=0;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



