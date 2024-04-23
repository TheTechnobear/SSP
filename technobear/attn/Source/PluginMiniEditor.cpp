#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    static constexpr unsigned MAX_SIG = PluginProcessor::MAX_SIG_IN;

    for (unsigned sigpage = 0; sigpage < MAX_SIG / 4; sigpage++) {
        int sigp = sigpage * 4;
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 0]->val, 0.25));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 1]->val, 0.25));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 2]->val, 0.25));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 3]->val, 0.25));
    }

    view->addButton(
        std::make_shared<bcontrol_type>(processor_.params_.slew, 12 * SSP_COMPACT_SCALE, Colours::lightskyblue));

    addView(view);
    setView(0);
}
