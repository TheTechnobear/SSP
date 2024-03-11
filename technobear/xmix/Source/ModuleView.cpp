#include "ModuleView.h"


// ModuleView
static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;


ModuleView::ModuleView(PluginProcessor &p) : ssp::BaseView(&p, false), processor_(p) {
}

ModuleView::~ModuleView() {
}

void ModuleView::drawView(Graphics &g) {
    drawModulePanel(g);
}


void ModuleView::drawModulePanel(Graphics &g) {
    unsigned panelWidth = pluginWidth;
    // unsigned panelHeight = pluginHeight;

    auto editor = processor_.getEditor(PluginProcessor::M_MAIN);
    if (!editor) {
        int x = pluginWidth / 2;
        g.setColour(Colours::white);

        g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
        g.drawSingleLineText("No Module Loaded", x - 40, getHeight() / 2 - 20);
        g.setFont(Font(Font::getDefaultMonospacedFontName(), 16, Font::plain));
        g.drawSingleLineText("(hold UP to load)", x - 40, getHeight() / 2);
        return;
    }

    // we are currently using editorhost->baseview, this is because editorshost is AudioProcessorEditor
    // and it appears AudioProcessorEditor has special handling for resizing etc.
    auto pComponent = editor->editorComponent();
    if (pComponent != pComponent_) {
        if (pComponent_ != nullptr) removeChildComponent(pComponent_);
        pComponent_ = pComponent;
        addChildComponent(pComponent_);
        pComponent->setBounds(0, 0, pluginWidth, pluginHeight);
        pComponent->resized();

        // juce crashes if we let mouse events go to components
        // possibly something to do with AudioProcessorEditor and us using 'its' component!
        pComponent->setInterceptsMouseClicks(false, false);
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

    auto desc = processor_.getDescriptor(PluginProcessor::M_MAIN);
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
    auto plugin = processor_.getPlugin(PluginProcessor::M_MAIN);
    if (!plugin) return;
    plugin->encoderTurned(enc, v);
}

void ModuleView::onEncoderSwitch(unsigned enc, bool v) {
    auto plugin = processor_.getPlugin(PluginProcessor::M_MAIN);
    if (!plugin) return;
    plugin->encoderPressed(enc, v);
}

void ModuleView::onButton(unsigned btn, bool v) {
    auto plugin = processor_.getPlugin(PluginProcessor::M_MAIN);
    if (!plugin) return;
    plugin->buttonPressed(btn, v);
}


void ModuleView::onUpButton(bool v) {
    auto plugin = processor_.getPlugin(PluginProcessor::M_MAIN);
    if (!plugin) return;
    plugin->buttonPressed(SSP_Up, v);
}

void ModuleView::onDownButton(bool v) {
    auto plugin = processor_.getPlugin(PluginProcessor::M_MAIN);
    if (!plugin) return;
    plugin->buttonPressed(SSP_Down, v);
}
