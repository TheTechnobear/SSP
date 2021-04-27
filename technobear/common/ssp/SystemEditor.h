#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "SSPButton.h"

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


protected:
//    using base_type = juce::AudioProcessorEditor;

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

    void setButtonBounds(SSPButton &btn, unsigned r, unsigned c);

    BaseProcessor *baseProcessor_;

    SSPButton leftBtn_, rightBtn_, upBtn_, downBtn_;
    SSPButton leftShiftBtn_, rightShiftBtn_;
private:
    void paint(Graphics &) override;

    std::vector<MidiDeviceInfo> inDevices_;
    std::vector<MidiDeviceInfo> outDevices_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SystemEditor)
};

}




