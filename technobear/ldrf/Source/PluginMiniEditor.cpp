#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    static constexpr float fltCoarse = 500.0f, fltFin = 10.0f;
    for (int f = 0; f < PluginProcessor::MAX_FILTERS; f++) {
        view->addParam(std::make_shared<pcontrol_type>(processor_.getFilter(f).cutoff, fltCoarse, fltFin));
        view->addParam(std::make_shared<pcontrol_type>(processor_.getFilter(f).res, 0.1, 0.01f));
    }

    addView(view);
    setView(0);
}
