#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BarParamEditor.h"


class PluginEditor : public ssp::BarParamEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::BarParamEditor;
private:

    PluginProcessor &processor_;
    static constexpr unsigned MAX_SIG = PluginProcessor::MAX_SIG_IN;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



