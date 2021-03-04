#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

using namespace juce;

#include "BaseEditor.h"

namespace ssp {

class BaseParamControl;

class ButtonControl;

class SingleViewEditor :
    public BaseEditor {

public:
    explicit SingleViewEditor(BaseProcessor *p, String title, String version);

    virtual void onEncoder(unsigned enc, float v);
    virtual void onEncoderSwitch(unsigned enc, bool v);
    virtual void onButton(unsigned btn, bool v);

protected:
    using base_type = BaseEditor;
    virtual void chgParamPage(int delta, bool changeVis=false);
    virtual void chgButtonPage(int delta);

    virtual ControlPage addParamPage(
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

    std::vector<ControlPage> controlPages_;
    std::vector<ButtonPage> buttonPages_;

    bool encoderFine[4] = {false, false, false, false};
    unsigned paramPage_ = 0;
    unsigned buttonPage_ = 0;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleViewEditor)
};

}




