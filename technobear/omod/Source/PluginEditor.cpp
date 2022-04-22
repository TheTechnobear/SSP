
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_S_OSC = PluginProcessor::MAX_S_OSC;
static constexpr unsigned MAX_VIEW = 2;
static constexpr unsigned L_CLRS = 5;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p),
      resetButton_("Reset", nullptr, 32, Colours::yellow){

    static juce::Colour clrs[] = {Colours::red, Colours::blue, Colours::yellow, Colours::green, Colours::cyan};
    unsigned view = 0;
    unsigned page = 0;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.freq, 100.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.wave, 1.0, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.amp, 0.1, 0.01),
        std::make_shared<pcontrol_type>(processor_.params_.phase,10.0f, 0.1),
        view,
        Colours::orange
    );
    page++;

    for (unsigned oid = 0; oid < MAX_S_OSC; oid++) {
        auto &op = processor_.getSlaveOscParam(oid);

        addParamPage(
            std::make_shared<pcontrol_type>(op.ratio, 0.1f, 0.01f),
            std::make_shared<pcontrol_type>(op.wave, 1.0, 1.0f),
            std::make_shared<pcontrol_type>(op.amp, 0.1, 0.01),
            std::make_shared<pcontrol_type>(op.phase,10.0f, 0.1),
            view,
            clrs[oid % L_CLRS]
        );
        page++;
        if (page > 4) {
            page = 0;
            view++;
        }
    }

    setButtonBounds(resetButton_, 0, 0);
    addAndMakeVisible(resetButton_);

    setSize(1600, 480);

}

void PluginEditor::onButton(unsigned btn, bool v) {
    base_type::onButton(btn, v);
    if (!v) { // on release
        switch (btn) {
            case BN_RESET: {
                processor_.requestReset();
                break;
            }
        }
    }
}