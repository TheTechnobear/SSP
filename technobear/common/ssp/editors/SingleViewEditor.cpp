#include "SingleViewEditor.h"

#include "ssp/BaseProcessor.h"

#include "SSP.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

namespace ssp {

SingleViewEditor::SingleViewEditor(BaseProcessor *p)
    : base_type(p) {

    setButtonBounds(upBtn_, 0, 5);
    addAndMakeVisible(upBtn_);

    setButtonBounds(downBtn_, 1, 5);
    addAndMakeVisible(downBtn_);
}


SingleViewEditor::ControlPage SingleViewEditor::addParamPage(
    std::shared_ptr<BaseParamControl> c1,
    std::shared_ptr<BaseParamControl> c2,
    std::shared_ptr<BaseParamControl> c3,
    std::shared_ptr<BaseParamControl> c4
) {
    ControlPage page;
    page.control_[0] = c1;
    page.control_[1] = c2;
    page.control_[2] = c3;
    page.control_[3] = c4;
    controlPages_.push_back(page);
    return page;
}

void SingleViewEditor::addButtonPage(
    std::shared_ptr<ParamButton> c1,
    std::shared_ptr<ParamButton> c2,
    std::shared_ptr<ParamButton> c3,
    std::shared_ptr<ParamButton> c4,
    std::shared_ptr<ParamButton> c5,
    std::shared_ptr<ParamButton> c6,
    std::shared_ptr<ParamButton> c7,
    std::shared_ptr<ParamButton> c8
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
    buttonPages_.push_back(page);

    setButtonBounds(0, c1);
    setButtonBounds(1, c2);
    setButtonBounds(2, c3);
    setButtonBounds(3, c4);
    setButtonBounds(4, c5);
    setButtonBounds(5, c6);
    setButtonBounds(6, c7);
    setButtonBounds(7, c8);

    if (paramPage_ == buttonPages_.size() - 1) {
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


void SingleViewEditor::onEncoder(unsigned enc, float v) {
//    std::cerr << "onEncoder " << enc <<  " : " << v << std::endl;
    base_type::onEncoder(enc, v);
    if (v > -0.01f && v < 0.01f) return;

    if (paramPage_ < controlPages_.size()) {
        auto page = controlPages_[paramPage_];
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

void SingleViewEditor::onEncoderSwitch(unsigned enc, bool v) {
//    std::cerr << "onEncoderSwitch " << enc <<  " : " << v << std::endl;

    base_type::onEncoderSwitch(enc, v);

    if (v) return; // change on button up

    if (!encoderFine[enc]) {
        if (paramPage_ < controlPages_.size()) {
            auto page = controlPages_[paramPage_];
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

void SingleViewEditor::onButton(unsigned btn, bool v) {
//    std::cerr << "SingleViewEditor:: onButton " << btn <<  " : " << v << std::endl;

    base_type::onButton(btn, v);

    if (buttonPage_ < buttonPages_.size()) {
        auto page = buttonPages_[buttonPage_];
        auto c = page.control_[btn];
        if (c != nullptr) {
            if (v) c->onDown();
            else c->onUp();
        }
    }
}

void SingleViewEditor::eventButton(unsigned btn,bool longPress) {
    base_type::eventButton(btn, longPress);
    if(longPress) return;

    if (buttonPage_ < buttonPages_.size()) {
        auto page = buttonPages_[buttonPage_];
        auto c = page.control_[btn];
        if (c != nullptr) {
            c->onClick();
        }
    }
}


void SingleViewEditor::chgParamPage(int delta, bool changeVis) {
    unsigned lastP = paramPage_;
    unsigned nextP = paramPage_;
    if (delta < 0) {
        nextP = paramPage_ > 0 ? paramPage_- 1 : paramPage_;
    } else if (delta > 0) {
        nextP = paramPage_ + 1 < controlPages_.size() ? paramPage_+ 1 : paramPage_;
    }

    if (lastP != nextP) {
        paramPage_=nextP;
        auto page = controlPages_[lastP];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                if(changeVis)c->setVisible(false);
                c->active(false);
            }
        }
        page = controlPages_[nextP];
        for (auto i = 0; i < 4; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                if(changeVis) c->setVisible(true);
                c->active(true);
            }
        }
    }
}

void SingleViewEditor::chgButtonPage(int delta) {
    static constexpr unsigned MAX_BTN = 8;

    unsigned lastP = buttonPage_;
    unsigned nextP = buttonPage_;
    if (delta < 0) {
        nextP = buttonPage_ > 0 ? buttonPage_- 1 : buttonPage_;
    } else if (delta > 0) {
        nextP = buttonPage_ + 1 < buttonPages_.size() ? buttonPage_+ 1 : buttonPage_;
    }

    if(lastP != nextP) {
        buttonPage_=nextP;
        auto page = buttonPages_[lastP];
        for (auto i = 0; i < MAX_BTN; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                c->setVisible(false);
//                c->active(false);
            }
        }

        page = buttonPages_[nextP];
        for (auto i = 0; i < MAX_BTN; i++) {
            auto &c = page.control_[i];
            if (c != nullptr) {
                c->setVisible(true);
//                c->active(true);
            }
        }
    }
}

}//ssp