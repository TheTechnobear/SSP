#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    auto view = std::make_shared<ssp::MiniParamView>(&p);

    static constexpr float defCoarse = 0.1, defFine = 0.01;
    {  // AB
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.Freq, 100, 1));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.Decay, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.Tone, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.Accent, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.Gain, 1.0f, 0.05f));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.AB_AttackFM, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ab_.AB_SelfFM, defCoarse, defFine));
    }

    {  // SB
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.Freq, 100, 1));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.Decay, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.Accent, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.Gain, 1.0f, 0.05f));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_Dirt, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_FMDecay, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_EnvFM, defCoarse, defFine));
    }

    {  // AS
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.as_.Freq, 100, 1));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.as_.Decay, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.as_.Tone, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.as_.Accent, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.as_.Gain, 1.0f, 0.05f));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.as_.AS_Snappy, defCoarse, defFine));
    }
    {  // SS
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ss_.Freq, 100, 1));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ss_.Decay, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ss_.Accent, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ss_.Gain, 1.0f, 0.05f));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ss_.SS_FM, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.ss_.SS_Snappy, defCoarse, defFine));
    }
    {  // HH1
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Freq, 100, 1));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Decay, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Tone, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Accent, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Gain, 1.0f, 0.05f));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh1_.HH1_Noise, defCoarse, defFine));
    }
    {  // HH2
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Freq, 100, 1));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Decay, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Tone, defCoarse, defFine));
        view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Accent, defCoarse, defFine));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Gain, 1.0f, 0.05f));
        // view->addParam(std::make_shared<pcontrol_type>(processor_.params_.hh2_.HH2_Noise, defCoarse, defFine));
    }


    addView(view);
    setView(0);
}
