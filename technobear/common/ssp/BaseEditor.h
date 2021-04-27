#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "SSPButton.h"

namespace ssp {

class BaseProcessor;

class BaseParamControl;
class ButtonControl;

class BaseEditor :
    public AudioProcessorEditor,
    public Timer {

public:
    explicit BaseEditor(BaseProcessor *p, String title, String version);
    virtual ~BaseEditor();

    void timerCallback() override;

    void resized() override;

    virtual void onEncoder(unsigned enc, float v);
    virtual void onEncoderSwitch(unsigned enc, bool v);

    virtual void onButton(unsigned btn, bool v);
    virtual void onLeftButton(bool v);
    virtual void onRightButton(bool v);
    virtual void onUpButton(bool v);
    virtual void onDownButton(bool v);
    virtual void onLeftShiftButton(bool v);
    virtual void onRightShiftButton(bool v);


protected:

    using base_type=juce::AudioProcessorEditor;

    virtual void drawMenuBox(Graphics &g);
    virtual void drawButtonBox(Graphics &g);
    virtual void drawView(Graphics &g);

    void setButtonBounds(unsigned idx, std::shared_ptr<ButtonControl>);

    // temp
    void setMenuBounds(SSPButton &btn, unsigned r);
    void setButtonBounds(SSPButton &btn, unsigned r, unsigned c);

    BaseProcessor *baseProcessor_;
    std::vector<std::unique_ptr<ParameterAttachment>> encAtt_;
    std::vector<std::unique_ptr<ParameterAttachment>> encSwAtt_;
    std::vector<std::unique_ptr<ParameterAttachment>> btnAtt_;

    ParameterAttachment leftAtt_, rightAtt_, upAtt_, downAtt_, LSAtt_, RSAtt_;

    struct ControlPage {
        std::shared_ptr<BaseParamControl> control_[4] = {nullptr, nullptr, nullptr, nullptr};
    };

    struct ButtonPage {
        std::shared_ptr<ButtonControl> control_[8] = {
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr
        };
    };


    bool encoderState_[4]={false,false,false,false};
    SSPButton leftBtn_, rightBtn_, upBtn_, downBtn_;
    SSPButton leftShiftBtn_, rightShiftBtn_;



private:


    void paint(Graphics &) override;




    SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;
    String title_, version_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseEditor)
};

}




