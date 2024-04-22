#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include "BaseProcessor.h"
#include "SSP.h"
#include "SSPActions.h"

namespace ssp {

class EditorHost;

class SSPUI : public juce::Component, public juce::Timer, juce::Button::Listener {
public:
    SSPUI(BaseProcessor* processor, EditorHost* actions);
    ~SSPUI();
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    void buttonClicked(juce::Button* button) override;
    void buttonStateChanged(juce::Button* button) override;
    BaseProcessor* processor_;
    EditorHost* actions_;

    static constexpr unsigned NENC = 4;
    juce::TextButton encUp_[NENC];
    juce::TextButton encDown_[NENC];
    juce::ToggleButton encSW_[NENC];

    juce::ToggleButton left_, right_, up_, down_, ls_, rs_;
    static constexpr unsigned NBUTS = 8;
    juce::ToggleButton buttons_[NBUTS];

    static constexpr unsigned NIO = 24;
    juce::ToggleButton inputs_[NIO];
    juce::ToggleButton outputs_[NIO];


    void buttonPressed(int n, bool val);

    void generateButtenEvents(int n, bool val);
    bool buttonState_[SSP_LastBtn] = { false, false, false, false, false, false, false,
                                       false, false, false, false, false, false, false };
    unsigned buttonCounter_[SSP_LastBtn] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    juce_UseDebuggingNewOperator
};


}  // namespace ssp
