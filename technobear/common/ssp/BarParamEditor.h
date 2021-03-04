#pragma once

#include "LineParamEditor.h"

namespace ssp {

class BarParamEditor : public LineParamEditor {
public:
    BarParamEditor(BaseProcessor *p, String title, String version);

protected:
    using base_type = ssp::LineParamEditor;

    void paint(Graphics &) override;

    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl>);
    ControlPage addParamPage(
        std::shared_ptr<BaseParamControl> c1,
        std::shared_ptr<BaseParamControl> c2,
        std::shared_ptr<BaseParamControl> c3,
        std::shared_ptr<BaseParamControl> c4
    ) override;
};

}