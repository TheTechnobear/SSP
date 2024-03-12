#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include "SSPActions.h"

namespace ssp {

class BaseProcessor;
class EditorHost;


class BaseView :
    public juce::Component,
    public SSPActions {

public:
    explicit BaseView(BaseProcessor *p, bool compactUI);
    virtual ~BaseView();

    void resized() override;

    virtual void drawView(juce::Graphics &g);

    virtual void editorShown() {}
    virtual void editorHidden() {}

    // SSPActions
    void onEncoder(unsigned id, float v) override {}
    void onEncoderSwitch(unsigned id, bool v) override {};
    void onButton(unsigned id, bool v) override {};
    void onUpButton(bool v) override {};
    void onDownButton(bool v) override {};
    void onLeftButton(bool v) override {};
    void onRightButton(bool v) override {};
    void onLeftShiftButton(bool v) override {};
    void onRightShiftButton(bool v) override {};

    void onSSPTimer() override;

    void eventButton(unsigned btn,bool longPress) override {}
    void eventUp(bool longPress) override {}
    void eventDown(bool longPress) override {}
    void eventLeft(bool longPress) override {}
    void eventRight(bool longPress) override {}
    void eventLeftShift(bool longPress) override {}
    void eventRightShift(bool longPress) override {}
    void eventButtonCombo(unsigned btn,unsigned comboBtn, bool longPress) override {}


    friend class EditorHost;


protected:
    BaseProcessor *baseProcessor_;

    juce::Colour defaultBg_=  juce::Colour(0xff111111);
    bool compactUI_=false;

private:
    void paint(juce::Graphics &) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseView)
};

}




