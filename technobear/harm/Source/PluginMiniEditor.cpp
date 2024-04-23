#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p, [&](bool io, int bus) { return p.ioActivity(io, bus); });

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.pitch, 1.0f, 0.01));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.first, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.centre, 1.0f, 0.1f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.spread, 1.0f, 0.1f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.amount, 0.1, 0.01f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.spread));

    addView(view);
    setView(0);
}

void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}

void PluginMiniEditor::resized() {
    base_type::resized();
}
