
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

// main params + layer voltages

static constexpr unsigned MAX_HARMONICS = PluginProcessor::MAX_HARMONICS;
static constexpr unsigned MAX_VIEW = 2;
static constexpr unsigned L_CLRS = 5;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p) {

    static juce::Colour clrs[L_CLRS] = {Colours::red, Colours::blue, Colours::yellow, Colours::green, Colours::cyan};
    unsigned view = 0;

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.pitch, 1.0f, 0.01),
        std::make_shared<pcontrol_type>(processor_.params_.first, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.centre, 1.0f, 0.1f),
        std::make_shared<pcontrol_type>(processor_.params_.spread, 1.0f, 0.1f),
        view,
        Colours::orange
    );

//    addButtonPage(
//        nullptr,
//        nullptr,
//        nullptr,
//        nullptr,
//        nullptr,
//        nullptr,
//        nullptr,
//        nullptr,
//        view
//    );
    view++;

    unsigned h = 0;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getHarmonic(0).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(1).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(2).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(3).amp, 0.25),
        view,
        clrs[view % L_CLRS]
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getHarmonic(4).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(5).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(6).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(7).amp, 0.25),
        view,
        clrs[view % L_CLRS]
    );
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getHarmonic(8).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(9).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(10).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(11).amp, 0.25),
        view,
        clrs[view % L_CLRS]
    );
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getHarmonic(12).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(13).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(14).amp, 0.25),
        std::make_shared<pcontrol_type>(processor_.getHarmonic(15).amp, 0.25),
        view,
        clrs[view % L_CLRS]
    );


    addButtonPage(
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        view
    );
//    view++;

    setSize(1600, 480);
}
