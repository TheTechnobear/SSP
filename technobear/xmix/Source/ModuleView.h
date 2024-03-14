#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "ssp/BaseView.h"

using namespace juce;


class ModuleView : public ssp::BaseView {
public:
    ModuleView(PluginProcessor &p, unsigned modIdx);
    ~ModuleView() override;

    void drawView(Graphics &g) override;
    void drawModulePanel(Graphics &);
    void drawIO(Graphics &);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;

    void editorShown() override;

private:
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    unsigned moduleIdx_;
    PluginProcessor &processor_;

    SSPExtendedApi::PluginEditorInterface* editor_ = nullptr;
    Component *pComponent_ = nullptr;
};
