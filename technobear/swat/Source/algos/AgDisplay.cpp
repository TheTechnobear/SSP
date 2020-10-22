#include "AgDisplay.h"
#include "Algos.h"

#include "../JuceLibraryCode/JuceHeader.h"

void AgDisplay::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    if (x != nullptr) lastX_ = x[0];
    if (y != nullptr) lastY_ = y[0];
    if (z != nullptr) lastZ_ = z[0];


    if (a != nullptr) {
        FloatVectorOperations::fill(a, 0.0f, n);
    }


    if (b != nullptr) {
        FloatVectorOperations::fill(b, 0.0f, n);
    }
}


static constexpr unsigned MAX_TONICS = 12;

static const char tonics[MAX_TONICS][3] = {
    "A",
    "A#",
    "B",
    "C",
    "C#",
    "D",
    "D#",
    "E",
    "F",
    "F#",
    "G",
    "G#"
};

String AgDisplay::getNoteValue(float f) {
    float voct = cv2Pitch(f) + 60.0f; // -5v = 0
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    unsigned cents = (voct - floorf(voct)) * 100;
    return String(tonics[note] + String(oct)) + " " + String(cents) + " cts";
}

void AgDisplay::paint (Graphics& g) {
    Algo::paint(g);

    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;

    float dx = lastX_, dy = lastY_, dz = lastZ_;

    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.drawSingleLineText("X : " + String::formatted("%5.3f", dx) + "  -  " + String::formatted("%4.2f", dx * 5.0f) + "V - " + getNoteValue(dx), x, y);
    y += space * 2;
    g.drawSingleLineText("Y : " + String::formatted("%5.3f", dy) + "  -  " + String::formatted("%4.2f", dy * 5.0f) + "V - " + getNoteValue(dy), x, y);
    y += space * 2;
    g.drawSingleLineText("Z : " + String::formatted("%5.3f", dz) + "  -  " + String::formatted("%4.2f", dz * 5.0f) + "V - " + getNoteValue(dz), x, y);
}
