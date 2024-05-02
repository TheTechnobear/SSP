#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    static constexpr unsigned L_CLRS = PluginProcessor::O_MAX;
    static juce::Colour clrs[L_CLRS] = { Colours::cyan,  Colours::red,  Colours::orange,
                                         Colours::green, Colours::blue, Colours::yellow };

    int clridx = 0;
    static constexpr float defCoarse = 0.1, defFine = 0.01;
    {  // AB
        auto clr = clrs[clridx++];
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.ab_.Freq, 100, 1),
                     std::make_shared<pcontrol_type>(processor_.params_.ab_.Decay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.ab_.Tone, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.ab_.Accent, defCoarse, defFine), clr);


        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.ab_.Gain, 1.0f, 0.05f),
                     std::make_shared<pcontrol_type>(processor_.params_.ab_.AB_AttackFM, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.ab_.AB_SelfFM, defCoarse, defFine), nullptr,
                     clr);
    }

    {  // SB
        auto clr = clrs[clridx++];
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.sb_.Freq, 100, 1),
                     std::make_shared<pcontrol_type>(processor_.params_.sb_.Decay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.sb_.Accent, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.sb_.Gain, 1.0f, 0.05f), clr);

        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_Dirt, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_FMDecay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_EnvFM, defCoarse, defFine), nullptr,
                     clr);
    }

    {  // AS
        auto clr = clrs[clridx++];
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.as_.Freq, 100, 1),
                     std::make_shared<pcontrol_type>(processor_.params_.as_.Decay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.as_.Tone, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.as_.Accent, defCoarse, defFine), clr);

        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.as_.Gain, 1.0f, 0.05f),
                     std::make_shared<pcontrol_type>(processor_.params_.as_.AS_Snappy, defCoarse, defFine), nullptr,
                     nullptr, clr);
    }
    {  // SS
        auto clr = clrs[clridx++];
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.ss_.Freq, 100, 1),
                     std::make_shared<pcontrol_type>(processor_.params_.ss_.Decay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.ss_.Accent, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.ss_.Gain, 1.0f, 0.05f), clr);
    }
    {  // HH1
        auto clr = clrs[clridx++];
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Freq, 100, 1),
                     std::make_shared<pcontrol_type>(processor_.params_.hh1_.Decay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.hh1_.Tone, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.hh1_.Accent, defCoarse, defFine), clr);

        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.hh1_.Gain, 1.0f, 0.05f),
                     std::make_shared<pcontrol_type>(processor_.params_.hh1_.HH1_Noise, defCoarse, defFine), nullptr,
                     nullptr, clr);
    }
    {  // HH2
        auto clr = clrs[clridx++];
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Freq, 100, 1),
                     std::make_shared<pcontrol_type>(processor_.params_.hh2_.Decay, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.hh2_.Tone, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(processor_.params_.hh2_.Accent, defCoarse, defFine), clr);


        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.hh2_.Gain, 1.0f, 0.05f),
                     std::make_shared<pcontrol_type>(processor_.params_.hh2_.HH2_Noise, defCoarse, defFine), nullptr,
                     nullptr, clr);
    }
}
