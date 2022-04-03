#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

//using pcontrol_type = ssp::SimpleParamControl;
//using pcontrol_type = ssp::LineParamControl;
using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.ratio),
        std::make_shared<pcontrol_type>(processor_.params_.threshold),
        std::make_shared<pcontrol_type>(processor_.params_.attack),
        std::make_shared<pcontrol_type>(processor_.params_.release)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.makeup),
        nullptr,
        nullptr,
        nullptr
    );


    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.automakeup, 24, Colours::lightskyblue),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    inVu_.init("In");
    outVu_.init("Out");

    float inL, inR, outL, outR;
    processor_.getRMS(inL, inR, outL, outR);
    inVu_.level(inL, inR);
    outVu_.level(outL, outR);

    addAndMakeVisible(inVu_);
    addAndMakeVisible(outVu_);

    setSize(1600, 480);
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
