#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    for (const auto &param : processor_.params_.rnboParams_) {
        float inc = 1.0f;
        float finc = 0.01f;
        if (param->info_.enumValues != nullptr) {
            finc = inc;
        } else if (param->info_.steps > 2) {
            inc = (param->info_.max - param->info_.min) / (param->info_.steps - 1);
            finc = inc;
        }

        view->addParam(std::make_shared<pcontrol_type>(param->val_, inc, finc));
    }

    addView(view);
    setView(0);
}
