
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p), processor_(p) {


    // TODO
    // each page = 4*4 parameters
    // as always enc+/- switch parameters
    // layer+/- (left/right) switches between layers/
    // we also need to have a 'page' for general parameters (e.g. select)


     for (unsigned layer= 0; layer < PluginProcessor::MAX_LAYERS;layer++) {
         auto& l = processor_.getLayer(layer);
         for (unsigned sigpage = 0; sigpage < MAX_SIG / 4; sigpage++) {
             int sigp = sigpage * 4;
             addParamPage(
                 std::make_shared<pcontrol_type>(l.volts_[sigp + 0]->val, 0.25),
                 std::make_shared<pcontrol_type>(l.volts_[sigp + 1]->val, 0.25),
                 std::make_shared<pcontrol_type>(l.volts_[sigp + 2]->val, 0.25),
                 std::make_shared<pcontrol_type>(l.volts_[sigp + 3]->val, 0.25)
             );
         }
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

