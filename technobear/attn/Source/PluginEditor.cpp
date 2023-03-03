
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p), processor_(p) {

    for (unsigned sigpage = 0; sigpage < MAX_SIG / 4; sigpage++) {
        int sigp=sigpage * 4;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 0]->val, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 1]->val, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 2]->val, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.attnparams_[sigp + 3]->val, 0.25)
        );
    }

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.slew, 24, Colours::lightskyblue),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    setSize(1600, 480);

}

