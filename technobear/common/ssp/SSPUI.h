#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "SSPActions.h"
#include "BaseProcessor.h"

namespace ssp {

class SSPUI : public juce::Component,juce::Button::Listener {
public:
    SSPUI(BaseProcessor* processor, SSPActions* actions);
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    void buttonClicked(juce::Button *button) override;
    void buttonStateChanged(juce::Button *button) override;
    BaseProcessor* processor_;
    SSPActions* actions_;

    static constexpr unsigned NENC=4;
    juce::TextButton encUp_[NENC];
    juce::TextButton encDown_[NENC];
    juce::ToggleButton encSW_[NENC];

    juce::ToggleButton left_,right_,up_,down_, ls_,rs_;
    static constexpr unsigned NBUTS=8;
    juce::ToggleButton buttons_[NBUTS];

    static constexpr unsigned NIO=24;
    juce::ToggleButton inputs_[NIO];
    juce::ToggleButton outputs_[NIO];
    juce_UseDebuggingNewOperator
};


} // namespace
