#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Module.h"
#include "PluginProcessor.h"
#include "Track.h"
#include "ssp/BaseView.h"
#include "ssp/controls/ListControl.h"
#include "ssp/controls/ValueButton.h"


using namespace juce;

class LoadModuleView : public ssp::BaseView {
public:
    explicit LoadModuleView(PluginProcessor &p, bool compactUI);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    void eventButton(unsigned btn, bool v) override;

    void drawView(Graphics &g) override;
    void editorShown() override;
    void resized() override;

    void setButtonBounds(ssp::ValueButton &btn, unsigned r, unsigned c);
    void drawButtonBox(Graphics &g);

    void moduleIdx(unsigned t, unsigned m);
    unsigned trackIdx() { return trackIdx_; }
    unsigned moduleIdx() { return moduleIdx_; }

    bool moduleUpdated() { return moduleUpdated_; }

private:
    void updateModuleList();

    int curCatIdx_ = 0;
    std::vector<std::string> categories_;
    std::vector<int> modulesId_;


    using base_type = ssp::BaseView;
    static constexpr unsigned btnTopY = 380 - 1;
    static constexpr unsigned btnSpaceY = 50;

    ssp::ValueButton scanBtn_, loadBtn_, clearBtn_;

    void loadModule();

    int trackIdx_ = PluginProcessor::MAX_TRACKS;
    int moduleIdx_ = Track::M_MAX;

    bool moduleUpdated_ = false;
    PluginProcessor &processor_;
    std::string pluginDescripton_;

    ssp::ListControl<> categoryList_;
    ssp::ListControl<> moduleList_;
};
