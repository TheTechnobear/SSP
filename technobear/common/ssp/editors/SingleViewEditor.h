#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "BaseEditor.h"

namespace ssp {

class BaseParamControl;

class ParamButton;

class SingleViewEditor :
    public BaseEditor {

public:
    explicit SingleViewEditor(BaseProcessor *p);

    virtual void onEncoder(unsigned enc, float v);
    virtual void onEncoderSwitch(unsigned enc, bool v);

    virtual void onButton(unsigned btn, bool v);
    virtual void eventButton(unsigned btn,bool longPress); 

    virtual ControlPage addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4
    );

    void addButtonPage(
        std::shared_ptr<ParamButton> c1,
        std::shared_ptr<ParamButton> c2,
        std::shared_ptr<ParamButton> c3,
        std::shared_ptr<ParamButton> c4,
        std::shared_ptr<ParamButton> c5,
        std::shared_ptr<ParamButton> c6,
        std::shared_ptr<ParamButton> c7,
        std::shared_ptr<ParamButton> c8
    );
protected:
    virtual void chgParamPage(int delta, bool changeVis=false);
    virtual void chgButtonPage(int delta);


    std::vector<ControlPage> controlPages_;
    std::vector<ButtonPage> buttonPages_;

    bool encoderFine[4] = {false, false, false, false};
    unsigned paramPage_ = 0;
    unsigned buttonPage_ = 0;

private:
    using base_type = BaseEditor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleViewEditor)
};

}




