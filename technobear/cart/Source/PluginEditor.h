#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/LineParamEditor.h"

class PluginEditor : public ssp::LineParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::LineParamEditor;

    void drawView(Graphics &) override;
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


    static constexpr unsigned MAX_LAYERS=3;
    juce::Colour clrs_[MAX_LAYERS];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



