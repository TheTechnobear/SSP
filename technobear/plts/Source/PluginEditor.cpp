
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/BaseParamControl.h"
#include "ssp/ButtonControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ButtonControl;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p,
                String(JucePlugin_Name) + " : " + String(JucePlugin_Desc),
                JucePlugin_VersionString),
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


void PluginEditor::paint(Graphics &g) {
    float outL, outR;
    processor_.getRMS(outL, outR);
    outVu_.level(outL, outR);

    base_type::paint(g);
}


void PluginEditor::resized() {
    const unsigned h = 150;
    const unsigned sp = 10;
    const unsigned vuStart = 1500;
    const unsigned vuW = 45;
    unsigned y = 50;
    unsigned x = vuStart;
    x += vuW + sp;
    outVu_.setBounds(x, y, vuW, h);
}


