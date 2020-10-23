#include "AgLogic.h"
#include "Algos.h"


#include "../JuceLibraryCode/JuceHeader.h"


// Algos ////////////////////////////////////////////////////////////////////////



// "A = X AND Y\n"
// "B = ! (X AND Y)\n"
// "Z is gate\n"
void AgLogicAnd::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    bool gate = true;
    bool t = false;

    for (auto i = 0; i < n; i++) {
        if (z != nullptr) gate = z[i];
        if (gate) {
            t =
                (x == nullptr ? true : x[i] > 0.5f)
                &&
                (y == nullptr ? true : y[i] > 0.5f)
                ;
        } else {
            t = lastA_;
        }

        if (a != nullptr) a[i] = t;
        if (b != nullptr) b[i] = !t;
    }

    if (a != nullptr) lastA_ = a[0];
    if (b != nullptr) lastB_ = b[0];
}


///////////////////////////////////////////////////////////////////////////////

// "A = X OR Y\n"
// "B = ! (X OR Y)\n"
// "Z is gate\n"
void AgLogicOr::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    bool gate = true;
    bool t = false;

    for (auto i = 0; i < n; i++) {
        if (z != nullptr) gate = z[i];
        if (gate) {
            t =
                (x == nullptr ? false : x[i] > 0.5f)
                ||
                (y == nullptr ? false : y[i] > 0.5f)
                ;
        } else {
            t = lastA_;
        }

        if (a != nullptr) a[i] = t;
        if (b != nullptr) b[i] = !t;
    }

    if (a != nullptr) lastA_ = a[0];
    if (b != nullptr) lastB_ = b[0];
}


///////////////////////////////////////////////////////////////////////////////

// "A = X XOR Y\n"
// "B = ! (X XOR Y)\n"
// "Z is gate\n"
void AgLogicXor::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    bool gate = true;
    bool t = false;

    for (auto i = 0; i < n; i++) {
        if (z != nullptr) gate = z[i];
        if (gate) {
            t =
                ! (x == nullptr ? false : x[i] > 0.5f)
                !=
                ! (y == nullptr ? false : y[i] > 0.5f)
                ;
        } else {
            t = lastA_;
        }

        if (a != nullptr) a[i] = t;
        if (b != nullptr) b[i] = !t;
    }

    if (a != nullptr) lastA_ = a[0];
    if (b != nullptr) lastB_ = b[0];
}

