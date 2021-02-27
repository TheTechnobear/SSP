
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
        std::make_shared<pcontrol_type>(processor_.params_.structure),
        std::make_shared<pcontrol_type>(processor_.params_.brightness),
        std::make_shared<pcontrol_type>(processor_.params_.damping)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.position),
        std::make_shared<pcontrol_type>(processor_.params_.polyphony, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.model, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.in_gain)
    );


    inVu_.init("In");
    outVu_.init("Out");

    float in, outL, outR;
    processor_.getRMS(in, outL, outR);
    inVu_.level(in);
    outVu_.level(outL, outR);

    addAndMakeVisible(inVu_);
    addAndMakeVisible(outVu_);

    setSize(1600, 480);
}

PluginEditor::~PluginEditor() {
}


void PluginEditor::paint(Graphics &g) {
    float in, outL, outR;
    processor_.getRMS(in, outL, outR);
    inVu_.level(in);
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
    inVu_.setBounds(x, y, vuW, h);
    x += vuW + sp;
    outVu_.setBounds(x, y, vuW, h);
}
