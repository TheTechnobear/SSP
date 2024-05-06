
#include "LoadModuleView.h"

std::string nicePlugName(const std::string &n) {
    std::string nn = n.substr(n.find_last_of('/') + 1);
#if __APPLE__
#else
    // linux, remove .so
    nn = nn.substr(0, nn.find_last_of('.'));
#endif
    return nn;
}


LoadModuleView::LoadModuleView(PluginProcessor &p, bool compactUI)
    : base_type(&p, compactUI),
      processor_(p),
      scanBtn_("Scan", nullptr, 16 * COMPACT_UI_SCALE, Colours::white, Colours::black),
      loadBtn_("Load", nullptr, 16 * COMPACT_UI_SCALE, Colours::white, Colours::black),
      clearBtn_("Clear", nullptr, 16 * COMPACT_UI_SCALE, Colours::white, Colours::black) {
    addAndMakeVisible(scanBtn_);
    addAndMakeVisible(clearBtn_);
    addAndMakeVisible(loadBtn_);
    addAndMakeVisible(categoryList_);
    addAndMakeVisible(moduleList_);
    if (processor_.getSupportedModules().size() == 0) { processor_.loadSupportedModules(); }
}


void LoadModuleView::drawView(Graphics &g) {
    static constexpr unsigned fh = 14 * COMPACT_UI_SCALE;
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned x = gap;
    static constexpr unsigned y = gap + SSP_COMPACT_HEIGHT - (80 * COMPACT_UI_SCALE) + gap;
    g.setColour(Colours::white);
    g.setFont(fh);

    g.drawText(pluginDescripton_, x, y, SSP_COMPACT_WIDTH - gap * 2, fh, Justification::topLeft, true);
    drawButtonBox(g);
}

void LoadModuleView::resized() {
    ssp::BaseView::resized();
    setButtonBounds(loadBtn_, 0, 0);
    setButtonBounds(clearBtn_, 1, 0);
    setButtonBounds(scanBtn_, 0, 3);

    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned x = gap;
    static constexpr unsigned y = gap;
    static constexpr unsigned w = (SSP_COMPACT_WIDTH - 3 * gap) / 2;
    categoryList_.setBounds(x, y, w, SSP_COMPACT_HEIGHT - (80 * COMPACT_UI_SCALE));
    moduleList_.setBounds(x + gap + w, y, w, SSP_COMPACT_HEIGHT - (80 * COMPACT_UI_SCALE));
}

void LoadModuleView::drawButtonBox(Graphics &g) {
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned nParamPerPage = 4;
    static constexpr unsigned titleH = 12 * COMPACT_UI_SCALE;
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

void LoadModuleView::setButtonBounds(ssp::ValueButton &btn, unsigned r, unsigned c) {
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned nParamPerPage = 4;
    static constexpr unsigned titleH = 12 * COMPACT_UI_SCALE;
    unsigned canvasH = SSP_COMPACT_HEIGHT - titleH;
    unsigned canvasW = SSP_COMPACT_WIDTH - (gap * 2);
    unsigned buttonBarH = canvasH / (nParamPerPage + 3);
    unsigned butTopY = SSP_COMPACT_HEIGHT - buttonBarH;

    unsigned butLeftX = gap;
    unsigned bw = (canvasW) / 4;
    btn.setBounds(butLeftX + (c * bw), butTopY + r * (buttonBarH / 2), bw, buttonBarH / 2);
}


void LoadModuleView::loadModule() {
    if (trackIdx_ >= PluginProcessor::MAX_TRACKS || moduleIdx_ <= Track::M_IN || moduleIdx_ >= Track::M_OUT) return;
    if (moduleList_.idx() < 0) return;

    auto curMod = processor_.getLoadedPlugin(trackIdx_, moduleIdx_);
    auto &modules = processor_.getSupportedModules();

    int modId = modulesId_[moduleList_.idx()];

    auto &newMod = modules[modId];

    if (newMod.name != curMod) {
        // time to load a new module !
        bool r = false;
        while (!r) { r = processor_.requestModuleChange(trackIdx_, moduleIdx_, newMod.name); }
        moduleUpdated_ = true;
    }
}


void LoadModuleView::onEncoder(unsigned enc, float v) {
    switch (enc) {
        case 0: {
            if (v > 0)
                categoryList_.nextItem();
            else
                categoryList_.prevItem();
            updateModuleList();
            break;
        }
        case 1: {
            if (v > 0)
                moduleList_.nextItem();
            else
                moduleList_.prevItem();
            int modId = modulesId_[moduleList_.idx()];
            auto &modules = processor_.getSupportedModules();
            pluginDescripton_ = modules[modId].description;
            break;
        }
    }
}

void LoadModuleView::updateModuleList() {
    auto cat = categoryList_.idx();
    if (cat == curCatIdx_) return;

    moduleList_.clear();
    modulesId_.clear();

    auto &modules = processor_.getSupportedModules();
    int modIdx = 0;
    int modListIdx = 0, selListIdx = 0;
    for (auto &m : modules) {
        if (cat == 0 ||
            std::find(m.categories.begin(), m.categories.end(), categories_[cat - 1]) != m.categories.end()) {
            modulesId_.push_back(modIdx);
            std::string niceName = nicePlugName(m.name);
            moduleList_.addItem(niceName);
            if (m.name == processor_.getLoadedPlugin(trackIdx_, moduleIdx_)) selListIdx = modIdx;
            modListIdx++;
        }
        modIdx++;
    }
    moduleList_.idx(selListIdx);
    int modId = modulesId_[moduleList_.idx()];
    pluginDescripton_ = modules[modId].description;
}


void LoadModuleView::onEncoderSwitch(unsigned enc, bool v) {
    if (v) return;
    if (enc == 1) loadModule();
}

void LoadModuleView::eventButton(unsigned int btn, bool v) {
    if (v) return;
    switch (btn) {
        case 0: {
            loadModule();
            break;
        }
        case 3: {
            processor_.loadSupportedModules(true);
            editorShown();  // reset current plugin
            break;
        }
        case 4: {
            if (trackIdx_ >= PluginProcessor::MAX_TRACKS || moduleIdx_ >= Track::M_MAX) return;
            bool r = false;
            while (!r) { r = processor_.requestModuleChange(trackIdx_, moduleIdx_, ""); }
            break;
        }
    }
}

void LoadModuleView::editorShown() {
    categories_.clear();
    modulesId_.clear();
    moduleList_.clear();
    categoryList_.clear();
    moduleUpdated_ = false;

    categoryList_.addItem("All");
    auto &modules = processor_.getSupportedModules();
    for (auto &m : modules) {
        for (auto &cat : m.categories) {
            if (std::find(categories_.begin(), categories_.end(), cat) == categories_.end()) {
                categories_.push_back(cat);
                categoryList_.addItem(cat);
            }
        }
    }
    curCatIdx_ = 0xff;
    categoryList_.idx(0);
    updateModuleList();
}


void LoadModuleView::moduleIdx(unsigned trackIdx, unsigned mIdx) {
    if (trackIdx < PluginProcessor::MAX_TRACKS && mIdx < Track::M_MAX) {
        trackIdx_ = trackIdx;
        moduleIdx_ = mIdx;
    }
}
