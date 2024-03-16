
#include "LoadView.h"

std::string nicePlugName(const std::string &n) {
    std::string nn = n.substr(n.find_last_of('/') + 1);
#if __APPLE__
#else
    // linux, remove .so
    nn = nn.substr(0, nn.find_last_of('.'));
#endif
    return nn;
}


LoadView::LoadView(PluginProcessor &p, bool compactUI)
    : ssp::BaseView(&p, compactUI),
      processor_(p),
      scanBtn_("Scan", nullptr, 32, Colours::white, Colours::black),
      loadBtn_("Load", nullptr, 32, Colours::white, Colours::black),
      clearBtn_("Clear", nullptr, 32, Colours::white, Colours::black),
      upBtn_("EN-", nullptr, 32, Colours::black, Colours::red),
      downBtn_("EN+", nullptr, 32, Colours::black, Colours::red),
      leftBtn_("", nullptr, 32, Colours::black, Colours::red),
      rightBtn_("", nullptr, 32, Colours::black, Colours::red) {
    addAndMakeVisible(scanBtn_);
    addAndMakeVisible(clearBtn_);
    addAndMakeVisible(loadBtn_);
    if (!compactUI_) {
        addAndMakeVisible(upBtn_);
        addAndMakeVisible(downBtn_);
        addAndMakeVisible(leftBtn_);
        addAndMakeVisible(rightBtn_);
    }
}


void LoadView::drawView(Graphics &g) {
    drawButtonBox(g);

    unsigned x = 50;
    unsigned y = 24;
    static constexpr unsigned fh = 24;
    static constexpr unsigned h = fh + 5;
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    auto &modules = processor_.getSupportedModules();

    g.setColour(Colours::yellow);
    g.drawSingleLineText("Load Module", x, y);
    y += h;
    int i = 0;
    for (auto &m : modules) {
        if (i == curModNameIdx_) {
            g.setColour(Colours::red);
        } else {
            g.setColour(Colours::white);
        }
        g.drawSingleLineText(nicePlugName(m), x, y);
        y += h;
        i++;
    }
}

void LoadView::resized() {
    ssp::BaseView::resized();
    setButtonBounds(scanBtn_, 0, 0);
    setButtonBounds(clearBtn_, 1, 3);
    setButtonBounds(loadBtn_, 0, 3);
    if (!compactUI_) {
        setButtonBounds(upBtn_, 0, 5);
        setButtonBounds(leftBtn_, 1, 4);
        setButtonBounds(downBtn_, 1, 5);
        setButtonBounds(rightBtn_, 1, 6);
    }
}

void LoadView::drawButtonBox(Graphics &g) {
    if (!compactUI_) {
        unsigned butTopY = btnTopY;
        unsigned butLeftX = 900 - 1;
        float x = butLeftX;
        float y = butTopY;
        g.setColour(Colours::grey);
        g.drawHorizontalLine(y, x, 1600 - 1);
        g.drawHorizontalLine(y + btnSpaceY, x, 1600 - 1);
        g.drawHorizontalLine(480 - 1, x, 1600 - 1);
        for (int i = 0; i < 8; i++) { g.drawVerticalLine(x + i * 100, butTopY, 480 - 1); }
    } else {
        static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
        static constexpr unsigned nParamPerPage = 4;
        static constexpr unsigned titleH = 30;
        unsigned canvasH = SSP_COMPACT_HEIGHT - titleH;
        unsigned canvasW = SSP_COMPACT_WIDTH - (gap * 2);
        unsigned buttonBarH = canvasH / (nParamPerPage + 3);
        unsigned butTopY = SSP_COMPACT_HEIGHT - buttonBarH;

        unsigned butLeftX = COMPACT_UI_SCALE * 5;
        unsigned bw = canvasW / 4;

        g.setColour(Colours::grey);
        g.drawHorizontalLine(butTopY, butLeftX, gap + canvasW - 1);
        g.drawHorizontalLine(butTopY + (buttonBarH) / 2, butLeftX, gap + canvasW - 1);
        g.drawHorizontalLine((butTopY + buttonBarH - 1), butLeftX, gap + canvasW - 1);
        for (int i = 0; i < 5; i++) { g.drawVerticalLine(butLeftX + (i * bw) - 1, butTopY, butTopY + buttonBarH - 1); }
    }
}

void LoadView::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
    if (!compactUI_) {
        const int w = 100;
        const int h = btnSpaceY;
        unsigned x = 900 + (c * w);
        unsigned y = btnTopY + (r * h);
        btn.setBounds(x, y, w, h);
    } else {
        static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
        static constexpr unsigned nParamPerPage = 4;
        static constexpr unsigned titleH = 30;
        unsigned canvasH = SSP_COMPACT_HEIGHT - titleH;
        unsigned canvasW = SSP_COMPACT_WIDTH - (gap * 2);
        unsigned buttonBarH = canvasH / (nParamPerPage + 3);
        unsigned butTopY = SSP_COMPACT_HEIGHT - buttonBarH;

        unsigned butLeftX = gap;
        unsigned bw = (canvasW) / 4;
        btn.setBounds(butLeftX + (c * bw), butTopY + r * (buttonBarH / 2), bw, buttonBarH / 2);
    }
}


void LoadView::loadModule() {
    if (trackIdx_ >= PluginProcessor::MAX_TRACKS || moduleIdx_ >= Track::M_MAX) return;

    auto curMod = processor_.getLoadedPlugin(trackIdx_, moduleIdx_);
    auto &modules = processor_.getSupportedModules();
    auto &newMod = modules[curModNameIdx_];
    if (newMod != curMod) {
        // time to load a new module !
        bool r = false;
        while (!r) { r = processor_.requestModuleChange(trackIdx_, moduleIdx_, newMod); }
        moduleUpdated_ = true;
    }
}

void LoadView::eventUp(bool v) {
    if (v) return;
    if (curModNameIdx_ > 0) curModNameIdx_--;
}

void LoadView::eventDown(bool v) {
    if (v) return;
    int maxSz = processor_.getSupportedModules().size();
    if (curModNameIdx_ < 0)
        curModNameIdx_ = 0;
    else if (curModNameIdx_ < (maxSz - 1))
        curModNameIdx_++;
}

void LoadView::onEncoder(unsigned enc, float v) {
    if (v > 0.0f) {
        int maxSz = processor_.getSupportedModules().size();
        if (curModNameIdx_ < 0)
            curModNameIdx_ = 0;
        else if (curModNameIdx_ < (maxSz - 1))
            curModNameIdx_++;
    } else if (v < 0.0f) {
        if (curModNameIdx_ > 0) curModNameIdx_--;
    }
}

void LoadView::onEncoderSwitch(unsigned enc, bool v) {
    if (v) return;
    loadModule();
}

void LoadView::eventButton(unsigned int btn, bool v) {
    if (v) return;
    switch (btn) {
        case 0: {
            processor_.scanPlugins();
            editorShown();  // reset current plugin
            break;
        }
        case 3: {
            loadModule();
            break;
        }
        case 7: {
            if (trackIdx_ >= PluginProcessor::MAX_TRACKS || moduleIdx_ >= Track::M_MAX) return;
            bool r = false;
            curModNameIdx_ = -1;
            while (!r) { r = processor_.requestModuleChange(trackIdx_, moduleIdx_, ""); }
            break;
        }
    }
}

void LoadView::editorShown() {
    moduleUpdated_ = false;
    if (moduleIdx_ < PluginProcessor::M_MAX) {
        auto curMod = processor_.getLoadedPlugin(moduleIdx_);
        auto &modules = processor_.getSupportedModules();
        int i = 0;
        for (auto &m : modules) {
            if (m == curMod) {
                curModNameIdx_ = i;
                return;
            }
            i++;
        }
    }
    curModNameIdx_ = -1;
}


void LoadView::moduleIdx(int midx) {
    if (midx < PluginProcessor::M_MAX) { moduleIdx_ = midx; }
}
