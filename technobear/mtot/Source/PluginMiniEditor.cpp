#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_a, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_b, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_c, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_d, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_e, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_f, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_g, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.cv_h, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.pb_range, 1.0f, 1.0f));

    addView(view);
    setView(0);
}
