
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
      clearBtn_("Clear", nullptr, 32, Colours::white, Colours::black) {
    addAndMakeVisible(scanBtn_);
    addAndMakeVisible(clearBtn_);
    addAndMakeVisible(loadBtn_);
    addAndMakeVisible(moduleList_);
}


void LoadView::drawView(Graphics &g) {
    drawButtonBox(g);

    unsigned x = 50;
    unsigned y = 24;
    static constexpr unsigned fh = 24;
    static constexpr unsigned h = fh + 5;
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.drawSingleLineText("Load Module", x, y);
}

void LoadView::resized() {
    ssp::BaseView::resized();
    setButtonBounds(scanBtn_, 0, 0);
    setButtonBounds(clearBtn_, 1, 3);
    setButtonBounds(loadBtn_, 0, 3);

    unsigned x = 50;
    unsigned y = 24;
    moduleList_.setBounds(x, y + 25, SSP_COMPACT_WIDTH, SSP_COMPACT_HEIGHT);
}

void LoadView::drawButtonBox(Graphics &g) {
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

void LoadView::setButtonBounds(ValueButton &btn, unsigned r, unsigned c) {
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


void LoadView::loadModule() {
    if (trackIdx_ >= PluginProcessor::MAX_TRACKS || moduleIdx_ <= Track::M_IN || moduleIdx_ >= Track::M_OUT) return;
    if (moduleList_.idx() < 0) return;

    auto curMod = processor_.getLoadedPlugin(trackIdx_, moduleIdx_);
    auto &modules = processor_.getSupportedModules();

    auto &newMod = modules[moduleList_.idx()];
    if (newMod != curMod) {
        // time to load a new module !
        bool r = false;
        while (!r) { r = processor_.requestModuleChange(trackIdx_, moduleIdx_, newMod); }
        moduleUpdated_ = true;
    }
}

void LoadView::eventUp(bool v) {
    if (v) return;
    moduleList_.prevItem();
}

void LoadView::eventDown(bool v) {
    if (v) return;
    moduleList_.nextItem();
}

void LoadView::onEncoder(unsigned enc, float v) {
    if (v > 0)
        moduleList_.nextItem();
    else
        moduleList_.prevItem();
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
            while (!r) { r = processor_.requestModuleChange(trackIdx_, moduleIdx_, ""); }
            break;
        }
    }
}

void LoadView::editorShown() {
    moduleUpdated_ = false;
    moduleList_.clear();
    if (moduleIdx_ < Track::M_MAX) {
        auto curMod = processor_.getLoadedPlugin(trackIdx_, moduleIdx_);
        auto &modules = processor_.getSupportedModules();
        int modIdx = 0;
        for (auto &m : modules) {
            moduleList_.addItem(nicePlugName(m));
            if (m == curMod) moduleList_.idx(modIdx);
            modIdx++;
        }
    }
}


void LoadView::moduleIdx(unsigned trackIdx, unsigned mIdx) {
    if (trackIdx < PluginProcessor::MAX_TRACKS && mIdx < Track::M_MAX) {
        trackIdx_ = trackIdx;
        moduleIdx_ = mIdx;
    }
}
