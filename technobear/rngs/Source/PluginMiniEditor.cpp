#include "PluginMiniEditor.h"

#include "PluginProcessor.h"

#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) :
    base_type(&p), processor_(p) {

    auto view =
        std::make_shared<ssp::MiniParamView>(&p, [&](bool io, int bus) { return p.ioActivity(io, bus); });

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.pitch));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.structure));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.brightness));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.damping));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.position));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.polyphony, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.model, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.in_gain));

    addView(view);
    setView(0);
}

void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}

void PluginMiniEditor::resized() {
    base_type::resized();
}
