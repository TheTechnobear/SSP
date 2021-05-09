
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.cv_a, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.cv_b, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.cv_c, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.cv_d, 1.0f, 1.0f)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.cv_e, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.cv_f, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.cv_g, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.cv_h, 1.0f, 1.0f)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.pb_range, 1.0f, 1.0f),
        nullptr,
        nullptr,
        nullptr
    );

    setSize(1600, 480);
}
