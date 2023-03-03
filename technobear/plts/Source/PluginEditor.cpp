
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.pitch),
        std::make_shared<pcontrol_type>(processor_.params_.harmonics),
        std::make_shared<pcontrol_type>(processor_.params_.timbre),
        std::make_shared<pcontrol_type>(processor_.params_.morph)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.model, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.freq_mod),
        std::make_shared<pcontrol_type>(processor_.params_.timbre_mod),
        std::make_shared<pcontrol_type>(processor_.params_.morph_mod)
    );
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.lpg),
        std::make_shared<pcontrol_type>(processor_.params_.vca),
        nullptr,
        nullptr);


    outVu_.init("Out");
    float outL, outR;
    processor_.getRMS(outL, outR);
    outVu_.level(outL, outR);

    addAndMakeVisible(outVu_);

    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    float outL, outR;
    processor_.getRMS(outL, outR);
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
    x += vuW + sp;
    outVu_.setBounds(x, y, vuW, h);
}


