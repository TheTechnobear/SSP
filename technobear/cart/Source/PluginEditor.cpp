
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;
//static constexpr unsigned MAX_VIEW = MAX_LAYERS + 1;
static constexpr unsigned MAX_VIEW = MAX_LAYERS;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW),
      processor_(p),
      clrs_{Colours::red, Colours::green, Colours::orange, Colours::cyan} {

    unsigned view = 0;

    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[view];
        for (unsigned i = 0; i < 16 / 4; i++) {
            int pi = i * 4;
            addParamPage(
                std::make_shared<pcontrol_type>(l->steps_[pi + 0]->cv, 0.25),
                std::make_shared<pcontrol_type>(l->steps_[pi + 1]->cv, 0.25),
                std::make_shared<pcontrol_type>(l->steps_[pi + 2]->cv, 0.25),
                std::make_shared<pcontrol_type>(l->steps_[pi + 3]->cv, 0.25),
                view,
                clrs_[view % L_CLRS]
            );

            addButtonPage(
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->gate, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->gate, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->gate, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->gate, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->access, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->access, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->access, 24, clrs_[view]),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->access, 24, clrs_[view]),
                view
            );
        }
        view++;
    }


    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
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
    voct += 0.005f; // so we round up fractions of cent
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    int cents = ((voct - floorf(voct)) * 100.0f);
    if (cents > 50) {
        cents = 50 - cents;
        note = (note + 1) % MAX_TONICS;
        oct += (note == 0);
    }
    String cts = String::formatted("%+02d", cents);
    return String(tonics[note] + String(oct - (note < 3))) + " " + cts;
}
