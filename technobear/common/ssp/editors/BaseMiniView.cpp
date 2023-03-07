#include "BaseMiniView.h"

#include "ssp/BaseProcessor.h"

namespace ssp {

BaseMiniView::BaseMiniView(BaseProcessor *p) : base_type(p, true) {

}


MiniParamView::MiniParamView(BaseProcessor *p, ioActivity cb)
    : base_type(p, true),
      ioCallback_(cb),
      pageBtn_("Page", nullptr, 12 * scale, Colours::red) {
    addAndMakeVisible(pageBtn_);
}

void MiniParamView::drawView(Graphics &g) {
    static constexpr unsigned gap = 5 * scale;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 10 * scale, Font::plain));

    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + ":"
                         + String(JucePlugin_Desc)
                         + String(" @ thetechnobear"),
                         gap,
                         gap * 2);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString),
                         270 * scale,
                         gap * 2);

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
            if (encoderState_[enc]) {
                encoderFine_[enc] = true;
            }
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
    if (v) return; // change on button up

    if (!encoderFine_[enc]) {
        if ((enc + paramOffset_) < params_.size()) {
            auto c = params_[enc + paramOffset_];
            if (c != nullptr) {
                if (v == 0) {
                    c->reset();
                }
            }
        }
    }
    encoderFine_[enc] = false;
}

void MiniParamView::onButton(unsigned int id, bool v) {
    base_type::onButton(id, v);

    if (id == nextPageBtnId) {
        pageBtn_.value(v);
    } else {
        unsigned r = id / 4;
        unsigned c = id % 4;

        unsigned bidx = (r * 3) + c;
        if (bidx < buttons_.size()) {
            auto &btn = buttons_[bidx];
            if (v) btn->onDown();
            else btn->onUp();
        }
    }

    if (v) return;

    if (id == nextPageBtnId) {
        nextPage();
    }
}

void MiniParamView::nextPage() {
    unsigned nPages = (params_.size() / nParamPerPage);
    nPages += (params_.size() % 4) > 0;
    if ((paramOffset_ + nParamPerPage) < ((nPages * nParamPerPage) )) {
        paramOffset_ += nParamPerPage;
    } else {
        // cycle around
        paramOffset_ = 0;
    }
    unsigned pidx = 0;
    unsigned pStart = paramOffset_;
    unsigned pEnd = paramOffset_ + nParamPerPage;
    for (auto p: params_) {
        bool act = pidx >= pStart && pidx < pEnd;
        p->active(act);
        p->setVisible(act);
        pidx++;
    }
}

void MiniParamView::resized() {
    base_type::resized();
    static constexpr unsigned gap = 5 * scale;
    canvasH_ = getHeight() - titleH_;
    canvasW_ = getWidth() - (gap * 2);
    buttonBarH_ = canvasH_ / (nParamPerPage + 3);
    ioW_ = 60 * scale;

    // size buttons
    unsigned butTopY = getHeight() - buttonBarH_;
    unsigned butLeftX = gap;
    unsigned bw = (canvasW_) / 4;
    unsigned bidx = 0;
    for (auto p: buttons_) {
        if (bidx < maxUserBtns) {
            unsigned r = bidx / 3;
            unsigned c = bidx % 3;
            p->setBounds(butLeftX + (c * bw), butTopY + r * (buttonBarH_ / 2),
                         bw, buttonBarH_ / 2);
        }
        bidx++;
    }
    unsigned r = 1;
    unsigned c = 3;
    pageBtn_.setBounds(butLeftX + (c * bw), butTopY + r * (buttonBarH_ / 2), bw, buttonBarH_ / 2);

    // size parameters
    unsigned paramW = canvasW_ - ioW_ - gap;
    unsigned paramH = (canvasH_ - buttonBarH_) - gap;
    unsigned paramX = gap + ioW_;
    unsigned paramY = titleH_;
    unsigned pidx = 0;
    for (auto p: params_) {
        float pos = (pidx % nParamPerPage);
        bool act = pidx >= paramOffset_ && pidx < paramOffset_ + nParamPerPage;
        unsigned w = paramW - 1;
        unsigned h = paramH / 4;
        p->setBounds(paramX, (pos * h) + paramY, w, h);
        p->setVisible(act);
        pidx++;
    }
}

void MiniParamView::drawButtonBox(Graphics &g) {
    static constexpr unsigned gap = 5 * scale;
    unsigned butTopY = getHeight() - buttonBarH_;
    unsigned butLeftX = scale * 5;
    unsigned bw = canvasW_ / 4;

    g.setColour(Colours::grey);
    g.drawHorizontalLine(butTopY, butLeftX, gap + canvasW_ - 1);
    g.drawHorizontalLine(butTopY + (buttonBarH_) / 2, butLeftX, gap + canvasW_ - 1);
    g.drawHorizontalLine((butTopY + buttonBarH_ - 1), butLeftX, gap + canvasW_ - 1);
    for (int i = 0; i < 5; i++) {
        g.drawVerticalLine(butLeftX + (i * bw) - 1, butTopY, butTopY + buttonBarH_ - 1);
    }
}


void MiniParamView::drawIO(Graphics &g) {
    static constexpr unsigned gap = 5 * scale;
    int box = scale * 7;
    unsigned tx = gap + (box * 2);
    unsigned y = titleH_ + (gap * 6);

    static constexpr unsigned fh = 9 * scale;
    static constexpr unsigned h = fh + 2 * scale;
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
            if (ioCallback_) {
                float v = ioCallback_(true, i);
                Colour clr = v > 0.001f ? Colours::green : (v < -0.001f ? Colours::red : Colours::black);
                float aMod = ((v > 0.0f ? v : -v) * alphaMult) + alphaOffset;
                clr = clr.withMultipliedAlpha(aMod);
                g.setColour(clr);
                g.fillRect(gap, y - box, box, box);
            }
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
            if (ioCallback_) {
                float v = ioCallback_(false, i);
                Colour clr = v > 0.01f ? Colours::green : (v < -0.01f ? Colours::red : Colours::black);
                float aMod = ((v > 0.0f ? v : -v) * alphaMult) + alphaOffset;
                clr = clr.withMultipliedAlpha(aMod);
                g.setColour(clr);
                g.fillRect(gap, y - box, box, box);
            }
        }
        y += h;
    }

}


} // namespace