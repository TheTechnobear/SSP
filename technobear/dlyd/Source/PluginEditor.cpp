
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_TAPS = PluginProcessor::MAX_TAPS;
static constexpr unsigned MAX_VIEW = MAX_TAPS + 1;
static constexpr unsigned L_CLRS = 5;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p) {

    static juce::Colour clrs[L_CLRS] = {Colours::red, Colours::blue, Colours::yellow, Colours::green, Colours::cyan};
    unsigned view = 0;
    static constexpr float defCoarse = 10.0f;
    static constexpr float defFine = 0.1f;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.size, defCoarse, defFine),
        std::make_shared<pcontrol_type>(processor_.params_.mix, defCoarse, defFine),
        std::make_shared<pcontrol_type>(processor_.params_.in_level, defCoarse, defFine),
        std::make_shared<pcontrol_type>(processor_.params_.out_level, defCoarse, defFine),
        view,
        Colours::orange
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
    view++;

    for (unsigned tap = 0; tap < MAX_TAPS; tap++) {
        auto &t = processor_.getTap(tap);

        addParamPage(
            std::make_shared<pcontrol_type>(t.time, defCoarse, defFine),
            std::make_shared<pcontrol_type>(t.pan, defCoarse, defFine),
            std::make_shared<pcontrol_type>(t.level, defCoarse, defFine),
            std::make_shared<pcontrol_type>(t.feedback, defCoarse, defFine),
            view,
            clrs[view % L_CLRS]
        );
        addParamPage(
            std::make_shared<pcontrol_type>(t.repeats, defCoarse, defFine),
            nullptr,
            nullptr,
            nullptr,
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
        view++;
    }

    setSize(1600, 480);

}

