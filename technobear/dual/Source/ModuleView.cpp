#include "ModuleView.h"

ModuleView::ModuleView(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    if (processor_.getSupportedModules().size() == 0) { processor_.loadSupportedModules(); }
}

ModuleView::~ModuleView() {
}

void ModuleView::drawView(Graphics &g) {
    base_type::drawView(g);
    drawModulePanel(g);
}

void ModuleView::onSSPTimer() {
    base_type::onSSPTimer();
    auto editor = processor_.getEditor(trackIdx_, moduleIdx_);
    if (editor) {
        editor->frameStart();
        auto pComponent = editor->editorComponent();
        pComponent->repaint();
    }
}

void ModuleView::drawModulePanel(Graphics &g) {
    int fh = 14 * COMPACT_UI_SCALE;
    unsigned panelWidth = pluginWidth;
    // unsigned panelHeight = pluginHeight;

    auto editor = processor_.getEditor(trackIdx_, moduleIdx_);
    if (!editor) {
        int x = pluginWidth / 2;
        g.setColour(Colours::white);

        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 2 * fh, juce::Font::plain)));
        g.drawSingleLineText("No Module Loaded", x - fh * 8, getHeight() / 2 - (20 * COMPACT_UI_SCALE));
        return;
    }

    auto pComponent = editor->editorComponent();
    if (pComponent) { pComponent->paintEntireComponent(g, true); }
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

void ModuleView::editorShown() {
    auto plugin = processor_.getEditor(trackIdx_, moduleIdx_);
    if (plugin) { plugin->visibilityChanged(true); }
}


void ModuleView::editorHidden() {
    auto plugin = processor_.getEditor(trackIdx_, moduleIdx_);
    if (plugin) { plugin->visibilityChanged(false); }
}