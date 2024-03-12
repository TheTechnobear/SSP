
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
        if (i == curModIdx_) {
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
    auto &curMod = processor_.getLoadedPlugin(PluginProcessor::M_MAIN);
    auto &modules = processor_.getSupportedModules();
    auto &newMod = modules[curModIdx_];
    if (newMod != curMod) {
        // time to load a new module !
        bool r = false;
        while (!r) { r = processor_.requestModuleChange(PluginProcessor::M_MAIN, newMod); }
    }
}

void LoadView::onUpButton(bool v) {
    upBtn_.value(v);
    if (v) return;
    if (curModIdx_ > 0) curModIdx_--;
}

void LoadView::onDownButton(bool v) {
    downBtn_.value(v);
    if (v) return;

    int maxSz = processor_.getSupportedModules().size();
    if (curModIdx_ < 0)
        curModIdx_ = 0;
    else if (curModIdx_ < (maxSz - 1))
        curModIdx_++;
}

void LoadView::onEncoder(unsigned enc, float v) {
    if (v > 0.0f) {
        int maxSz = processor_.getSupportedModules().size();
        if (curModIdx_ < 0)
            curModIdx_ = 0;
        else if (curModIdx_ < (maxSz - 1))
            curModIdx_++;
    } else if (v < 0.0f) {
        if (curModIdx_ > 0) curModIdx_--;
    }
}

void LoadView::onEncoderSwitch(unsigned enc, bool v) {
    if (v) return;

    loadModule();
}

void LoadView::onButton(unsigned int btn, bool v) {
    switch (btn) {
        case 0: {
            scanBtn_.value(v);
            if (v) return;
            processor_.scanPlugins();
            editorShown();  // reset current plugin
            break;
        }
        case 3: {
            loadBtn_.value(v);
            if (v) return;
            loadModule();
            break;
        }
        case 7: {
            clearBtn_.value(v);
            if (v) return;
            bool r = false;
            while (!r) { r = processor_.requestModuleChange(PluginProcessor::M_MAIN, ""); }
            break;
        }
    }
}

void LoadView::editorShown() {
    curModIdx_ = -1;
    int i = 0;
    auto &modules = processor_.getSupportedModules();
    auto &curMod = processor_.getLoadedPlugin(PluginProcessor::M_MAIN);
    for (auto &m : modules) {
        if (m == curMod) {
            curModIdx_ = i;
            break;
        }
        i++;
    }
}

