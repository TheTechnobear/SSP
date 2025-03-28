#include "BaseMiniView.h"

#include "ssp/BaseProcessor.h"

namespace ssp {


LineMiniEditor::LineMiniEditor(BaseProcessor *p) : base_type(p, true) {
    ;
}

void LineMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), titleFH, Font::plain)));

    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + ":" + String(JucePlugin_Desc) + String(" @ thetechnobear"), gap,
                         gap * 2);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString), 270 * COMPACT_UI_SCALE, gap * 2);

    drawButtonBox(g);
}

void LineMiniEditor::resized() {
    base_type::resized();

    for (auto &btnPage : buttonPages_) {
        int bidx = 0;
        for (auto &b : btnPage->control_) {
            if (b) setButtonBounds(bidx, b.get());
            bidx++;
        }
    }
}

void LineMiniEditor::drawButtonBox(Graphics &g) {
    g.setColour(Colours::darkgrey);
    unsigned singleButtonH = buttonBarH / 2;
    g.drawHorizontalLine(SSP_COMPACT_HEIGHT - buttonBarH - 1, gap, SSP_COMPACT_WIDTH - gap);
    g.drawHorizontalLine(SSP_COMPACT_HEIGHT - singleButtonH - 1, gap, SSP_COMPACT_WIDTH - gap);
    g.drawHorizontalLine(SSP_COMPACT_HEIGHT - 1, gap, SSP_COMPACT_WIDTH - gap);

    for (int i = 0; i < nParamsPerPage + 1; i++) {
        unsigned x = gap + (i * gridW);
        g.drawVerticalLine(x, SSP_COMPACT_HEIGHT - buttonBarH, SSP_COMPACT_HEIGHT - 1);
    }
}


void LineMiniEditor::setParamBounds(unsigned idx, juce::Component *p) {
    if (p == nullptr) return;

    int offset = gap + (gap / 2);
    unsigned c = idx % 4;
    int w = paramWidth;
    int h = paramHeight;
    int x = offset + (c * gridW);
    int y = SSP_COMPACT_HEIGHT - buttonBarH - paramHeight;
    p->setBounds(x, y, w, h);
}

void LineMiniEditor::setButtonBounds(unsigned bidx, juce::Component *p) {
    if (p == nullptr) return;

    int offset = gap + (gap / 2);
    unsigned r = bidx / 4;
    unsigned c = bidx % 4;
    int w = paramWidth;
    int h = buttonBarH / 2;
    int x = offset + (c * gridW);
    int y = SSP_COMPACT_HEIGHT - buttonBarH + (r * h);
    if (p) p->setBounds(x, y, w, h);
}


void LineMiniEditor::addParamPage(std::shared_ptr<BaseParamControl> c1, std::shared_ptr<BaseParamControl> c2,
                                  std::shared_ptr<BaseParamControl> c3, std::shared_ptr<BaseParamControl> c4,
                                  const juce::Colour &fg) {
    auto page = std::make_shared<ControlPage>();
    page->control_[0] = c1;
    page->control_[1] = c2;
    page->control_[2] = c3;
    page->control_[3] = c4;
    controlPages_.push_back(page);

    for (int i = 0; i < nParamsPerPage; i++) {
        auto c = page->control_[i];
        if (c) {
            c->fg(fg);
            setParamBounds(i, c.get());
        }
    }

    if (paramPage_ == controlPages_.size() - 1) {
        for (auto i = 0; i < 4; i++) {
            auto c = page->control_[i];
            if (c) {
                addAndMakeVisible(c.get());
                c->active(true);
            }
        }
    } else {
        for (auto i = 0; i < nParamsPerPage; i++) {
            auto c = page->control_[i];
            if (c) {
                addChildComponent(c.get());
                c->active(false);
            }
        }
    }
}

void LineMiniEditor::addButtonPage(std::shared_ptr<ParamButton> c1, std::shared_ptr<ParamButton> c2,
                                   std::shared_ptr<ParamButton> c3, std::shared_ptr<ParamButton> c4,
                                   std::shared_ptr<ParamButton> c5, std::shared_ptr<ParamButton> c6,
                                   std::shared_ptr<ParamButton> c7, std::shared_ptr<ParamButton> c8) {
    auto page = std::make_shared<ButtonPage>();
    page->control_[0] = c1;
    page->control_[1] = c2;
    page->control_[2] = c3;
    page->control_[3] = c4;
    page->control_[4] = c5;
    page->control_[5] = c6;
    page->control_[6] = c7;
    page->control_[7] = c8;
    buttonPages_.push_back(page);

    for (int i = 0; i < maxUserBtns; i++) {
        auto c = page->control_[i];
        if (c) { setButtonBounds(i, c.get()); }
    }

    if (paramPage_ == buttonPages_.size() - 1) {
        for (auto i = 0; i < maxUserBtns; i++) {
            auto c = page->control_[i];
            if (c) {
                addAndMakeVisible(c.get());
                // c->active(true);
            }
        }
    } else {
        for (auto i = 0; i < maxUserBtns; i++) {
            auto c = page->control_[i];
            if (c) {
                addChildComponent(c.get());
                // c->active(false);
            }
        }
    }
}


void LineMiniEditor::chgParamPage(int inc, bool changeVis) {
    if (controlPages_.empty()) return;

    int newPage = paramPage_ + inc;
    if (newPage < 0) newPage = controlPages_.size() - 1;
    if (newPage >= controlPages_.size()) newPage = 0;

    if (newPage != paramPage_ && changeVis) {
        auto &opage = controlPages_[paramPage_];
        for (auto i = 0; i < nParamsPerPage; i++) {
            auto c = opage->control_[i];
            if (c) {
                c->active(false);
                c->setVisible(false);
            }
        }
        if (paramPage_ < buttonPages_.size()) {
            for (auto i = 0; i < maxUserBtns; i++) {
                auto b = buttonPages_[paramPage_]->control_[i];
                if (b) {
                    // b->active(false);
                    b->setVisible(false);
                }
            }
        }
        // setup new page
        paramPage_ = newPage;
        auto npage = controlPages_[paramPage_];
        for (auto i = 0; i < nParamsPerPage; i++) {
            auto c = npage->control_[i];
            if (c) {
                c->active(true);
                c->setVisible(true);
            }
        }

        if (paramPage_ < buttonPages_.size()) {
            for (auto i = 0; i < maxUserBtns; i++) {
                auto b = buttonPages_[paramPage_]->control_[i];
                if (b) {
                    // b->active(false);
                    b->setVisible(true);
                }
            }
        } else {
            if (buttonPages_.size() == 1)
                for (auto i = 0; i < maxUserBtns; i++) {
                    auto b = buttonPages_[0]->control_[i];
                    if (b) {
                        // b->active(false);
                        b->setVisible(true);
                    }
                }
        }
    }
    paramPage_ = newPage;
}


void LineMiniEditor::eventUp(bool v) {
    base_type::eventUp(v);

    if (v) return;
    chgParamPage(-1, true);
}

void LineMiniEditor::eventDown(bool v) {
    base_type::eventDown(v);

    if (v) return;
    chgParamPage(1, true);
}

void LineMiniEditor::onEncoder(unsigned enc, float v) {
    base_type::onEncoder(enc, v);
    if (v > -0.01f && v < 0.01f) return;

    auto page = controlPages_[paramPage_];
    auto c = page->control_[enc];
    if (c != nullptr) {
        if (encoderState_[enc]) { encoderFine_[enc] = true; }
        if (v > 0.0f) {
            c->inc(encoderFine_[enc]);
        } else if (v < 0.0f) {
            c->dec(encoderFine_[enc]);
        }
    }
}

void LineMiniEditor::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    encoderState_[enc] = v;
    if (v) return;  // change on button up

    if (!encoderFine_[enc]) {
        auto page = controlPages_[paramPage_];
        auto c = page->control_[enc];
        if (c) {
            if (v == 0) { c->reset(); }
        }
    }
    encoderFine_[enc] = false;
}

void LineMiniEditor::onButton(unsigned int bidx, bool v) {
    base_type::onButton(bidx, v);

    int page = buttonPages_.size() > 1 ? paramPage_ : 0;
    auto &btnPage = buttonPages_[page];
    if (btnPage) {
        auto &btn = btnPage->control_[bidx];
        if (btn) {
            if (v)
                btn->onDown();
            else
                btn->onUp();
        }
    }
}


void LineMiniEditor::eventButton(unsigned int bidx, bool longPress) {
    base_type::eventButton(bidx, longPress);
    if (longPress) return;

    int page = buttonPages_.size() > 1 ? paramPage_ : 0;
    auto &btnPage = buttonPages_[page];
    if (btnPage) {
        auto &btn = btnPage->control_[bidx];
        if (btn) btn->onClick();
    }
}


} // namespace ssp
