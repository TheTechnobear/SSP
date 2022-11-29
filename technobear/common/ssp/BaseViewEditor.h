#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "BaseEditor.h"
//#include "BaseView.h"


#include <vector>

using namespace juce;

namespace ssp {

class BaseProcessor;

class BaseParamControl;

class ParamButton;

class BaseViewEditor :
    public BaseEditor {

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

    void editorShown() override;
    void editorHidden() override;

protected:
    using base_type = BaseEditor;
    using base_view = BaseEditor;

    void drawView(Graphics &g) override;
    void resized() override;
    void addView(std::shared_ptr<base_view>);

    virtual void setView(unsigned newView);
    void nextView();
    void previousView();

    std::vector<std::shared_ptr<base_view>> views_;

    int view_ = -1;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseViewEditor)
};

}




