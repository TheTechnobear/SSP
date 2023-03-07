#include "PluginMiniEditor.h"

#include "PluginProcessor.h"

#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) :
    base_type(&p), processor_(p) {

    auto view =
        std::make_shared<ssp::MiniParamView>(&p, [&](bool io, int bus) { return p.ioActivity(io, bus); });

    static constexpr float defCoarse = 10.0f;
    static constexpr float defFine = 0.1f;

    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.size, defCoarse, defFine));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.mix, defCoarse, defFine));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.in_level, defCoarse, defFine));
    view->addParam(std::make_shared<pcontrol_type>(processor_.params_.out_level, defCoarse, defFine));


    view->addButton(std::make_shared<bcontrol_type>(processor_.params_.freeze, 12 * SSP_COMPACT_SCALE, Colours::lightskyblue));

    addView(view);
    setView(0);
}

void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}

void PluginMiniEditor::resized() {
    base_type::resized();
}
