#pragma once

#include "SingleViewEditor.h"

namespace ssp {

class LineParamEditor : public SingleViewEditor {
public:
    LineParamEditor(BaseProcessor *p);


    void drawView(juce::Graphics &) override;

    void eventUp(bool v) override;
    void eventDown(bool v) override;
    ControlPage addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4
    ) override;
protected:
    void setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl>);

private:
    using base_type=ssp::SingleViewEditor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LineParamEditor)

};

}