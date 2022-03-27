#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;


#include "SSPActions.h"

#include "ValueButton.h"
#include "BaseEditor.h"
#include "SystemEditor.h"
#include "BaseProcessor.h"
#include "SSPUI.h"

namespace ssp {

class EditorHost :
    public AudioProcessorEditor,
    public SSPActions {
public:
    explicit EditorHost(BaseProcessor *p, BaseEditor *e);
    ~EditorHost();

    void paint(Graphics &g) override {
        drawBasicPanel(g);
    }

    void resized() override {
        system_->resized();
        editor_->resized();
        if(sspui_) sspui_->resized();
    }

    // SSPActions
    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;
    void onSSPTimer() override;

private:
    void drawMenuBox(Graphics &g);
    void drawButtonBox(Graphics &g);
    void drawBasicPanel(Graphics &g);


    void setMenuBounds(ValueButton &btn, unsigned r);

    void sysEditor();
    ValueButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;

    bool LSActive_ = false;
    bool RSActive_ = false;
    bool sysActive_ = false;

    BaseProcessor *processor_;
    BaseEditor *editor_;
    SystemEditor *system_;

    SSPUI *sspui_= nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorHost)
};


}




