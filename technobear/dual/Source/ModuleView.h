#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "ssp/BaseView.h"

using namespace juce;


class ModuleView : public ssp::BaseView {
public:
    ModuleView(PluginProcessor &p);
    ~ModuleView() override;

    void drawView(Graphics &g) override;
    void drawModulePanel(Graphics &);
    void drawIO(Graphics &);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;

    void onSSPTimer() override;
    void editorShown() override;
    void editorHidden() override;

    void moduleIdx(unsigned t, unsigned m) {
        trackIdx_ = t;
        moduleIdx_ = m;
    }

private:
    using base_type = ssp::BaseView;
    
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    int trackIdx_ = PluginProcessor::MAX_TRACKS;
    int moduleIdx_ = Track::M_MAX;
    PluginProcessor &processor_;
};
