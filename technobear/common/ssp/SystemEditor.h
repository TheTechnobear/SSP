#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "ValueButton.h"
#include "ValueControl.h"

namespace ssp {

class BaseProcessor;

class EditorHost;


#include "SSPActions.h"

class SystemEditor :
    public Component,
    public Timer,
    public SSPActions {

public:
    explicit SystemEditor(BaseProcessor *p);
    virtual ~SystemEditor();

    void timerCallback() override;

    void resized() override;


    void midiLearn(bool b);
    void deleteAutomation(bool b);
    void noteInput(bool b);
protected:
//    using base_type = juce::AudioProcessorEditor;

    void paint(Graphics &g) override;
    virtual void drawView(Graphics &g);

    // ssp actions
    friend class EditorHost;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;

    void onSSPTimer() override {}

    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);

    BaseProcessor *baseProcessor_;

    ValueButton leftBtn_, rightBtn_, upBtn_, downBtn_;
    ValueButton leftShiftBtn_, rightShiftBtn_;

    ValueButton learnBtn_, delBtn_, noteInputBtn_;

    ListValueControl midiInCtrl_, midiOutCtrl_, midiChannelCtrl_;

    void midiInCallback(float idx, const std::string &dev);
    void midiOutCallback(float idx, const std::string &dev);
    void midiChannelCallback(float idx, const std::string &ch);
private:
    void drawLabel(Graphics &, const std::string &str, unsigned idx);

    std::vector<std::string> midiInStr_;
    std::vector<std::string> midiOutStr_;
    std::vector<std::string> midiChStr_;

    std::vector<MidiDeviceInfo> inDevices_;
    std::vector<MidiDeviceInfo> outDevices_;

    static constexpr unsigned MAX_SHOWN = 12;
    int idxOffset_ = 0;
    int selIdx_ = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SystemEditor)
};

}




