#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "ValueButton.h"

namespace ssp {

class BaseProcessor;

class BaseParamControl;

class ParamButton;

class EditorHost;


#include "SSPActions.h"

class BaseEditor :
    public Component,
    public Timer,
    public SSPActions {

public:
    explicit BaseEditor(BaseProcessor *p);
    virtual ~BaseEditor();

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
    void onSSPTimer() override;


    void setButtonBounds(unsigned idx, std::shared_ptr<ParamButton>);

    // temp
    void setButtonBounds(ValueButton &btn, unsigned r, unsigned c);

    BaseProcessor *baseProcessor_;

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
    void paint(Graphics &) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseEditor)
};

}




