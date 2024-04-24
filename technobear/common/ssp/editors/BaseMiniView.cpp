#include "BaseMiniView.h"

#include "ssp/BaseProcessor.h"

namespace ssp {

BaseMiniView::BaseMiniView(BaseProcessor *p) : base_type(p, true) {
}


MiniParamView::MiniParamView(BaseProcessor *p, ioActivity cb) : base_type(p, true), ioCallback_(cb) {
}

void MiniParamView::drawView(Graphics &g) {
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 10 * COMPACT_UI_SCALE, Font::plain));

    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + ":" + String(JucePlugin_Desc) + String(" @ thetechnobear"), gap,
                         gap * 2);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString), 270 * COMPACT_UI_SCALE, gap * 2);

    drawButtonBox(g);
    drawIO(g);
}


void MiniParamView::addParam(const std::shared_ptr<BaseParamControl> &p) {
    params_.push_back(p);
    addChildComponent(p.get());

    bool act = params_.size() <= (paramOffset_ + nParamPerPage);
    p->active(act);
}

void MiniParamView::addButton(const std::shared_ptr<ParamButton> &p) {
    buttons_.push_back(p);
    jassert(buttons_.size() <= maxUserBtns);
    addAndMakeVisible(p.get());
}


void MiniParamView::onEncoder(unsigned enc, float v) {
    base_type::onEncoder(enc, v);
    if (v > -0.01f && v < 0.01f) return;

    if ((enc + paramOffset_) < params_.size()) {
        auto c = params_[enc + paramOffset_];
        if (c != nullptr) {
            if (encoderState_[enc]) { encoderFine_[enc] = true; }
            if (v > 0.0f) {
                c->inc(encoderFine_[enc]);
            } else if (v < 0.0f) {
                c->dec(encoderFine_[enc]);
            }
        }
    }
}

void MiniParamView::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    encoderState_[enc] = v;
    if (v) return;  // change on button up

    if (!encoderFine_[enc]) {
        if ((enc + paramOffset_) < params_.size()) {
            auto c = params_[enc + paramOffset_];
            if (c != nullptr) {
                if (v == 0) { c->reset(); }
            }
        }
    }
    encoderFine_[enc] = false;
}

void MiniParamView::onButton(unsigned int id, bool v) {
    base_type::onButton(id, v);

    unsigned r = id / 4;
    unsigned c = id % 4;

    unsigned bidx = (r * 4) + c;
    if (bidx < buttons_.size()) {
        auto &btn = buttons_[bidx];
        if (v)
            btn->onDown();
        else
            btn->onUp();
    }
}


void MiniParamView::eventButton(unsigned int id, bool longPress) {
    base_type::eventButton(id, longPress);
    if (longPress) return;

    unsigned r = id / 4;
    unsigned c = id % 4;

    unsigned bidx = (r * 4) + c;
    if (bidx < buttons_.size()) {
        auto &btn = buttons_[bidx];
        btn->onClick();
    }
}


void MiniParamView::eventUp(bool longPress) {
    base_type::eventUp(longPress);
    if (longPress) return;
    prevPage();
}

void MiniParamView::eventDown(bool longPress) {
    base_type::eventDown(longPress);
    if (longPress) return;
    nextPage();
}


void MiniParamView::prevPage() {
    unsigned nPages = (params_.size() / nParamPerPage);
    nPages += (params_.size() % 4) > 0;
    if ((int(paramOffset_) - int(nParamPerPage)) >= 0) {
        paramOffset_ -= nParamPerPage;
    } else {
        return;
    }
    unsigned pidx = 0;
    unsigned pStart = paramOffset_;
    unsigned pEnd = paramOffset_ + nParamPerPage;
    for (auto p : params_) {
        bool act = pidx >= pStart && pidx < pEnd;
        p->active(act);
        p->setVisible(act);
        pidx++;
    }
}


void MiniParamView::nextPage() {
    unsigned nPages = (params_.size() / nParamPerPage);
    nPages += (params_.size() % 4) > 0;
    if ((paramOffset_ + nParamPerPage) < ((nPages * nParamPerPage))) {
        paramOffset_ += nParamPerPage;
    } else {
        return;
        // // cycle around
        // paramOffset_ = 0;
    }
    unsigned pidx = 0;
    unsigned pStart = paramOffset_;
    unsigned pEnd = paramOffset_ + nParamPerPage;
    for (auto p : params_) {
        bool act = pidx >= pStart && pidx < pEnd;
        p->active(act);
        p->setVisible(act);
        pidx++;
    }
}

void MiniParamView::resized() {
    base_type::resized();
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    ioW_ = ioCallback_ ? 60 * COMPACT_UI_SCALE : gap;

    // size buttons
    unsigned butTopY = SSP_COMPACT_HEIGHT - buttonBarH;
    unsigned butLeftX = gap;
    unsigned bw = (canvasW) / 4;
    unsigned bidx = 0;
    for (auto p : buttons_) {
        if (bidx < maxUserBtns) {
            unsigned r = bidx / 4;
            unsigned c = bidx % 4;
            p->setBounds(butLeftX + (c * bw), butTopY + r * (buttonBarH / 2), bw, buttonBarH / 2);
        }
        bidx++;
    }

    // size parameters
    unsigned paramW = canvasW - ioW_ - gap;
    unsigned paramH = canvasH - gap;
    unsigned paramX = gap + ioW_;
    unsigned paramY = titleH;
    unsigned pidx = 0;
    for (auto p : params_) {
        float pos = (pidx % nParamPerPage);
        bool act = pidx >= paramOffset_ && pidx < paramOffset_ + nParamPerPage;
        unsigned w = paramW - 1;
        unsigned h = paramH / 4;
        p->setBounds(paramX, (pos * h) + paramY, w, h);
        p->setVisible(act);
        pidx++;
    }
}

void MiniParamView::drawIO(Graphics &g) {
    if (!ioCallback_) return;

    int box = COMPACT_UI_SCALE * 7;
    unsigned tx = gap + (box * 2);
    unsigned y = titleH + (gap * 6);

    static constexpr unsigned fh = 9 * COMPACT_UI_SCALE;
    static constexpr unsigned h = fh + 2 * COMPACT_UI_SCALE;
    static constexpr float alphaMult = 0.5f;
    static constexpr float alphaOffset = (1.0f - alphaMult) / 4.0f;


    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(Colours::yellow);
    g.drawSingleLineText("Inputs", tx, y);
    y += h;

    g.setColour(Colours::white);
    int i;
    for (i = 0; i < 8 && i < baseProcessor_->getBusCount(true); i++) {
        auto bus = baseProcessor_->getBus(true, i);
        if (bus) {
            g.setColour(Colours::white);
            g.drawSingleLineText(bus->getName(), tx, y);
            float v = ioCallback_(true, i);
            Colour clr = v > 0.001f ? Colours::green : (v < -0.001f ? Colours::red : Colours::black);
            float aMod = ((v > 0.0f ? v : -v) * alphaMult) + alphaOffset;
            clr = clr.withMultipliedAlpha(aMod);
            g.setColour(clr);
            g.fillRect(gap, y - box, box, box);
        }
        y += h;
    }

    y += (8 - i) * h;

    y += h;
    g.setColour(Colours::yellow);
    g.drawSingleLineText("Outputs", tx, y);
    y += h;

    for (i = 0; i < 2 && i < baseProcessor_->getBusCount(false); i++) {
        auto bus = baseProcessor_->getBus(false, i);
        if (bus) {
            g.setColour(Colours::white);
            g.drawSingleLineText(bus->getName(), tx, y);
            float v = ioCallback_(false, i);
            Colour clr = v > 0.01f ? Colours::green : (v < -0.01f ? Colours::red : Colours::black);
            float aMod = ((v > 0.0f ? v : -v) * alphaMult) + alphaOffset;
            clr = clr.withMultipliedAlpha(aMod);
            g.setColour(clr);
            g.fillRect(gap, y - box, box, box);
        }
        y += h;
    }
}


void MiniParamView::drawButtonBox(Graphics &g) {
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    unsigned butTopY = SSP_COMPACT_HEIGHT - buttonBarH;
    unsigned butLeftX = gap;
    unsigned bw = canvasW / 4;

    g.setColour(Colours::grey);
    g.drawHorizontalLine(butTopY, butLeftX, gap + canvasW - 1);
    g.drawHorizontalLine(butTopY + (buttonBarH) / 2, butLeftX, gap + canvasW - 1);
    g.drawHorizontalLine((butTopY + buttonBarH - 1), butLeftX, gap + canvasW - 1);
    for (int i = 0; i < 5; i++) { g.drawVerticalLine(butLeftX + (i * bw) - 1, butTopY, butTopY + buttonBarH - 1); }
}


///////MiniBasicView


MiniBasicView::MiniBasicView(BaseProcessor *p, ioActivity cb) : base_type(p, true), ioCallback_(cb) {
    buttonBox_ = std::make_shared<ssp::ButtonBox>();
    addAndMakeVisible(buttonBox_.get());
}

void MiniBasicView::addButton(unsigned idx, const std::shared_ptr<ValueButton> &p) {
    buttonBox_->addButton(idx, p);
}

void MiniBasicView::onButton(unsigned int id, bool v) {
    base_type::onButton(id, v);
    buttonBox_->onButton(id, v);
}

std::shared_ptr<ValueButton> MiniBasicView::getButton(unsigned idx) {
    return buttonBox_->getButton(idx);
}


void MiniBasicView::resized() {
    base_type::resized();

    buttonBox_->setBounds(0, canvasH, canvasW, buttonBarH);
}


/// LineMiniEditor

LineMiniEditor::LineMiniEditor(BaseProcessor *p) : base_type(p, true) {
    ;
}

void LineMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 10 * COMPACT_UI_SCALE, Font::plain));

    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + ":" + String(JucePlugin_Desc) + String(" @ thetechnobear"), gap,
                         gap * 2);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString), 270 * COMPACT_UI_SCALE, gap * 2);

    drawButtonBox(g);
}

void LineMiniEditor::resized() {
    base_type::resized();
    int bidx = 0;
    for (auto btn : buttons_) {
        if (bidx < maxUserBtns) {
            unsigned r = bidx / 4;
            unsigned c = bidx % 4;
            unsigned bw = canvasW / 4;
            unsigned bh = buttonBarH / 2;
            unsigned x = gap + (c * bw);
            unsigned y = SSP_COMPACT_HEIGHT - buttonBarH + (r * bh);
            unsigned w = bw;
            unsigned h = bh;
            if (btn) btn->setBounds(x, y, w, h);
        }
        bidx++;
    }
}

void LineMiniEditor::drawButtonBox(Graphics &g) {
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    unsigned butTopY = SSP_COMPACT_HEIGHT - buttonBarH;
    unsigned butLeftX = gap;
    unsigned bw = canvasW / 4;

    g.setColour(Colours::grey);
    g.drawHorizontalLine(butTopY, butLeftX, gap + canvasW - 1);
    g.drawHorizontalLine(butTopY + (buttonBarH) / 2, butLeftX, gap + canvasW - 1);
    g.drawHorizontalLine((butTopY + buttonBarH - 1), butLeftX, gap + canvasW - 1);
    for (int i = 0; i < 5; i++) { g.drawVerticalLine(butLeftX + (i * bw) - 1, butTopY, butTopY + buttonBarH - 1); }
}


void LineMiniEditor::setParamBounds(unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

    unsigned sp = 2 * COMPACT_UI_SCALE;
    unsigned h = paramHeight;
    unsigned w = ((canvasW - gap * 2) / nParamPerPage);

    unsigned x = gap + (idx * w);
    unsigned y = SSP_COMPACT_HEIGHT - buttonBarH - paramHeight;
    c->setBounds(x + sp, y, w - (2 * sp), h);
}

void LineMiniEditor::addParamPage(std::shared_ptr<BaseParamControl> c1, std::shared_ptr<BaseParamControl> c2,
                                  std::shared_ptr<BaseParamControl> c3, std::shared_ptr<BaseParamControl> c4) {
    auto page = std::make_shared<ControlPage>();
    page->control_[0] = c1;
    page->control_[1] = c2;
    page->control_[2] = c3;
    page->control_[3] = c4;
    controlPages_.push_back(page);

    setParamBounds(0, c1);
    setParamBounds(1, c2);
    setParamBounds(2, c3);
    setParamBounds(3, c4);

    if (paramPage_ == controlPages_.size() - 1) {
        for (auto i = 0; i < 4; i++) {
            auto c = page->control_[i];
            if (c) {
                addAndMakeVisible(c.get());
                c->active(true);
            }
        }
    } else {
        for (auto i = 0; i < 4; i++) {
            auto c = page->control_[i];
            if (c) {
                addChildComponent(c.get());
                c->active(false);
            }
        }
    }
}

void LineMiniEditor::addButton(const std::shared_ptr<ParamButton> &p) {
    buttons_.push_back(p);
    if (p) addAndMakeVisible(p.get());
}

void LineMiniEditor::chgParamPage(int inc, bool changeVis) {
    if (controlPages_.empty()) return;

    int newPage = paramPage_ + inc;
    if (newPage < 0) newPage = controlPages_.size() - 1;
    if (newPage >= controlPages_.size()) newPage = 0;

    if (newPage != paramPage_ && changeVis) {
        auto &opage = controlPages_[paramPage_];
        for (auto i = 0; i < nParamPerPage; i++) {
            auto c = opage->control_[i];
            if (c) {
                c->active(false);
                c->setVisible(false);
            }
        }
        auto npage = controlPages_[newPage];
        for (auto i = 0; i < nParamPerPage; i++) {
            auto c = npage->control_[i];
            if (c) {
                c->active(true);
                c->setVisible(true);
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

    if (bidx < buttons_.size()) {
        auto &btn = buttons_[bidx];
        if (!btn) return;
        if (v)
            btn->onDown();
        else
            btn->onUp();
    }
}


void LineMiniEditor::eventButton(unsigned int bidx, bool longPress) {
    base_type::eventButton(bidx, longPress);
    if (longPress) return;

    if (bidx < buttons_.size()) {
        auto &btn = buttons_[bidx];
        if (!btn) return;
        btn->onClick();
    }
}


}  // namespace ssp