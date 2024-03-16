#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <vector>

#include "BaseEditor.h"

namespace ssp {

class BaseProcessor;

class MultiView : public BaseView {
public:
    explicit MultiView(BaseProcessor *p, bool compactUI);

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

    int getViewIdx() { return view_; }

protected:
    using base_type = BaseView;
    using base_view = BaseView;

    void drawView(juce::Graphics &g) override;
    void resized() override;
    unsigned addView(std::shared_ptr<base_view>);

    virtual void setView(unsigned newView);
    void nextView();
    void previousView();

    std::vector<std::shared_ptr<base_view>> views_;

    int view_ = -1;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiView)
};

}  // namespace ssp
