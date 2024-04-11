#include "ModuleView.h"

#include "Module.h"



ModuleView::ModuleView(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    if (processor_.getSupportedModules().size() == 0) { processor_.scanPlugins(); }
}

ModuleView::~ModuleView() {
}

void ModuleView::drawView(Graphics &g) {
    base_type::drawView(g);
    drawModulePanel(g);
}

void ModuleView::onSSPTimer() {
    base_type::onSSPTimer();
    if (pComponent_) pComponent_->repaint();
}


void ModuleView::drawModulePanel(Graphics &g) {
    unsigned panelWidth = pluginWidth;
    // unsigned panelHeight = pluginHeight;

    auto editor = processor_.getEditor(trackIdx_, moduleIdx_);
    if (!editor) {
        int x = pluginWidth / 2;
        g.setColour(Colours::white);

        g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
        g.drawSingleLineText("No Module Loaded", x - 40, getHeight() / 2 - 20);
        return;
    }
}

void ModuleView::editorShown() {
    if (pComponent_ != nullptr) removeChildComponent(pComponent_);

    editor_ = processor_.getEditor(trackIdx_, moduleIdx_);
    if (editor_ != nullptr) {
        // we are currently using editorhost->baseview, this is because editorshost is AudioProcessorEditor
        // and it appears AudioProcessorEditor has special handling for resizing etc.
        pComponent_ = editor_->editorComponent();
        addChildComponent(pComponent_);
        pComponent_->setBounds(0, 0, pluginWidth, pluginHeight);
        // juce crashes if we let mouse events go to components
        // possibly something to do with AudioProcessorEditor and us using 'its' component!
        pComponent_->setInterceptsMouseClicks(false, false);
    } else {
        // no editor
        pComponent_ = nullptr;
    }
}


/// not currenly used. but we may do !
void ModuleView::drawIO(Graphics &g) {
    unsigned panelWidth = SSP_FULL_WIDTH / 2;
    unsigned panelX = 0;

    unsigned x = panelX + 50;
    unsigned y = 100;
    static constexpr unsigned fh = 18;
    static constexpr unsigned h = fh + 5;
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    auto desc = processor_.getDescriptor(trackIdx_, moduleIdx_);
    if (desc) {
        g.setColour(Colours::yellow);
        g.drawSingleLineText("Inputs", x, y);
        y += h;

        g.setColour(Colours::white);
        int i = 8;  // max 8 inputs
        const auto &inputs = desc->inputChannelNames;
        for (const auto &n : inputs) {
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
        i = 2;  // max 2 outputs
        g.setColour(Colours::white);
        const auto &outputs = desc->outputChannelNames;
        for (const auto &n : outputs) {
            g.drawSingleLineText(n, x, y);
            y += h;
            if (i == 0) break;
        }
    }
}


void ModuleView::onEncoder(unsigned enc, float v) {
    auto plugin = processor_.getPlugin(trackIdx_, moduleIdx_);
    if (!plugin) return;
    plugin->encoderTurned(enc, v);
}

void ModuleView::onEncoderSwitch(unsigned enc, bool v) {
    auto plugin = processor_.getPlugin(trackIdx_, moduleIdx_);
    if (!plugin) return;
    plugin->encoderPressed(enc, v);
}

void ModuleView::onButton(unsigned btn, bool v) {
    auto plugin = processor_.getPlugin(trackIdx_, moduleIdx_);
    if (!plugin) return;
    plugin->buttonPressed(btn, v);
}

void ModuleView::onUpButton(bool v) {
    auto plugin = processor_.getPlugin(trackIdx_, moduleIdx_);
    if (!plugin) return;
    plugin->buttonPressed(SSP_Up, v);
}

void ModuleView::onDownButton(bool v) {
    auto plugin = processor_.getPlugin(trackIdx_, moduleIdx_);
    if (!plugin) return;
    plugin->buttonPressed(SSP_Down, v);
}
