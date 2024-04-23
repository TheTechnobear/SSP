#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p), processor_(p), resetButton_("Reset", nullptr, 32, Colours::yellow) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.freq, 50.0f, 0.01f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.wave, 1.0, 1.0f));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.amp, 0.1, 0.01));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.phase, 10.0f, 0.1));

    for (unsigned oid = 0; oid < PluginProcessor::MAX_S_OSC; oid++) {
        auto &op = processor_.getSlaveOscParam(oid);
        view->addParam(std::make_shared<pcontrol_type>(op.ratio, 0.1f, 0.01f));
        view->addParam(std::make_shared<pcontrol_type>(op.wave, 1.0, 1.0f));
        view->addParam(std::make_shared<pcontrol_type>(op.amp, 0.1, 0.01));
        view->addParam(std::make_shared<pcontrol_type>(op.phase, 10.0f, 0.1));
    }

    view->addButton(std::make_shared<bcontrol_type>(processor_.params_.lfo, 32, Colours::lightskyblue));

    addView(view);
    setView(0);
}
