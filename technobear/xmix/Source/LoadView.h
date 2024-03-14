#pragma once



#include "ssp/BaseView.h"
#include "PluginProcessor.h"
#include "ssp/controls/ValueButton.h"
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

class LoadView : public ssp::BaseView {
public:
    LoadView(PluginProcessor &p, bool compactUI);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    void eventButton(unsigned btn, bool v) override;
    void eventUp(bool v) override;
    void eventDown(bool v) override;


    void drawView(Graphics &g) override;
    void editorShown() override;
    void resized() override;

    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);
    void drawButtonBox(Graphics &g);

    void moduleIdx(int midx);
    int moduleIdx() { return moduleIdx_;}

    bool moduleUpdated() { return moduleUpdated_;}
private:
    static constexpr unsigned btnTopY = 380 - 1;
    static constexpr unsigned btnSpaceY = 50;

    ValueButton scanBtn_, loadBtn_, clearBtn_, upBtn_, downBtn_, leftBtn_, rightBtn_;

    void loadModule();

    int moduleIdx_ = PluginProcessor::M_MAX;
    int curModNameIdx_ = -1;
    bool moduleUpdated_ = false;
    PluginProcessor &processor_;
};
