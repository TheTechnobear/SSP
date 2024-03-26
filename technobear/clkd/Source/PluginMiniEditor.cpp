#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p),
      runButton_("Run", nullptr, 32, Colours::green),
      resetButton_("Reset", nullptr, 32, Colours::yellow),
      useTrigsButton_("TrigSync", nullptr, 20, Colours::cyan) {

    //TODO, value buttons support and IO activity?
    auto view = std::make_shared<ssp::MiniParamView>(&p);
    // std::make_shared<ssp::MiniParamView>(&p, [&](bool io, int bus) { return p.ioActivity(io, bus); });

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.source, 1.0f, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.clkindiv, 1.0f, 0.25f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.bpm, 10.0f, 0.1f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.midippqn, 1.0f, 1.0f));

    for (int d = 0; d < PluginProcessor::MAX_CLK_OUT; d++) {
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.divisions_[d]->val, 1, 0.25));
    }


    addView(view);
    setView(0);
}
