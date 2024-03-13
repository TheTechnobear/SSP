#pragma once



#include "ssp/BaseView.h"
#include "PluginProcessor.h"
#include "ssp/controls/ValueButton.h"
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

class LoadView : public ssp::BaseView {
public:
    LoadView(PluginProcessor &p);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    void eventButton(unsigned btn, bool v) override;
    void eventUp(bool v) override;
    void eventDown(bool v) override;


    void drawView(Graphics &g) override;

    void editorShown() override;

    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
        const int w = 100;
        const int h = btnSpaceY;
        unsigned x = 900 + (c * w);
        unsigned y = btnTopY + (r * h);
        btn.setBounds(x, y, w, h);
    }

    void drawButtonBox(Graphics &g) {
        unsigned butTopY = btnTopY;
        unsigned butLeftX = 900 - 1;
        float x = butLeftX;
        float y = butTopY;
        g.setColour(Colours::grey);
        g.drawHorizontalLine(y, x, 1600 - 1);
        g.drawHorizontalLine(y + btnSpaceY, x, 1600 - 1);
        g.drawHorizontalLine(480 - 1, x, 1600 - 1);
        for (int i = 0; i < 8; i++) { g.drawVerticalLine(x + i * 100, butTopY, 480 - 1); }
    }

    void moduleIdx(int midx);
    int moduleIdx() { return moduleIdx_;}

private:
    static constexpr unsigned btnTopY = 380 - 1;
    static constexpr unsigned btnSpaceY = 50;

    ValueButton scanBtn_, loadBtn_, clearBtn_, upBtn_, downBtn_, leftBtn_, rightBtn_;

    void loadModule();

    int moduleIdx_ = PluginProcessor::M_MAX;
    int curModNameIdx_ = -1;
    PluginProcessor &processor_;
};
