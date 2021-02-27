#include "LineParamEditor.h"

#include "BaseParamControl.h"

namespace ssp {

static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


LineParamEditor::LineParamEditor(BaseProcessor *p, String title, String version)
    : base_type(p, title, version) {
    ;
}

void LineParamEditor::paint(Graphics &g) {
    base_type::paint(g);
}

void LineParamEditor::setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

    unsigned h = 2 * paramSpaceY;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = idx * w;
    unsigned y = paramTopY + 5;
    c->setBounds(x, y, w, h);
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


void LineParamEditor::onUpButton(bool v) {
    base_type::onUpButton(v);

    if (v) return; // change on button up

    if (paramPage_ > 0) {
        auto page = controlPages_[paramPage_];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) c->setVisible(false);
        }

        paramPage_--;

        page = controlPages_[paramPage_];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) c->setVisible(true);
        }
    }
}

void LineParamEditor::onDownButton(bool v) {
    base_type::onDownButton(v);

    if (v) return; // change on button up

    if ((paramPage_ + 1) < controlPages_.size()) {
        auto page = controlPages_[paramPage_];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                c->setVisible(false);
                c->active(false);
            }
        }

        paramPage_++;

        page = controlPages_[paramPage_];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                c->setVisible(true);
                c->active(true);
            }
        }
    }
}

} // namespace
