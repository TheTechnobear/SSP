
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


LoadView::LoadView(PluginProcessor &p)
    : ssp::BaseView(&p, false),
      processor_(p),
      scanBtn_("Scan", nullptr, 32, Colours::white, Colours::black),
      loadBtn_("Load", nullptr, 32, Colours::white, Colours::black),
      clearBtn_("Clear", nullptr, 32, Colours::white, Colours::black),
      upBtn_("EN-", nullptr, 32, Colours::black, Colours::red),
      downBtn_("EN+", nullptr, 32, Colours::black, Colours::red),
      leftBtn_("", nullptr, 32, Colours::black, Colours::red),
      rightBtn_("", nullptr, 32, Colours::black, Colours::red) {
    setButtonBounds(scanBtn_, 0, 0);
    setButtonBounds(clearBtn_, 1, 3);
    setButtonBounds(upBtn_, 0, 5);
    setButtonBounds(loadBtn_, 0, 3);
    setButtonBounds(leftBtn_, 1, 4);
    setButtonBounds(downBtn_, 1, 5);
    setButtonBounds(rightBtn_, 1, 6);

    addAndMakeVisible(scanBtn_);
    addAndMakeVisible(clearBtn_);
    addAndMakeVisible(loadBtn_);
    addAndMakeVisible(upBtn_);
    addAndMakeVisible(downBtn_);
    addAndMakeVisible(leftBtn_);
    addAndMakeVisible(rightBtn_);
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


void LoadView::loadModule() {
    if(moduleIdx_>= PluginProcessor::M_MAX) return;

    auto &curMod = processor_.getLoadedPlugin(moduleIdx_);
    auto &modules = processor_.getSupportedModules();
    auto &newMod = modules[curModNameIdx_];
    if (newMod != curMod) {
        // time to load a new module !
        bool r = false;
        while (!r) { r = processor_.requestModuleChange(moduleIdx_, newMod); }
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
            if(moduleIdx_>= PluginProcessor::M_MAX) return;
            bool r = false;
            while (!r) { r = processor_.requestModuleChange(moduleIdx_, ""); }
            break;
        }
    }
}

void LoadView::editorShown() {
    if (moduleIdx_ < PluginProcessor::M_MAX) {
        auto curMod = processor_.getLoadedPlugin(moduleIdx_);
        auto &modules = processor_.getSupportedModules();
        int i = 0;
        for (auto &m : modules) {
            if (m == curMod) {
                curModNameIdx_ = i;
                break;
            }
            i++;
        }
    }
}

void LoadView::moduleIdx(int midx) {
    if (midx < PluginProcessor::M_MAX) { moduleIdx_ = midx; }
}
