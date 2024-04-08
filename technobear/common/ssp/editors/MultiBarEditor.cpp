#include "MultiBarEditor.h"

#include "ssp/BaseProcessor.h"

#include "SSP.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

namespace ssp {

static constexpr unsigned paramSpaceY = 34;

MultiBarEditor::MultiBarEditor(BaseProcessor *p, unsigned maxView)
    : base_type(p, maxView) {
}

MultiViewEditor::ControlPage MultiBarEditor::addParamPage(
    std::shared_ptr<BaseParamControl> c1,
    std::shared_ptr<BaseParamControl> c2,
    std::shared_ptr<BaseParamControl> c3,
    std::shared_ptr<BaseParamControl> c4,
    unsigned view,
    juce::Colour clr
) {
    auto page = base_type::addParamPage(c1, c2, c3, c4, view, clr);
    if (view < views_.size()) {
        auto &v = views_[view];
        unsigned pageN = v.controlPages_.size() - 1;
        setParamBounds(pageN, 0, c1);
        setParamBounds(pageN, 1, c2);
        setParamBounds(pageN, 2, c3);
        setParamBounds(pageN, 3, c4);

        for (const auto &c : page.control_) {
            if (c) {
                c->setVisible(view == view_);
                c->active(view == view_ && pageN == paramPage_);
            }
        }
    }
    return page;
}


void MultiBarEditor::setView(unsigned v) {
    unsigned lastV = view_;
    unsigned nextV = v;
    if (lastV != nextV) {
        view_ = nextV;
        static constexpr unsigned MAX_BTN = 8;

        {  // display current buttons/params
            auto &view = views_[lastV];
            if(buttonPage_ < view.buttonPages_.size()) {
                auto bpage = view.buttonPages_[buttonPage_];
                for (auto i = 0; i < MAX_BTN; i++) {
                    auto &c = bpage.control_[i];
                    if (c != nullptr) {
                        c->setVisible(false);
                    }
                }
            }

            for (auto ppage : view.controlPages_) {
                for (auto i = 0; i < 4; i++) {
                    auto &c = ppage.control_[i];
                    if (c != nullptr) {
                        c->setVisible(false);
                        c->active(false);
                    }
                }
            }
        }

        {   // enable new buttons/controls
            auto &view = views_[nextV];
            if(buttonPage_ >= view.buttonPages_.size()) {
                buttonPage_=0;
            }
            if(buttonPage_ < view.buttonPages_.size()) {
                auto bpage = view.buttonPages_[buttonPage_];
                for (auto i = 0; i < MAX_BTN; i++) {
                    auto &c = bpage.control_[i];
                    if (c != nullptr) {
                        c->setVisible(true);
                    }
                }
            }

            unsigned pageN = 0;
            if (paramPage_ >= view.controlPages_.size()) {
                paramPage_ = view.controlPages_.size() - 1;
            }
            for (auto ppage : view.controlPages_) {
                for (auto i = 0; i < 4; i++) {
                    auto &c = ppage.control_[i];
                    if (c != nullptr) {
                        c->setVisible(true);
                        c->active(paramPage_ == pageN);
                    }
                }
                pageN++;
            }
        }
    }
}

void MultiBarEditor::onUpButton(bool v) {
    upBtn_.onButton(v);
}

void MultiBarEditor::onDownButton(bool v) {
    downBtn_.onButton(v);
}


void MultiBarEditor::eventUp(bool v) {
    if (v) return; 
    chgParamPage(-1, false);
}

void MultiBarEditor::eventDown(bool v) {
    if (v) return;
    chgParamPage(1, false);
}


void MultiBarEditor::eventLeft(bool v) {
    if (v) return;
    chgView(-1);
}

void MultiBarEditor::eventRight(bool v) {
    if (v) return;
    chgView(1);
}


void MultiBarEditor::setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

    int w = int(900.0f / 4.0f);
    int x = int(idx) * w;
    int h = 2 * paramSpaceY;
    int y = (int(page) * h) + h;

    int sp = 2;
    c->setBounds(x + sp, y, w - sp * 2, h);
}

}//ssp