
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

// main params + layer voltages


static constexpr unsigned MAX_VIEW = 1;
static constexpr unsigned L_CLRS = 4;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p) {

    static juce::Colour clrs[L_CLRS] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
    unsigned view = 0;


    static constexpr float fltCoarse = 500.0f, fltFin = 10.0f;
    unsigned f = 0;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getFilter(f).cutoff, fltCoarse, fltFin),
        std::make_shared<pcontrol_type>(processor_.getFilter(f).res, 0.1, 0.01f),
        nullptr,
        nullptr,
        view,
        clrs[f % L_CLRS]
    );

    f++;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getFilter(f).cutoff, fltCoarse, fltFin),
        std::make_shared<pcontrol_type>(processor_.getFilter(f).res, 0.1, 0.01f),
        nullptr,
        nullptr,
        view,
        clrs[f % L_CLRS]
    );

    f++;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getFilter(f).cutoff, fltCoarse, fltFin),
        std::make_shared<pcontrol_type>(processor_.getFilter(f).res, 0.1, 0.01f),
        nullptr,
        nullptr,
        view,
        clrs[f % L_CLRS]
    );

    f++;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.getFilter(f).cutoff, fltCoarse, fltFin),
        std::make_shared<pcontrol_type>(processor_.getFilter(f).res, 0.1, 0.01f),
        nullptr,
        nullptr,
        view,
        clrs[f % L_CLRS]
    );


    setSize(1600, 480);
}
