#include "PluginMiniEditor.h"

#include "PluginProcessor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) :
    ssp::BaseMiniView(&p),
    processor_(p) {

    auto view = std::make_shared<ssp::MiniParamView>(&p);

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.position));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.size));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.density));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.texture));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.mix));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.spread));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.feedback));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.reverb));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.pitch));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.mode, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.in_gain));
//    std::make_shared<bcontrol_type>(processor_.params_.freeze, 24, Colours::lightskyblue)

    addView(view);
    setView(0);
}

void PluginMiniEditor::drawView(Graphics &) {
}

void PluginMiniEditor::resized() {
}
