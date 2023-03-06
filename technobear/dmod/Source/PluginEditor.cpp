#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SSPApiEditor.h"

#include "ssp/editors/MultiView.h"

//TODO
// - PluginEditorInterface::draw  ... I don't use this, not sure when it would be needed


static constexpr unsigned MAX_MODULES = 2;
static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

class ModuleView : public ssp::BaseView {
public:
    ModuleView(PluginProcessor &p)
        : ssp::BaseView(&p, false),
          processor_(p) {
    }

    void drawView(Graphics &g) override;
    void drawModulePanel(Graphics &, unsigned);
    void drawIO(Graphics &, unsigned);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
//    void onLeftButton(bool v) override;
//    void onRightButton(bool v) override;
//    void onUpButton(bool v) override;
//    void onDownButton(bool v) override;
//    void onLeftShiftButton(bool v) override;
//    void onRightShiftButton(bool v) override;

    void setActiveModule(int m) { activeModule_ = m; }

private:

    int activeModule_ = 0;
    PluginProcessor &processor_;
};


class LoadView : public ssp::BaseView {
public:
    LoadView(PluginProcessor &p)
        : ssp::BaseView(&p, false),
          processor_(p) {
    }

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
//    void onLeftButton(bool v) override;
//    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;


    void drawView(Graphics &g) override;
    void setActiveModule(int m);

    void editorShown() override;

private:

    void loadModule();

    int activeModule_ = 0;
    int curModIdx_ = -1;
    PluginProcessor &processor_;
};


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, false),
      processor_(p) {

    activeModule_ = 0;
    moduleView_ = std::make_shared<ModuleView>(p);
    loadView_ = std::make_shared<LoadView>(p);
    addView(moduleView_);
    addView(loadView_);
    moduleView_->setActiveModule(activeModule_);
    loadView_->setActiveModule(activeModule_);
    setView(0);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}


void PluginEditor::onSSPTimer() {
    base_type::onSSPTimer();
    for (int i = 0; i < MAX_MODULES; i++) {
//        auto editor = processor_.createEditorIfNeeded(i);
//        if (!editor) continue;
//        editor->frameStart();
    }
}

void PluginEditor::editorShown() {
    base_type::editorShown();
    for (int i = 0; i < MAX_MODULES; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->visibilityChanged(true);
    }
}

void PluginEditor::editorHidden() {
    base_type::editorHidden();
    for (int i = 0; i < MAX_MODULES; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->visibilityChanged(false);
    }
}


void PluginEditor::onLeftButton(bool v) {
//    base_type::onLeftButton(v);
    if (!v) activeModule_ = 0;
    moduleView_->setActiveModule(activeModule_);
    loadView_->setActiveModule(activeModule_);
}

void PluginEditor::onRightButton(bool v) {
//    base_type::onRightButton(v);
    if (!v) activeModule_ = 1;
    moduleView_->setActiveModule(activeModule_);
    loadView_->setActiveModule(activeModule_);
}


void PluginEditor::onEncoderSwitch(unsigned int enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
    if (v) return;

    if (getViewIdx() == 1) {
        // load view
        setView(0);
    }
}

void PluginEditor::onRightShiftButton(bool v) {
//    base_type::onRightShiftButton(v);
    if (v) return;

    int viewIdx = getViewIdx();
    if (viewIdx == 1) {
        // let loadview, load a new module before switching
        base_type::onRightShiftButton(v);
    }

    viewIdx = !viewIdx;
    setView(viewIdx);
}

void PluginEditor::onLeftShiftButton(bool v) {
//    base_type::onRightShiftButton(v);
    if (v) return;

    int viewIdx = getViewIdx();
    if (viewIdx == 1) {
        // let loadview, load a new module before switching
        base_type::onLeftShiftButton(v);
    }

    viewIdx = !viewIdx;
    setView(viewIdx);
}


void PluginEditor::resized() {
    base_type::resized();
    for (int i = 0; i < MAX_MODULES; i++) {
        auto editor = processor_.getEditor(i);
        if (!editor) continue;
        editor->draw(pluginWidth, pluginHeight);
    }
}


/// ModuleView

void ModuleView::drawView(Graphics &g) {
    for (int i = 0; i < MAX_MODULES; i++) {
        drawModulePanel(g, i);
    }
}


void ModuleView::drawModulePanel(Graphics &g, unsigned panel) {
    auto editor = processor_.getEditor(panel);
    if (!editor) return;

    //TODO
    static constexpr unsigned long IMAGECACHE_HASHCODE = 0x53533500000;

    unsigned panelWidth = SSP_FULL_WIDTH / MAX_MODULES;
    unsigned border = panelWidth - SSP_COMPACT_WIDTH;
    unsigned panelX = (panel * panelWidth);
    unsigned moduleX = panelX + border - 2;

    drawIO(g, panel);

    if (panel == activeModule_) {
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::lightgrey);
    }
    g.drawLine(moduleX - 2, 0, moduleX - 2, SSP_FULL_HEIGHT, 2);
    g.drawLine(moduleX + SSP_COMPACT_WIDTH + 1, 0, moduleX + SSP_COMPACT_WIDTH + 1, SSP_FULL_HEIGHT, 2);

    Image img = ImageCache::getFromHashCode(IMAGECACHE_HASHCODE + panel);
    if (!img.isValid()) {
        Image newimg(Image::ARGB, pluginWidth, pluginHeight, true);
        ImageCache::addImageToCache(newimg, IMAGECACHE_HASHCODE + panel);
        img = newimg;
    }

    Image::BitmapData bitmap(img, Image::BitmapData::writeOnly);
    editor->renderToImage(bitmap.data, pluginWidth, pluginHeight);

    g.drawImageAt(img, moduleX, 0);
}


void ModuleView::drawIO(Graphics &g, unsigned panel) {
    unsigned panelWidth = SSP_FULL_WIDTH / 2;
    unsigned panelX = panel * panelWidth;

    unsigned x = panelX + 50;
    unsigned y = 100;
    static constexpr unsigned fh = 18;
    static constexpr unsigned h = fh + 5;
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    auto desc = processor_.getDescriptor(panel);
    if (desc) {
        g.setColour(Colours::yellow);
        g.drawSingleLineText("Inputs", x, y);
        y += h;

        g.setColour(Colours::white);
        int i = 8; // max 8 inputs
        const auto &inputs = desc->inputChannelNames;
        for (const auto &n: inputs) {
            g.drawSingleLineText(n, x, y);
            y += h;
            i--;
            if (i == 0) break;
        }
        y += i * h;
        y += h;

        g.setColour(Colours::yellow);
        g.drawSingleLineText("Outputs", x, y);
        y += h;
        i = 2; // max 2 outputs
        g.setColour(Colours::white);
        const auto &outputs = desc->outputChannelNames;
        for (const auto &n: outputs) {
            g.drawSingleLineText(n, x, y);
            y += h;
            if (i == 0) break;
        }
    }
}


void ModuleView::onEncoder(unsigned enc, float v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->encoderTurned(enc, v);
}

void ModuleView::onEncoderSwitch(unsigned enc, bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->encoderPressed(enc, v);
}

void ModuleView::onButton(unsigned btn, bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->buttonPressed(btn, v);
}


/// LoadView
std::string nicePlugName(const std::string &n) {
    std::string nn = n.substr(n.find_last_of('/') + 1);
#if __APPLE__
#else
    // linux, remove .so
    nn = nn.substr(0,nn.find_last_of('l'));
#endif
    return nn;
}

void LoadView::drawView(Graphics &g) {
    unsigned x = 50;
    unsigned y = 100;
    static constexpr unsigned fh = 18;
    static constexpr unsigned h = fh + 5;
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    auto &modules = processor_.getSupportedModules();

    g.setColour(Colours::yellow);
    g.drawSingleLineText("Load Module", x, y);
    y += h;
    y += h;
    int i = 0;
    for (auto &m: modules) {
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
    auto &curMod = processor_.getPluginFile(activeModule_);
    auto &modules = processor_.getSupportedModules();
    auto &newMod = modules[curModIdx_];
    if (newMod != curMod) {
        // time to load a new module !
        bool r=false;
        while(!r) {
            r=processor_.requestModuleChange(activeModule_, newMod);
        }
    }
}

void LoadView::onUpButton(bool v) {
    if (v) return;
    if (curModIdx_ > 0) curModIdx_--;
}

void LoadView::onDownButton(bool v) {
    if (v) return;

    int maxSz = processor_.getSupportedModules().size();
    if (curModIdx_ < 0) curModIdx_ = 0;
    else if (curModIdx_ < (maxSz - 1)) curModIdx_++;
}

void LoadView::onEncoder(unsigned enc, float v) {
    if (v > 0.0f) {
        int maxSz = processor_.getSupportedModules().size();
        if (curModIdx_ < 0) curModIdx_ = 0;
        else if (curModIdx_ < (maxSz - 1)) curModIdx_++;
    } else if (v < 0.0f) {
        if (curModIdx_ > 0) curModIdx_--;
    }
}

void LoadView::onEncoderSwitch(unsigned enc, bool v) {
    if (v) return;

    loadModule();
}

void LoadView::onButton(unsigned int btn, bool v) {
    if(v) return;
    if(btn==0) {
        processor_.scanPlugins();
        editorShown(); // reset current plugin
    }
}

void LoadView::onLeftShiftButton(bool v) {
    if (v) return;
    bool r=false;
    while(!r) {
        r = processor_.requestModuleChange(activeModule_, "");
    }
}

void LoadView::onRightShiftButton(bool v) {
    if (v) return;
    loadModule();
}

void LoadView::editorShown() {
    curModIdx_ = -1;
    int i = 0;
    auto &modules = processor_.getSupportedModules();
    auto &curMod = processor_.getPluginFile(activeModule_);
    for (auto &m: modules) {
        if (m == curMod) {
            curModIdx_ = i;
            break;
        }
        i++;
    }
}

void LoadView::setActiveModule(int m) {
    activeModule_ = m;
}

