#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

//using pcontrol_type = ssp::SimpleParamControl;
//using pcontrol_type = ssp::LineParamControl;
using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.position),
        std::make_shared<pcontrol_type>(processor_.params_.size),
        std::make_shared<pcontrol_type>(processor_.params_.density),
        std::make_shared<pcontrol_type>(processor_.params_.texture)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.mix),
        std::make_shared<pcontrol_type>(processor_.params_.spread),
        std::make_shared<pcontrol_type>(processor_.params_.feedback),
        std::make_shared<pcontrol_type>(processor_.params_.reverb)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.pitch),
        std::make_shared<pcontrol_type>(processor_.params_.mode, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.in_gain),
        nullptr
    );


    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.freeze, 24, Colours::lightskyblue),
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

void PluginEditor::onEncoder(unsigned enc, float v) {
    base_type::onEncoder(enc, v);
}

void PluginEditor::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
}


void PluginEditor::onButton(unsigned int btn, bool v) {
    base_type::onButton(btn, v);
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
