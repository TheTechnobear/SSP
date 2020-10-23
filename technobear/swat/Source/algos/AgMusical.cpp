#include "AgMusical.h"
#include "Algos.h"


#include "../JuceLibraryCode/JuceHeader.h"


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



// Algos ////////////////////////////////////////////////////////////////////////

// "A = Note/Oct A + X\n"
// "B = Note/Oct B + Y\n"
void AgConstantN::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    NA_ = params_[0]->floatVal();
    OA_ = params_[1]->floatVal();
    NB_ = params_[2]->floatVal();
    OB_ = params_[3]->floatVal();

    float A_ = pitch2Cv((12.0f * OA_ + NA_) - 60.0f);
    float B_ = pitch2Cv((12.0f * OB_ + NB_) - 60.0f);

    if (a != nullptr) {
        if (x) FloatVectorOperations::copy(a, x, n);
        else FloatVectorOperations::fill(a, 0.0f, n);
        FloatVectorOperations::add(a, A_, n);
        lastA_=a[0];
    }


    if (b != nullptr) {
        if (y) FloatVectorOperations::copy(b, y, n);
        else FloatVectorOperations::fill(b, 0.0f, n);
        FloatVectorOperations::add(b, B_, n);
        lastB_=b[0];
    }
}


void AgConstantN::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Note A : " + String(tonics[unsigned(NA_)])+String(unsigned(OA_)), x, y);
    y += space * 2;
    g.drawSingleLineText("Note B : " + String(tonics[unsigned(NB_)])+String(unsigned(OB_)), x, y);



    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space * 2;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}


///////////////////////////////////////////////////////////////////////////////

// "A = X == Note/Oct + Y , 1  else 0\n"
// "B = X != Note/Oct + Y , 1  else 0\n"
// "Z hold"
void AgEqualN::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    N_ = params_[0]->floatVal();
    O_ = params_[1]->floatVal();

    float pcv = pitch2Cv((12.0f * O_ + N_) - 60.0f);

    bool gate = true;
    float x0 = 0.0f, y0 = pcv;
    if (z != nullptr) gate = z[0];

    if (x != nullptr) x0 = x[0];
    if (y != nullptr) y0 = pcv + y[0] ;

    if (gate) {
        lastA_ = x0 == y0;
        lastB_ = x0 != y0;
    }

    if (a != nullptr) {
        FloatVectorOperations::fill(a, lastA_ , n);
    }

    if (b != nullptr) {
        FloatVectorOperations::fill(b, lastB_ , n);
    }
}


void AgEqualN::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Note : " + String(tonics[unsigned(N_)])+String(unsigned(O_)), x, y);

    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space * 2;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}

