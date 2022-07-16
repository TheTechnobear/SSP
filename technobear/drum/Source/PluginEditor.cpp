
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

// main params + layer voltages

static constexpr unsigned MAX_VIEW = PluginProcessor::O_MAX;
static constexpr unsigned L_CLRS = PluginProcessor::O_MAX;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p) {

    static juce::Colour clrs[L_CLRS] = {Colours::cyan, Colours::lightcyan,
                                        Colours::green, Colours::lightgreen,
                                        Colours::blue, Colours::lightblue};
    unsigned view;
    static constexpr float defCoarse = 0.1, defFine = 0.01;
    { // AB
        view = 0;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.ab_.Freq, 100, 1),
            std::make_shared<pcontrol_type>(processor_.params_.ab_.Decay, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.ab_.Tone, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.ab_.Accent, defCoarse, defFine),
            view,
            clrs[view]
        );

        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.ab_.Gain, 1.0f, 0.05f),
            std::make_shared<pcontrol_type>(processor_.params_.ab_.AB_AttackFM, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.ab_.AB_SelfFM, defCoarse, defFine),
            nullptr,
            view,
            clrs[view]
        );
    }

    { // SB
        view++;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.sb_.Freq, 100, 1),
            std::make_shared<pcontrol_type>(processor_.params_.sb_.Decay, defCoarse, defFine),
//            std::make_shared<pcontrol_type>(processor_.params_.sb_.Tone, defCoarse, defFine),
            nullptr,
            std::make_shared<pcontrol_type>(processor_.params_.sb_.Accent, defCoarse, defFine),
            view,
            clrs[view]
        );

        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.sb_.Gain, 1.0f, 0.05f),
            std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_Dirt, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_FMDecay, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.sb_.SB_EnvFM, defCoarse, defFine),
            view,
            clrs[view]
        );
    }

    { // AS
        view++;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.as_.Freq, 100, 1),
            std::make_shared<pcontrol_type>(processor_.params_.as_.Decay, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.as_.Tone, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.as_.Accent, defCoarse, defFine),
            view,
            clrs[view]
        );

        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.as_.Gain, 1.0f, 0.05f),
            std::make_shared<pcontrol_type>(processor_.params_.as_.AS_Snappy, defCoarse, defFine),
            nullptr,
            nullptr,
            view,
            clrs[view]
        );
    }
    { // SS
        view++;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.ss_.Freq, 100, 1),
            std::make_shared<pcontrol_type>(processor_.params_.ss_.Decay, defCoarse, defFine),
//            std::make_shared<pcontrol_type>(processor_.params_.ss_.Tone, defCoarse, defFine),
            nullptr,
            std::make_shared<pcontrol_type>(processor_.params_.ss_.Accent, defCoarse, defFine),
            view,
            clrs[view]
        );

        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.ss_.Gain, 1.0f, 0.05f),
            std::make_shared<pcontrol_type>(processor_.params_.ss_.SS_FM, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.ss_.SS_Snappy, defCoarse, defFine),
            nullptr,
            view,
            clrs[view]
        );
    }
    { // HH1
        view++;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.hh1_.Freq, 100, 1),
            std::make_shared<pcontrol_type>(processor_.params_.hh1_.Decay, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.hh1_.Tone, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.hh1_.Accent, defCoarse, defFine),
            view,
            clrs[view]
        );

        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.hh1_.Gain, 1.0f, 0.05f),
            std::make_shared<pcontrol_type>(processor_.params_.hh1_.HH1_Noise, defCoarse, defFine),
            nullptr,
            nullptr,
            view,
            clrs[view]
        );
    }
    { // HH2
        view++;
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.hh2_.Freq, 100, 1),
            std::make_shared<pcontrol_type>(processor_.params_.hh2_.Decay, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.hh2_.Tone, defCoarse, defFine),
            std::make_shared<pcontrol_type>(processor_.params_.hh2_.Accent, defCoarse, defFine),
            view,
            clrs[view]
        );

        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.hh2_.Gain, 1.0f, 0.05f),
            std::make_shared<pcontrol_type>(processor_.params_.hh2_.HH2_Noise, defCoarse, defFine),
            nullptr,
            nullptr,
            view,
            clrs[view]
        );
    }


    setSize(1600, 480);

}

