#include "Quantizer.h"

#include <stdint.h>
#include <assert.h>

#include "Scales.h"

static const char tonics[MAX_TONICS][3] = {
    "C ",
    "C#",
    "D",
    "D#",
    "E ",
    "F ",
    "F#",
    "G",
    "G#",
    "A ",
    "A#",
    "B",
};


const char *Quantizer::getScaleName(unsigned i) { if (i < MAX_SCALES) return scales[i].name; else return "unknown"; }

const char *Quantizer::getTonicName(unsigned i) { if (i < MAX_TONICS) return tonics[i]; else return "unknown"; }

void Quantizer::quantize(unsigned root, unsigned scale, int &oct, unsigned &semi) {
    assert(semi <= MAX_TONICS);
    assert(root <= MAX_TONICS);

    unsigned iv = ((semi + MAX_TONICS) - root) % 12;

    uint16_t scalemask = scales[scale].scale;

    for (unsigned offset = 0; offset < MAX_TONICS; offset++) {

        unsigned niv = (iv + offset) % 12;
        unsigned mask = 1 << niv;
        if (scalemask & mask) {
            // found note
            semi = semi + offset;
            oct += (semi > 12);
            semi = semi % 12;
            return;
        }
    }

    assert(false); // should never happen since all scales include root!

    // if invalid scales, then force to root;
    oct += (semi > root);
    semi = root;
}
