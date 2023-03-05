#include "PluginMiniEditor.h"

#include "PluginProcessor.h"

//#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) :
    ssp::BaseMiniView(&p),
    processor_(p) {

    auto view = std::make_shared<ssp::MiniParamView>(&p);
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.pitch));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.harmonics));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.timbre));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.morph));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.model, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.freq_mod));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.timbre_mod));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.morph_mod));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.lpg));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.vca));
    addView(view);
    setView(0);
}

void PluginMiniEditor::drawView(Graphics &) {
}

void PluginMiniEditor::resized() {
}
