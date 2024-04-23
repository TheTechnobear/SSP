#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p, [&](bool io, int bus) { return p.ioActivity(io, bus); });

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.mix));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.feedback));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.lpfreq, 100, 5));
    view->addButton(
        std::make_shared<bcontrol_type>(processor_.params_.freeze, 12 * SSP_COMPACT_SCALE, Colours::lightskyblue));

    addView(view);
    setView(0);
}

void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}

void PluginMiniEditor::resized() {
    base_type::resized();
}
