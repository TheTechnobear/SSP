
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

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
        std::make_shared<bcontrol_type>(processor_.params_.freeze, 24, Colours::lightskyblue),
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
            std::make_shared<pcontrol_type>(t.level, defCoarse, defFine),
            std::make_shared<pcontrol_type>(t.feedback, defCoarse, defFine),
            std::make_shared<pcontrol_type>(t.pan, 0.1, 0.01),
            view,
            clrs[view % L_CLRS]
        );
        addParamPage(
            std::make_shared<pcontrol_type>(t.lpf, 100, 5),
            std::make_shared<pcontrol_type>(t.hpf, 100, 5),
            std::make_shared<pcontrol_type>(t.noise, 1, 0.01),
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

    inVu_.init("In");
    outVu_.init("Out");

    float inL, inR, outL, outR;
    processor_.getRMS(inL, inR, outL, outR);
    inVu_.level(inL, inR);
    outVu_.level(outL, outR);

    addAndMakeVisible(inVu_);
    addAndMakeVisible(outVu_);
}

void PluginEditor::drawView(Graphics &g) {
    float inL, inR, outL, outR;
    processor_.getRMS(inL, inR, outL, outR);
    inVu_.level(inL, inR);
    outVu_.level(outL, outR);

    base_type::drawView(g);
}


void PluginEditor::resized() {
    base_type::resized();
    const unsigned h = 130;
    const unsigned sp = 10;
    const unsigned vuStart = 1500;
    const unsigned vuW = 45;
    unsigned y = 50;
    unsigned x = vuStart;
    inVu_.setBounds(x, y, vuW, h);
    x += vuW + sp;
    outVu_.setBounds(x, y, vuW, h);
}