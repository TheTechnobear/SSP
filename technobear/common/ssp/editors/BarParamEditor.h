#pragma once

#include "LineParamEditor.h"

namespace ssp {

class BarParamEditor : public LineParamEditor {
public:
    BarParamEditor(BaseProcessor *p, bool autoColour=true);

    void drawView(Graphics &) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    ControlPage addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4
    ) override;

protected:
    using base_type = ssp::LineParamEditor;
    void setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl>);

    // colour page parameters using std colour scheme
    bool autoColour_=true;
};
}