#include "UIStringFunc.h"

#include "SSP.h"

static constexpr unsigned MAX_TONICS = 12;
static const char tonics[MAX_TONICS][3] = {
    "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B ",
};

juce::String getNoteValue(float f) {
    float voct = cv2Pitch(f) + 60.0f;  // -5v = 0
    voct += 0.005f;                    // so we round up fractions of cent
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    int cents = ((voct - floorf(voct)) * 100.0f);
    if (cents > 50) {
        cents = 50 - cents;
        note = (note + 1) % MAX_TONICS;
        oct += (note == 0);
    }
    juce::String cts = juce::String::formatted("%+02d", cents);
    return juce::String(tonics[note] + juce::String(oct - (note < 3))) + " " + cts;
}
