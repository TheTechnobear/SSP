#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include "BaseProcessor.h"
#include "BaseView.h"
#include "SSPActions.h"
#include "SSPUI.h"
#include "SystemEditor.h"
#include "controls/ValueButton.h"

namespace ssp {


class EditorHost : public juce::AudioProcessorEditor, public SSPActions {
public:
    explicit EditorHost(BaseProcessor *p, BaseView *e, bool compactUI = false, bool enableSysEditor = true,
                        bool drawDefaults = true);
    ~EditorHost();

    void paint(Graphics &g) override { drawBasicPanel(g); }

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


    void eventButton(unsigned btn,bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;
    void eventLeft(bool longPress) override;
    void eventRight(bool longPress) override;
    void eventLeftShift(bool longPress) override;
    void eventRightShift(bool longPress) override;
    void eventButtonCombo(unsigned btn,unsigned comboBtn, bool longPress) override;


    bool keyStateChanged(bool isKeyDown) override;
    void modifierKeysChanged(const ModifierKeys &modifiers) override;

    juce::Component *getEditorComponent() { 
        if(sysActive_ && system_ != nullptr) {
            return system_;
        } 
        return editor_; 
    }

    bool isCompactUI(){ return compactUI_;}

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

    bool keyStates_[6] = { false, false, false, false, false, false };

    BaseProcessor *processor_;
    BaseView *editor_;
    SystemEditor *system_;
    bool compactUI_ = false;
    bool drawDefaults_ = false;

    Colour defaultBg_ = Colour(0xff111111);

    SSPUI *sspui_ = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorHost)
};


}  // namespace ssp
