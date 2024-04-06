#include "DualView.h"

#include "ModuleView.h"
#include "SSP.h"

static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;


/// DualView

DualView::DualView(PluginProcessor &p) : ssp::BaseView(&p, false), processor_(p) {
}

DualView::~DualView() {
}

void DualView::drawView(Graphics &g) {
    for (int i = 0; i < PluginProcessor::M_MAX; i++) { drawModulePanel(g, i); }
}


void DualView::moduleIdx(int m) {
    refreshComponents();
    if (activeModule_ != m) {
        if (pComponent_[0]) pComponent_[0]->setAlpha(m == 0 ? 1.0f : 0.3f);
        if (pComponent_[1]) pComponent_[1]->setAlpha(m == 1 ? 1.0f : 0.3f);
        activeModule_ = m;
    }
}

void DualView::refreshComponents() {
    unsigned panelWidth = (SSP_FULL_WIDTH - 10) / PluginProcessor::M_MAX;
    unsigned border = (panelWidth - SSP_COMPACT_WIDTH) / 2;

    for (int panel = 0; panel < PluginProcessor::M_MAX; panel++) {
        // we are currently using editorhost->baseview, this is because editorshost is AudioProcessorEditor
        // and it appears AudioProcessorEditor has special handling for resizing etc.
        unsigned moduleX = (panel * panelWidth) + border;

        if (pComponent_[panel] != nullptr) removeChildComponent(pComponent_[panel]);

        pComponent_[panel] = nullptr;
        auto editor = processor_.getEditor(panel);
        if (editor) pComponent_[panel] = editor->editorComponent();
        if (pComponent_[panel] != nullptr) {
            addChildComponent(pComponent_[panel]);
            pComponent_[panel]->setBounds(moduleX, 0, pluginWidth, pluginHeight);
            pComponent_[panel]->resized();
            pComponent_[panel]->setAlpha(panel == activeModule_ ? 1.0f : 0.3f);
            // juce crashes if we let mouse events go to components
            // possibly something to do with AudioProcessorEditor and us using 'its' component!
            pComponent_[panel]->setInterceptsMouseClicks(false, false);
        }
    }
}

void DualView::editorShown() {
    refreshComponents();
}

void DualView::drawModulePanel(Graphics &g, unsigned panel) {
    unsigned panelWidth = (SSP_FULL_WIDTH - 10) / PluginProcessor::M_MAX;
    unsigned border = (panelWidth - SSP_COMPACT_WIDTH) / 2;
    unsigned moduleX = (panel * panelWidth) + border;

    auto editor = processor_.getEditor(panel);
    if (!editor) {
        if (panel == activeModule_)
            g.setColour(Colours::white);
        else
            g.setColour(Colours::darkgrey);

        g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
        g.drawSingleLineText("No Module Loaded", moduleX + 40, getHeight() / 2 - 20);
        g.setFont(Font(Font::getDefaultMonospacedFontName(), 16, Font::plain));
        g.drawSingleLineText("(hold SHIFT to load)", moduleX + 40, getHeight() / 2);
        return;
    }
}

void DualView::onEncoder(unsigned enc, float v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->encoderTurned(enc, v);
}

void DualView::onEncoderSwitch(unsigned enc, bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->encoderPressed(enc, v);
}

void DualView::onButton(unsigned btn, bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->buttonPressed(btn, v);
}


void DualView::onUpButton(bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->buttonPressed(SSP_Up, v);
}

void DualView::onDownButton(bool v) {
    auto plugin = processor_.getPlugin(activeModule_);
    if (!plugin) return;
    plugin->buttonPressed(SSP_Down, v);
}
