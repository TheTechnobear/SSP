#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

using namespace juce;

#include "SSPActions.h"
#include "controls/ValueButton.h"
#include "controls/ValueControl.h"

#include "controls/ButtonBox.h"

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

protected:
    BaseProcessor *baseProcessor_;

    ListValueControl midiInCtrl_, midiOutCtrl_, midiChannelCtrl_;

    void midiInCallback(float idx, const std::string &dev);
    void midiOutCallback(float idx, const std::string &dev);
    void midiChannelCallback(float idx, const std::string &ch);


    static constexpr unsigned MAX_SHOWN = 12;
    int idxOffset_ = 0;
    int selIdx_ = -1;
    ValueButton learnBtn_, delBtn_, noteInputBtn_;

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
    void paint(Graphics &g) override;
    virtual void drawView(Graphics &g);
    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);

private:
    void drawLabel(Graphics &, const std::string &str, unsigned idx);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SystemMiniEditor)
};



}  // namespace ssp
