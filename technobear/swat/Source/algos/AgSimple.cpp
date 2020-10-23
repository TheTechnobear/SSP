#include "AgSimple.h"
#include "Algos.h"


#include "../JuceLibraryCode/JuceHeader.h"



// Algos ////////////////////////////////////////////////////////////////////////

// "A = Constant A + X\n"
// "B = Constant B + Y\n"
void AgConstant::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {
    A_ = params_[0]->floatVal();
    B_ = params_[1]->floatVal();

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

///////////////////////////////////////////////////////////////////////////////


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

// void AgPrecAdder::encoder(unsigned enc, int dir) {
//     Algo::encoder(enc, dir);
// }

// void AgPrecAdder::button(unsigned btn, bool state) {
//     Algo::button(btn, state);
// }


// void AgPrecAdder::encswitch(unsigned enc, bool state) {
//     Algo::encswitch(btn, state);
// }

///////////////////////////////////////////////////////////////////////////////



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

///////////////////////////////////////////////////////////////////////////////



// "A = Z > 1 , X else Y\n"
// "B = Z > 1 , Y else Z\n"
// "Z switch"
void AgSwitch::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    bool gate = true;
    if (z != nullptr) gate = z[0];

    if (a != nullptr) {
        if (gate) {
            if (x) FloatVectorOperations::copy(a, x, n);
            else FloatVectorOperations::fill(a, 0.0, n);
        } else {
            if (y) FloatVectorOperations::copy(a, y, n);
            else FloatVectorOperations::fill(a, 0.0, n);
        }

        lastA_ = a[0];
    }

    if (b != nullptr) {
        if (!gate) {
            if (x) FloatVectorOperations::copy(b, x, n);
            else FloatVectorOperations::fill(b, 0.0, n);
        } else {
            if (y) FloatVectorOperations::copy(b, y, n);
            else FloatVectorOperations::fill(b, 0.0, n);
        }

        lastB_ = b[0];
    }
}

///////////////////////////////////////////////////////////////////////////////

// "A = X == Y , 1  else 0\n"
// "B = X != Y , 1  else 0\n"
// "Z hold"
void AgEqual::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    bool gate = true;
    float x0 = 0.0f, y0 = 0.0f;
    if (z != nullptr) gate = z[0];

    if (x != nullptr) x0 = x[0];
    if (y != nullptr) y0 = y[0];

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
