#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "MultiViewEditor.h"

namespace ssp {

class BaseProcessor;

class BaseParamControl;

class ParamButton;

class MultiBarEditor :
    public MultiViewEditor {

public:
    explicit MultiBarEditor(BaseProcessor *p,
                            unsigned maxView);

    ControlPage addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4,
        unsigned view,
        juce::Colour clr = Colours::red
    ) override;

    void setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl>);
    void setView(unsigned v) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;


    void eventUp(bool v) override;
    void eventDown(bool v) override;
    void eventLeft(bool v) override;
    void eventRight(bool v) override;

protected:

private:
    using base_type = MultiViewEditor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiBarEditor)
};

}




