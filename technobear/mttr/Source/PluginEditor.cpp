
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.tr_a, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.tr_b, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.tr_c, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.tr_d, 1.0f, 1.0f)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.tr_e, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.tr_f, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.tr_g, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.tr_h, 1.0f, 1.0f)
    );

    setSize(1600, 480);
}
