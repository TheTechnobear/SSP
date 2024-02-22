#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

using namespace juce;

#include "ssp/BaseView.h"
#include "ssp/controls/ValueButton.h"

namespace ssp {

class BaseProcessor;

class BaseParamControl;

class ParamButton;

#include "ssp/SSPActions.h"

class BaseEditor : public BaseView {

public:
    explicit BaseEditor(BaseProcessor *p);
    ~BaseEditor() override;


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
    void setButtonBounds(unsigned idx, std::shared_ptr<ParamButton>);
    // temp
    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);

    struct ControlPage {
        std::shared_ptr<BaseParamControl> control_[4] = {nullptr, nullptr, nullptr, nullptr};
    };

    struct ButtonPage {
        std::shared_ptr<ParamButton> control_[8] = {
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr
        };
    };

    bool encoderState_[4] = {false, false, false, false};


    ValueButton leftBtn_, rightBtn_, upBtn_, downBtn_;
    ValueButton leftShiftBtn_, rightShiftBtn_;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseEditor)
};

}




