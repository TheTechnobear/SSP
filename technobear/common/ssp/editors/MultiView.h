#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "BaseEditor.h"

#include <vector>

using namespace juce;

namespace ssp {

class BaseProcessor;

class MultiView :
    public BaseView {

public:
    explicit MultiView(BaseProcessor *p,bool compactUI);

    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void onButton(unsigned id, bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;

    void editorShown() override;
    void editorHidden() override;

    int getViewIdx() { return view_;}

protected:
    using base_type = BaseView;
    using base_view = BaseView;

    void drawView(Graphics &g) override;
    void resized() override;
    void addView(std::shared_ptr<base_view>);

    virtual void setView(unsigned newView);
    void nextView();
    void previousView();

    std::vector<std::shared_ptr<base_view>> views_;

    int view_ = -1;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiView)
};

}




