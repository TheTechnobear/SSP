#include "AgSimple.h"
#include "Algos.h"


#include "../JuceLibraryCode/JuceHeader.h"

void AgConstant::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {
    A_ = params_[0]->floatVal();
    B_ = params_[1]->floatVal();

    if (a != nullptr) {
        FloatVectorOperations::fill(a, A_, n);
        lastA_ = a[0];
    }


    if (b != nullptr) {
        FloatVectorOperations::fill(b, B_, n);
        lastB_ = b[0];
    }
}

void AgConstant::paint (Graphics& g) {
    Algo::paint(g);

    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space * 2;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}


// "Precision Adder\n"
// "A = X + Y + Z \n"
// "B = X - Y - Z\n"
void AgPrecAdder::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    if (a != nullptr) {
        if (x) FloatVectorOperations::copy(a, x, n);
        else FloatVectorOperations::fill(a, 0.0, n);
        if (y) FloatVectorOperations::add(a, y, n);
        if (z) FloatVectorOperations::add(a, z, n);
        lastA_ = a[0];
    }


    if (b != nullptr) {
        if (x) FloatVectorOperations::copy(b, x, n);
        else FloatVectorOperations::fill(b, 0.0, n);
        if (y) FloatVectorOperations::subtract(b, y, n);
        if (z) FloatVectorOperations::subtract(b, z, n);

        lastB_ = b[0];
    }
}

void AgPrecAdder::paint (Graphics& g) {
    Algo::paint(g);

    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space * 2;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}

// void AgPrecAdder::encoder(unsigned enc, int dir) {
//     Algo::encoder(enc, dir);
// }

// void AgPrecAdder::button(unsigned btn, bool state) {
//     Algo::button(btn, state);
// }


// void AgPrecAdder::encswitch(unsigned enc, bool state) {
//     Algo::encswitch(btn, state);
// }



// "A =min(X,Y)\n"
// "B =max(X,Y)\n"
// "Z gate"
void AgMinMax::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    bool gate = true;
    float x0 = 0.0f, y0 = 0.0f;
    if (z != nullptr) gate = z[0];

    if (x != nullptr) x0 = x[0];
    if (y != nullptr) y0 = y[0];

    if (gate) {
        lastA_ = std::min(x0, y0);
        lastB_ = std::max(x0, y0);
    }

    if (a != nullptr) {
        FloatVectorOperations::fill(a, lastA_ , n);
    }

    if (b != nullptr) {
        FloatVectorOperations::fill(b, lastB_ , n);
    }
}

void AgMinMax::paint (Graphics& g) {
    Algo::paint(g);

    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space * 2;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}



