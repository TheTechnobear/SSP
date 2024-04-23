#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.root, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.scale, 1.0f, 1.0f));

    view->addButton(std::make_shared<bcontrol_type>(processor_.params_.quant, 24, Colours::lightskyblue));

    addView(view);
    setView(0);
}

