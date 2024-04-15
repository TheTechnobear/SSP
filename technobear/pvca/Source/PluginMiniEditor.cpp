#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p), processor_(p), resetButton_("Reset", nullptr, 32, Colours::yellow) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);
    // std::make_shared<ssp::MiniParamView>(&p, [&](bool io, int bus) { return p.ioActivity(io, bus); });

    for (unsigned vid = 0; vid < PluginProcessor::MAX_VCA; vid++) {
        auto &op = processor_.getVcaParam(vid);
        view->addParam(std::make_shared<pcontrol_type>(op.gain, 0.1f, 0.01f));
        view->addParam(std::make_shared<pcontrol_type>(op.pan, 0.1f, 0.01f));
    }

    addView(view);
    setView(0);
}
