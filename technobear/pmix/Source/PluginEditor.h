#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/MultiViewEditor.h"
#include "ssp/VuMeter.h"
#include "MonoChannel.h"
#include "StereoChannel.h"

class PluginEditor : public ssp::MultiViewEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void paint(Graphics &) override;
    void resized() override;
    void timerCallback() override;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;
protected:
    using base_type = ssp::MultiViewEditor;

private:

    static constexpr unsigned POLL_TIME = 50; // mSec
    static constexpr unsigned BUT_COUNTER = POLL_TIME * 10; // 0.5 sec
    unsigned viewHeldCount_=0;
    unsigned unheldView_=0;

    MonoChannel inTracks_[PluginProcessor::IN_T_MAX];
    StereoChannel outTracks_[(PluginProcessor::OUT_T_MAX / 2)];

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

