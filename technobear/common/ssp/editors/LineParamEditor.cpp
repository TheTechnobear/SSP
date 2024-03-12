#include "LineParamEditor.h"

#include "ssp/controls/ParamControl.h"

namespace ssp {

static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


LineParamEditor::LineParamEditor(BaseProcessor *p)
    : base_type(p) {
    ;
}

void LineParamEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}

void LineParamEditor::setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

    unsigned h = 2 * paramSpaceY;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = idx * w;
    unsigned y = paramTopY + 5;
    unsigned sp = 2;
    c->setBounds(x + sp, y, w - sp * 2, h);
}

BaseEditor::ControlPage LineParamEditor::addParamPage(
    std::shared_ptr<BaseParamControl> c1,
    std::shared_ptr<BaseParamControl> c2,
    std::shared_ptr<BaseParamControl> c3,
    std::shared_ptr<BaseParamControl> c4
) {
    auto page = base_type::addParamPage(c1, c2, c3, c4);

    setParamBounds(0, c1);
    setParamBounds(1, c2);
    setParamBounds(2, c3);
    setParamBounds(3, c4);

    if (paramPage_ == controlPages_.size() - 1) {
        for (auto i = 0; i < 4; i++) {
            auto c = page.control_[i];
            if (c) {
                addAndMakeVisible(c.get());
                c->active(true);
            }
        }
    } else {
        for (auto i = 0; i < 4; i++) {
            auto c = page.control_[i];
            if (c) {
                addChildComponent(c.get());
                c->active(false);
            }
        }
    }
    return page;
}


void LineParamEditor::eventUp(bool v) {
    base_type::eventUp(v);

    if (v) return; 
    chgParamPage(-1,true);
}

void LineParamEditor::eventDown(bool v) {
    base_type::eventDown(v);

    if (v) return; 
    chgParamPage(1,true);
}

} // namespace
