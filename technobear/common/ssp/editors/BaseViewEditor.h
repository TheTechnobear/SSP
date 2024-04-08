#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <vector>

#include "BaseEditor.h"

namespace ssp {

class BaseProcessor;

class BaseViewEditor : public BaseEditor {
public:
    explicit BaseViewEditor(BaseProcessor *p);

    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void onButton(unsigned id, bool v) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;


    void eventButton(unsigned btn, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;
    void eventLeft(bool longPress) override;
    void eventRight(bool longPress) override;
    void eventLeftShift(bool longPress) override;
    void eventRightShift(bool longPress) override;
    void eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) override;


    void editorShown() override;
    void editorHidden() override;

protected:
    using view_type = BaseEditor;

    void drawView(juce::Graphics &g) override;
    void resized() override;
    unsigned addView(std::shared_ptr<view_type>);

    virtual void setView(unsigned newView);
    void nextView();
    void previousView();

    std::vector<std::shared_ptr<view_type>> views_;

    int view_ = -1;

private:
    using base_type = BaseEditor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseViewEditor)
};

}  // namespace ssp
