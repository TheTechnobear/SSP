#include "MultiViewEditor.h"

#include "ssp/BaseProcessor.h"

#include "SSP.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

namespace ssp {

MultiViewEditor::MultiViewEditor(BaseProcessor *p, unsigned maxView)
    : base_type(p) {
    setButtonBounds(upBtn_, 0, 5);
    addAndMakeVisible(upBtn_);

    setButtonBounds(downBtn_, 1, 5);
    addAndMakeVisible(downBtn_);

    setButtonBounds(leftBtn_, 1, 4);
    addAndMakeVisible(leftBtn_);

    setButtonBounds(rightBtn_, 1, 6);
    addAndMakeVisible(rightBtn_);


    // create views
    for (unsigned i = 0; i < maxView; i++) {
        View view;
        views_.push_back(view);
    }
}


MultiViewEditor::ControlPage MultiViewEditor::addParamPage(
    std::shared_ptr<BaseParamControl> c1,
    std::shared_ptr<BaseParamControl> c2,
    std::shared_ptr<BaseParamControl> c3,
    std::shared_ptr<BaseParamControl> c4,
    unsigned v,
    juce::Colour clr) {
    ControlPage page;
    page.control_[0] = c1;
    page.control_[1] = c2;
    page.control_[2] = c3;
    page.control_[3] = c4;

    setParamBounds(0, c1);
    setParamBounds(1, c2);
    setParamBounds(2, c3);
    setParamBounds(3, c4);

    if (c1) c1->fg(clr);
    if (c2) c2->fg(clr);
    if (c3) c3->fg(clr);
    if (c4) c4->fg(clr);

    if (v < views_.size()) {
        auto &view = views_[v];
        view.controlPages_.push_back(page);
        if (v == view_ && paramPage_ == view.controlPages_.size() - 1) {
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
    }

    return page;
}

void MultiViewEditor::addButtonPage(
    std::shared_ptr<ParamButton> c1,
    std::shared_ptr<ParamButton> c2,
    std::shared_ptr<ParamButton> c3,
    std::shared_ptr<ParamButton> c4,
    std::shared_ptr<ParamButton> c5,
    std::shared_ptr<ParamButton> c6,
    std::shared_ptr<ParamButton> c7,
    std::shared_ptr<ParamButton> c8,
    unsigned v
) {
    ButtonPage page;
    page.control_[0] = c1;
    page.control_[1] = c2;
    page.control_[2] = c3;
    page.control_[3] = c4;
    page.control_[4] = c5;
    page.control_[5] = c6;
    page.control_[6] = c7;
    page.control_[7] = c8;

    setButtonBounds(0, c1);
    setButtonBounds(1, c2);
    setButtonBounds(2, c3);
    setButtonBounds(3, c4);
    setButtonBounds(4, c5);
    setButtonBounds(5, c6);
    setButtonBounds(6, c7);
    setButtonBounds(7, c8);


    if (v < views_.size()) {
        auto &view = views_[v];
        view.buttonPages_.push_back(page);

        if (view_ == v && paramPage_ == view.buttonPages_.size() - 1) {
            for (auto i = 0; i < 8; i++) {
                auto c = page.control_[i];
                if (c) addAndMakeVisible(c.get());
            }
        } else {
            for (auto i = 0; i < 8; i++) {
                auto c = page.control_[i];
                if (c) addChildComponent(c.get());
            }
        }
    }
}


void MultiViewEditor::onEncoder(unsigned enc, float v) {
    base_type::onEncoder(enc, v);
    if (v > -0.01f && v < 0.01f) return;

    auto &view = views_[view_];
    if (paramPage_ < view.controlPages_.size()) {
        auto page = view.controlPages_[paramPage_];
        auto c = page.control_[enc];
        if (c != nullptr) {
            if (encoderState_[enc]) {
                encoderFine[enc] = true;
            }
            if (v > 0.0f) {
                c->inc(encoderFine[enc]);
            } else if (v < 0.0f) {
                c->dec(encoderFine[enc]);
            }
        }
    }
}

void MultiViewEditor::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    if (v) return; // change on button up

    auto &view = views_[view_];
    if (!encoderFine[enc]) {
        if (paramPage_ < view.controlPages_.size()) {
            auto page = view.controlPages_[paramPage_];
            auto c = page.control_[enc];
            if (c != nullptr) {
                if (v == 0) {
                    c->reset();
                }
            }
        }
    }
    encoderFine[enc] = false;
}

void MultiViewEditor::onButton(unsigned btn, bool v) {
    base_type::onButton(btn, v);

    auto &view = views_[view_];
    if (buttonPage_ < view.buttonPages_.size()) {
        auto page = view.buttonPages_[buttonPage_];
        auto c = page.control_[btn];
        if (c != nullptr) {
            if (v) c->onDown();
            else c->onUp();
        }
    }
}

void MultiViewEditor::eventButton(unsigned btn, bool v) {
    base_type::eventButton(btn, v);

    auto &view = views_[view_];
    if (buttonPage_ < view.buttonPages_.size()) {
        auto page = view.buttonPages_[buttonPage_];
        auto c = page.control_[btn];
        if (c != nullptr) {
            c->onClick();
        }
    }
}


void MultiViewEditor::chgParamPage(int delta, bool changeVis) {
    auto &view = views_[view_];
    unsigned nextP = paramPage_;
    if (delta < 0) {
        nextP = paramPage_ > 0 ? paramPage_ - 1 : paramPage_;
    } else if (delta > 0) {
        nextP = paramPage_ + 1 < view.controlPages_.size() ? paramPage_ + 1 : paramPage_;
    }
    if (nextP != paramPage_) setParamPage(nextP, changeVis);
}

void MultiViewEditor::setParamPage(unsigned p, bool changeVis) {
    auto &view = views_[view_];
    unsigned lastP = paramPage_;
    unsigned nextP = p;
    if (lastP != nextP) {
        paramPage_ = nextP;
        auto page = view.controlPages_[lastP];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                if (changeVis) c->setVisible(false);
                c->active(false);
            }
        }
        page = view.controlPages_[nextP];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                if (changeVis) c->setVisible(true);
                c->active(true);
            }
        }
    }
}

void MultiViewEditor::chgButtonPage(int delta) {
    auto &view = views_[view_];
    unsigned nextP = buttonPage_;
    if (delta < 0) {
        nextP = buttonPage_ > 0 ? buttonPage_ - 1 : buttonPage_;
    } else if (delta > 0) {
        nextP = buttonPage_ + 1 < view.buttonPages_.size() ? buttonPage_ + 1 : buttonPage_;
    }
    if (nextP != buttonPage_) setButtonPage(nextP);
}

void MultiViewEditor::setButtonPage(unsigned p) {
    static constexpr unsigned MAX_BTN = 8;
    auto &view = views_[view_];
    unsigned lastP = buttonPage_;
    unsigned nextP = p;

    if (lastP != nextP) {
        if(buttonPage_ < view.buttonPages_.size()) {
            buttonPage_ = nextP;
            auto page = view.buttonPages_[lastP];
            for (auto i = 0; i < MAX_BTN; i++) {
                auto &c = page.control_[i];
                if (c != nullptr) {
                    c->setVisible(false);
                    //                c->active(false);
                }
            }
        }

        if(nextP < view.buttonPages_.size()) {
            auto page = view.buttonPages_[nextP];
            for (auto i = 0; i < MAX_BTN; i++) {
                auto &c = page.control_[i];
                if (c != nullptr) {
                    c->setVisible(true);
//                c->active(true);
                }
            }
        }
    }
}

void MultiViewEditor::chgView(int delta) {
    unsigned nextV = view_;
    if (delta < 0) {
        nextV = view_ > 0 ? view_ - 1 : view_;
    } else if (delta > 0) {
        nextV = view_ + 1 < views_.size() ? view_ + 1 : view_;
    }
    if (nextV != view_) setView(nextV);
}

void MultiViewEditor::setView(unsigned v) {
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
            auto ppage = view.controlPages_[paramPage_];
            for (auto i = 0; i < 4; i++) {
                auto &c = ppage.control_[i];
                if (c != nullptr) {
                    c->setVisible(false);
                    c->active(false);
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
            auto ppage = view.controlPages_[paramPage_];
            for (auto i = 0; i < 4; i++) {
                auto &c = ppage.control_[i];
                if (c != nullptr) {
                    c->setVisible(true);
                    c->active(true);
                }
            }
        }
    }
}

void MultiViewEditor::eventUp(bool v) {
    base_type::eventUp(v);

    if (v) return; // change on button up
    chgParamPage(-1, true);
}

void MultiViewEditor::eventDown(bool v) {
    base_type::eventDown(v);

    if (v) return; // change on button up
    chgParamPage(1, true);
}


void MultiViewEditor::eventLeft(bool v) {
    base_type::eventLeft(v);

    if (v) return; // change on button up
    chgButtonPage(-1);
}

void MultiViewEditor::eventRight(bool v) {
    base_type::eventRight(v);

    if (v) return; // change on button up
    chgButtonPage(1);
}


void MultiViewEditor::setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl> c) {
    static constexpr unsigned paramTopY = 380 - 1;
    static constexpr unsigned paramSpaceY = 50;
    if (c == nullptr) return;

    unsigned h = 2 * paramSpaceY;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = idx * w;
    unsigned y = paramTopY + 5;

    unsigned sp = 2;
    c->setBounds(x + sp, y, w - sp * 2, h);
}

}//ssp