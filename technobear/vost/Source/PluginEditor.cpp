
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

// main params + layer voltages

static constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;
static constexpr unsigned MAX_VIEW = MAX_LAYERS + 1;
static constexpr unsigned L_CLRS = 5;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p) {

    static juce::Colour clrs[L_CLRS] = {Colours::red, Colours::blue, Colours::yellow, Colours::green,Colours::cyan};
    unsigned view = 0;

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.select, 1.0f, 0.01),
        nullptr,
        nullptr,
        nullptr,
        view,
        Colours::orange
    );

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.slew, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.morph, 24, Colours::orange),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        view
    );
    view++;

    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.getLayer(layer);
        for (unsigned vPage = 0; vPage < MAX_SIG / 4; vPage++) {
            int sigp = vPage * 4;
            addParamPage(
                std::make_shared<pcontrol_type>(l.volts_[sigp + 0]->val, 0.25),
                std::make_shared<pcontrol_type>(l.volts_[sigp + 1]->val, 0.25),
                std::make_shared<pcontrol_type>(l.volts_[sigp + 2]->val, 0.25),
                std::make_shared<pcontrol_type>(l.volts_[sigp + 3]->val, 0.25),
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
        }
        view++;
    }


    setSize(1600, 480);

}

