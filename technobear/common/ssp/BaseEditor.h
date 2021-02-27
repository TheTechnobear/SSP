#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "SSPButton.h"
#include "SSPParam.h"

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

    void paint(Graphics &) override;
    void resized() override;

    virtual void onEncoder(unsigned enc, float v);
    virtual void onEncoderSwitch(unsigned enc, bool v);

    // user buttons
    virtual void onButton(unsigned btn, bool v);

    // nav buttons
    virtual void onLeftButton(bool v);
    virtual void onRightButton(bool v);
    virtual void onUpButton(bool v);
    virtual void onDownButton(bool v);
    virtual void onLeftShiftButton(bool v);
    virtual void onRightShiftButton(bool v);


protected:
    using base_type=juce::AudioProcessorEditor;

//    virtual void setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl>)=0;

    virtual void drawMenuBox(Graphics &g);
    virtual void drawButtonBox(Graphics &g);
    void setButtonBounds(unsigned idx, std::shared_ptr<ButtonControl>);

    // temp
    void setMenuBounds(SSPButton &btn, unsigned r);
//    void setParamBounds(SSPParam &par, unsigned enc, unsigned var);
    void setButtonBounds(SSPButton &btn, unsigned r, unsigned c);

    BaseProcessor *baseProcessor_;
    std::vector<std::unique_ptr<ParameterAttachment>> encAtt_;
    std::vector<std::unique_ptr<ParameterAttachment>> encSwAtt_;
    std::vector<std::unique_ptr<ParameterAttachment>> btnAtt_;

    ParameterAttachment leftAtt_, rightAtt_, upAtt_, downAtt_, LSAtt_, RSAtt_;


    struct ControlPage {
        std::shared_ptr<BaseParamControl> control_[4] = {nullptr, nullptr, nullptr, nullptr};
    };

    std::vector<ControlPage> controlPages_;


    struct ButtonPage {
        std::shared_ptr<ButtonControl> control_[8] = {
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr
        };
    };

    virtual ControlPage  addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4
    );

    void addButtonPage(
        std::shared_ptr<ButtonControl> c1,
        std::shared_ptr<ButtonControl> c2,
        std::shared_ptr<ButtonControl> c3,
        std::shared_ptr<ButtonControl> c4,
        std::shared_ptr<ButtonControl> c5,
        std::shared_ptr<ButtonControl> c6,
        std::shared_ptr<ButtonControl> c7,
        std::shared_ptr<ButtonControl> c8
    );

    std::vector<ButtonPage> buttonPages_;

    SSPButton leftBtn_, rightBtn_, upBtn_, downBtn_;
    SSPButton leftShiftBtn_, rightShiftBtn_;
    bool encoderFine[4]={false, false, false, false};
    bool encoderState_[4]={false,false,false,false};


    unsigned paramPage_ = 0;

private:
    SSPButton globalBtn_, networkBtn_, plugInBtn_, recBtn_;
    String title_, version_;
    unsigned buttonPage_ = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseEditor)
};

}




