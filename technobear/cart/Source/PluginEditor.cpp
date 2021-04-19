
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

      processor_(p), clrs_{Colours::red, Colours::green, Colours::orange} {

    unsigned layer = 0;
    for (unsigned i = 0; i < 4; i++) {
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 0]->cv, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 1]->cv, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 2]->cv, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 3]->cv, 0.25),
            clrs_[layer]
        );

        addButtonPage(
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 0]->gate, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 1]->gate, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 2]->gate, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 3]->gate, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 0]->access, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 1]->access, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 2]->access, 24, clrs_[layer]),
            std::make_shared<bcontrol_type>(processor_.params_.layers_[layer]->steps_[i * 4 + 3]->access, 24, clrs_[layer])
        );
    }


    setSize(1600, 480);
}

ssp::BaseEditor::ControlPage PluginEditor::addParamPage(
    std::shared_ptr<ssp::BaseParamControl> c1,
    std::shared_ptr<ssp::BaseParamControl> c2,
    std::shared_ptr<ssp::BaseParamControl> c3,
    std::shared_ptr<ssp::BaseParamControl> c4,
    juce::Colour clr
) {
    if (c1) c1->fg(clr);
    if (c2) c2->fg(clr);
    if (c3) c3->fg(clr);
    if (c4) c4->fg(clr);
    return base_type::addParamPage(c1, c2, c3, c4);
}


void PluginEditor::timerCallback() {
    base_type::timerCallback();
}


void PluginEditor::paint(Graphics &g) {
    base_type::paint(g);
}

void PluginEditor::resized() {
}

static constexpr unsigned MAX_TONICS = 12;

static const char tonics[MAX_TONICS][3] = {
    "C ",
    "C#",
    "D ",
    "D#",
    "E ",
    "F ",
    "F#",
    "G ",
    "G#",
    "A ",
    "A#",
    "B ",
};

String getNoteValue(float f) {
    float voct = cv2Pitch(f) + 60.0f; // -5v = 0
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    unsigned cents = (voct - floorf(voct)) * 100;
    if (cents > 50) {
        cents -= 50;
        note = (note + 1) % MAX_TONICS;
    }
    return String(tonics[note] + String(oct - (note < 3))) + "." + String(cents);
}

