#include "PluginProcessor.h"
#include "PluginEditor.h"


#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/BaseParamControl.h"
#include "ssp/ButtonControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ButtonControl;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p,
                String(JucePlugin_Name) + " : " + String(JucePlugin_Desc),
                JucePlugin_VersionString),
      processor_(p) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.root, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.scale, 1.0f, 1.0f),
        nullptr,
        nullptr
    );

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.quant, 24, Colours::lightskyblue),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    // only one page of parameters, so up/down not required
    upBtn_.setVisible(false);
    downBtn_.setVisible(false);
    setSize(1600, 480);
}



