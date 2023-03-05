#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;


#include "SSPActions.h"

#include "controls/ValueButton.h"
#include "BaseView.h"
#include "SystemEditor.h"
#include "BaseProcessor.h"
#include "SSPUI.h"

namespace ssp {

class EditorHost :
    public AudioProcessorEditor,
    public SSPActions {
public:
    explicit EditorHost(BaseProcessor *p, BaseView *e,
                        bool compactUI=false,
                        bool enableSysEditor=true,
                        bool drawDefaults=true);
    ~EditorHost();

    void paint(Graphics &g) override {
        drawBasicPanel(g);
    }

    void resized() override;

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


    BaseView* getEditorView() { return editor_;}

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
    BaseView *editor_;
    SystemEditor *system_;
    bool compactUI_=false;
    bool drawDefaults_ =false;

    Colour defaultBg_=  Colour(0xff111111);

    SSPUI *sspui_= nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorHost)
};


}




