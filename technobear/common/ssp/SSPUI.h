#pragma once


#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "SSPActions.h"
#include "BaseProcessor.h"


namespace ssp {


class SSPUI : public Component,Button::Listener {
public:
    SSPUI(BaseProcessor* processor, SSPActions* actions);
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    void buttonClicked(Button *button) override;
    void buttonStateChanged(Button *button) override;
    BaseProcessor* processor_;
    SSPActions* actions_;

    static constexpr unsigned NENC=4;
    TextButton encUp_[NENC];
    TextButton encDown_[NENC];
    ToggleButton encSW_[NENC];

    ToggleButton left_,right_,up_,down_, ls_,rs_;
    static constexpr unsigned NBUTS=8;
    ToggleButton buttons_[NBUTS];

    static constexpr unsigned NIO=24;
    ToggleButton inputs_[NIO];
    ToggleButton outputs_[NIO];
    juce_UseDebuggingNewOperator
};


} // namespace
