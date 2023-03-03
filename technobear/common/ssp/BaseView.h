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
    explicit BaseView(BaseProcessor *p);
    virtual ~BaseView();

    void timerCallback() override;

    void resized() override;


//    using base_type = juce::AudioProcessorEditor;

    virtual void drawView(Graphics &g);

    // new for BaseViewEditor
    virtual void editorShown() {}
    virtual void editorHidden() {}

    // ssp actions
    friend class EditorHost;

    void onSSPTimer() override;

protected:
    BaseProcessor *baseProcessor_;

    Colour defaultBg_=  Colour(0xff111111);

private:
    void paint(Graphics &) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseView)
};

}




