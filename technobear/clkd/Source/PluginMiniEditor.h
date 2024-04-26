#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class PluginMiniEditor : public ssp::MiniParamView {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    using base_type = ssp::MiniParamView;

    void onButton(unsigned int id, bool v) override;
    void eventButton(unsigned int id, bool longPress) override;
    void resized() override;

private:
    enum ButtonNum {
        BN_RUN = 0,
        BN_RESET = 1,
        BN_USETRIGS = 4
    };


    PluginProcessor &processor_;
    ssp::ValueButton runButton_, resetButton_, useTrigsButton_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};
