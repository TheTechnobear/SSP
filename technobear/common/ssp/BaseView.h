#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

namespace ssp {

class BaseProcessor;
class EditorHost;


#include "SSPActions.h"

class BaseView :
    public Component,
    public Timer,
    public SSPActions {

public:
    explicit BaseView(BaseProcessor *p, bool compactUI);
    virtual ~BaseView();

    void timerCallback() override;

    void resized() override;

    virtual void drawView(Graphics &g);

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



    // ssp actions
    friend class EditorHost;

    void onSSPTimer() override;

protected:
    BaseProcessor *baseProcessor_;

    Colour defaultBg_=  Colour(0xff111111);
    bool compactUI_=false;

private:
    void paint(Graphics &) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseView)
};

}




