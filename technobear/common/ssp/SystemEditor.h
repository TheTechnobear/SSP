#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

using namespace juce;

#include "SSPActions.h"
#include "controls/ValueButton.h"
#include "controls/ValueControl.h"

#include "controls/ButtonBox.h"

#include "SSP.h"

namespace ssp {

class BaseProcessor;

class SystemEditor : public juce::Component,
                     // public juce::Timer,
                     public SSPActions {
public:
    explicit SystemEditor(BaseProcessor *p);
    virtual ~SystemEditor();

    void midiLearn(bool b);
    void deleteAutomation(bool b);
    void noteInput(bool b);


    // ssp actions
    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;

    void onLeftButton(bool v) override {}
    void onRightButton(bool v) override {}
    void onUpButton(bool v) override {}
    void onDownButton(bool v) override {}
    void onLeftShiftButton(bool v) override {}
    void onRightShiftButton(bool v) override {}


    void onSSPTimer() override {}
    void eventButton(unsigned btn, bool longPress) override {}
    void eventUp(bool longPress) override {}
    void eventDown(bool longPress) override {}
    void eventLeft(bool longPress) override {}
    void eventRight(bool longPress) override {}
    void eventLeftShift(bool longPress) override {}
    void eventRightShift(bool longPress) override {}
    void eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) override {}
    void eventButtonHeld(unsigned btn) override {}

protected:
    enum UI_Mode{
        M_PARAM, 
        M_DEVICE
    } mode_;

    virtual void mode(UI_Mode m);
    UI_Mode mode() { return mode_;}

    BaseProcessor *baseProcessor_;

    ListValueControl midiInCtrl_, midiOutCtrl_, midiChannelCtrl_;

    void midiInCallback(float idx, const std::string &dev);
    void midiOutCallback(float idx, const std::string &dev);
    void midiChannelCallback(float idx, const std::string &ch);


    static constexpr unsigned MAX_SHOWN = 12;
    int idxOffset_ = 0;
    int selIdx_ = -1;
    ValueButton learnBtn_, delBtn_, noteInputBtn_;
    ValueButton deviceMode_, paramMode_;

private:
    std::vector<std::string> midiInStr_;
    std::vector<std::string> midiOutStr_;
    std::vector<std::string> midiChStr_;
    std::vector<MidiDeviceInfo> inDevices_;
    std::vector<MidiDeviceInfo> outDevices_;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SystemEditor)
};


class SystemFullEditor : public SystemEditor {
public:
    explicit SystemFullEditor(BaseProcessor *p);
    virtual ~SystemFullEditor();

    void resized() override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;

protected:
    void paint(Graphics &g) override;
    virtual void drawView(Graphics &g);
    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);

    void drawButtonBox(Graphics &g); 
private:
    ValueButton leftBtn_, rightBtn_, upBtn_, downBtn_;
    ValueButton leftShiftBtn_, rightShiftBtn_;
    void drawLabel(Graphics &, const std::string &str, unsigned idx);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SystemFullEditor)
};

class SystemMiniEditor : public SystemEditor {
public:
    explicit SystemMiniEditor(BaseProcessor *p);
    virtual ~SystemMiniEditor();

    void resized() override;

protected:
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned buttonBarH = 32 * COMPACT_UI_SCALE;
    static constexpr unsigned titleH = 15 * COMPACT_UI_SCALE;
    static constexpr unsigned titleFH = 10 * COMPACT_UI_SCALE;
    static constexpr unsigned canvasW = SSP_COMPACT_WIDTH - 2 * gap;
    static constexpr unsigned canvasH = SSP_COMPACT_HEIGHT - buttonBarH - titleH;

    static constexpr unsigned nParamsPerPage = 4;
    static constexpr unsigned gridW = canvasW / nParamsPerPage;
    static constexpr unsigned paramWidth = gridW - gap;
    static constexpr unsigned paramHeight = 32 * COMPACT_UI_SCALE;

    void mode(UI_Mode m) override;
    void paint(Graphics &g) override;
    virtual void drawView(Graphics &g);
    void drawButtonBox(Graphics &g); 
    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);

private:
    void drawLabel(Graphics &, const std::string &str, unsigned idx);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SystemMiniEditor)
};



}  // namespace ssp
