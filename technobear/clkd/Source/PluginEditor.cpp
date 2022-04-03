
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p), processor_(p) {


    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.source,1.0f,1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.clkindiv, 1.0f, 0.25f),
        std::make_shared<pcontrol_type>(processor_.params_.bpm, 10.0f, 0.1f),
        std::make_shared<pcontrol_type>(processor_.params_.midippqn,1.0f,1.0f)
    );

    unsigned d=0;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 0]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 1]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 2]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 3]->val, 1, 0.25)
    );
    d+=4;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 0]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 1]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 2]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 3]->val, 1, 0.25)
    );

    setSize(1600, 480);
}

