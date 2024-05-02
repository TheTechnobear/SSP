#include "BaseMiniView.h"

#include "ssp/BaseProcessor.h"

namespace ssp {


MiniParamView::MiniParamView(BaseProcessor *p, ioActivity cb) : base_type(p, true), ioCallback_(cb) {
}

void MiniParamView::drawView(Graphics &g) {
    g.setFont(Font(Font::getDefaultMonospacedFontName(), titleFH, Font::plain));

    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + ":" + String(JucePlugin_Desc) + String(" @ thetechnobear"), gap,
                         gap * 2);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString), 270 * COMPACT_UI_SCALE, gap * 2);

    drawButtonBox(g);
    // drawIO(g);
}


void MiniParamView::addParam(const std::shared_ptr<BaseParamControl> &p, const juce::Colour &fg) {
    p->fg(fg);
    params_.push_back(p);
    addChildComponent(p.get());

    bool act = params_.size() <= (paramOffset_ + nParamsPerPage);
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

void MiniParamView::onButton(unsigned int bidx, bool v) {
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


void MiniParamView::eventButton(unsigned int bidx, bool longPress) {
    base_type::eventButton(bidx, longPress);
    if (longPress) return;

    if (bidx < buttons_.size()) {
        auto &btn = buttons_[bidx];
        if (btn) btn->onClick();
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
    unsigned nPages = (params_.size() / nParamsPerPage);
    nPages += (params_.size() % 4) > 0;
    if ((int(paramOffset_) - int(nParamsPerPage)) >= 0) {
        paramOffset_ -= nParamsPerPage;
    } else {
        return;
    }
    unsigned pidx = 0;
    unsigned pStart = paramOffset_;
    unsigned pEnd = paramOffset_ + nParamsPerPage;
    for (auto p : params_) {
        bool act = pidx >= pStart && pidx < pEnd;
        p->active(act);
        p->setVisible(act);
        pidx++;
    }
}


void MiniParamView::nextPage() {
    unsigned nPages = (params_.size() / nParamsPerPage);
    nPages += (params_.size() % 4) > 0;
    if ((paramOffset_ + nParamsPerPage) < ((nPages * nParamsPerPage))) {
        paramOffset_ += nParamsPerPage;
    } else {
        return;
        // // cycle around
        // paramOffset_ = 0;
    }
    unsigned pidx = 0;
    unsigned pStart = paramOffset_;
    unsigned pEnd = paramOffset_ + nParamsPerPage;
    for (auto p : params_) {
        bool act = pidx >= pStart && pidx < pEnd;
        p->active(act);
        p->setVisible(act);
        pidx++;
    }
}

void MiniParamView::setButtonBounds(unsigned bidx, juce::Component *btn) {
    if (!btn) return;

    int offset = gap + (gap / 2);
    unsigned r = bidx / 4;
    unsigned c = bidx % 4;
    int w = paramWidth;
    int h = buttonBarH / 2;
    int x = offset + (c * gridW);
    int y = SSP_COMPACT_HEIGHT - buttonBarH + (r * h);
    btn->setBounds(x, y, w, h);
}

void MiniParamView::resized() {
    base_type::resized();
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    // ioW_ = ioCallback_ ? 60 * COMPACT_UI_SCALE : 0;
    ioW_ = 0;

    // size buttons
    unsigned bidx = 0;
    for (auto p : buttons_) {
        if (bidx < maxUserBtns) {
            if (p) setButtonBounds(bidx, p.get());
        }
        bidx++;
    }

    // size parameters
    unsigned paramW = canvasW - ioW_ - (2 * gap);
    unsigned paramH = (canvasH - (5 * gap)) / nParamsPerPage;
    unsigned paramX = gap + ioW_;
    unsigned paramY = titleH;
    unsigned pidx = 0;
    for (auto p : params_) {
        float pos = (pidx % nParamsPerPage);
        bool act = pidx >= paramOffset_ && pidx < paramOffset_ + nParamsPerPage;
        unsigned w = paramW;
        unsigned h = paramH;
        p->setBounds(paramX, (pos * h) + paramY, paramW, paramH);
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


} // namespace