#pragma once

#include "SingleViewEditor.h"

namespace ssp {

class LineParamEditor : public SingleViewEditor {
public:
    LineParamEditor(BaseProcessor *p, String title, String version);

protected:
    using base_type=ssp::SingleViewEditor;

    void paint(Graphics &) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl>);
    ControlPage addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4
    ) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LineParamEditor)

};

}