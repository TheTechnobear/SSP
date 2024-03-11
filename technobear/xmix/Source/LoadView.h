#pragma once



#include "ssp/BaseView.h"
#include "PluginProcessor.h"
#include "ssp/controls/ValueButton.h"
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

class LoadView : public ssp::BaseView {
public:
    LoadView(PluginProcessor &p)
        : ssp::BaseView(&p, false),
          processor_(p),
          scanBtn_("Scan", nullptr, 32, Colours::white, Colours::black),
          loadBtn_("Load", nullptr, 32, Colours::white, Colours::black),
          clearBtn_("Clear", nullptr, 32, Colours::white, Colours::black),
          upBtn_("EN-", nullptr, 32, Colours::black, Colours::red),
          downBtn_("EN+", nullptr, 32, Colours::black, Colours::red),
          leftBtn_("", nullptr, 32, Colours::black, Colours::red),
          rightBtn_("", nullptr, 32, Colours::black, Colours::red) {
        setButtonBounds(scanBtn_, 0, 0);
        setButtonBounds(clearBtn_, 1, 3);
        setButtonBounds(upBtn_, 0, 5);
        setButtonBounds(loadBtn_, 0, 3);
        setButtonBounds(leftBtn_, 1, 4);
        setButtonBounds(downBtn_, 1, 5);
        setButtonBounds(rightBtn_, 1, 6);

        addAndMakeVisible(scanBtn_);
        addAndMakeVisible(clearBtn_);
        addAndMakeVisible(loadBtn_);
        addAndMakeVisible(upBtn_);
        addAndMakeVisible(downBtn_);
        addAndMakeVisible(leftBtn_);
        addAndMakeVisible(rightBtn_);
    }

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    //    void onLeftButton(bool v) override;
    //    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    // void onLeftShiftButton(bool v) override;
    // void onRightShiftButton(bool v) override;


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

private:
    static constexpr unsigned btnTopY = 380 - 1;
    static constexpr unsigned btnSpaceY = 50;

    ValueButton scanBtn_, loadBtn_, clearBtn_, upBtn_, downBtn_, leftBtn_, rightBtn_;

    void loadModule();

    int curModIdx_ = -1;
    PluginProcessor &processor_;
};
