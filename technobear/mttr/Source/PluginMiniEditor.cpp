#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_a, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_b, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_c, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_d, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_e, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_f, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_g, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.tr_h, 1.0f, 1.0f));

    addView(view);
    setView(0);
}
