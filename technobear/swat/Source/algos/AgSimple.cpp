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
        lastA_ = a[0];
    }


    if (b != nullptr) {
        if (y) FloatVectorOperations::copy(b, y, n);
        else FloatVectorOperations::fill(b, 0.0f, n);
        FloatVectorOperations::add(b, B_, n);
        lastB_ = b[0];
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
    for (auto i = 0; i < n; i++) {
        if (z != nullptr) gate = z[i];
        if (gate) {
            float xi = ( x == nullptr ?  0.0f : x[i]);
            float yi = ( y == nullptr ?  0.0f : y[i]);

            if (a != nullptr) a[i] = std::min(xi, yi);
            if (b != nullptr) b[i] = std::max(xi, yi);

        } else {
            if (a != nullptr) a[i] = lastA_;
            if (b != nullptr) b[i] = lastB_;
        }
    }

    if (a != nullptr) lastA_ = a[0];
    if (b != nullptr) lastB_ = b[0];

    // control rate variation
    // bool gate = true;
    // float x0 = 0.0f, y0 = 0.0f;
    // if (z != nullptr) gate = z[0];

    // if (x != nullptr) x0 = x[0];
    // if (y != nullptr) y0 = y[0];

    // if (gate) {
    //     lastA_ = std::min(x0, y0);
    //     lastB_ = std::max(x0, y0);
    // }

    // if (a != nullptr) {
    //     FloatVectorOperations::fill(a, lastA_ , n);
    // }

    // if (b != nullptr) {
    //     FloatVectorOperations::fill(b, lastB_ , n);
    // }
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

template<typename T> bool comparator(bool TS, T S1, T Th, T Hy ) {
    return TS ? S1 >= (Th - Hy) : S1 >= Th;
}

// "A = gate X > Y\n"
// "B = ! A\n"
// "Z Hysterisis"
void AgComparator::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    if (a != nullptr) {
        for (auto i = 0; i < n; i++) {
            float TS = lastTS_;
            float S1 = x != nullptr ? x[i] : 0.0f;
            float T = y != nullptr ? y[i] : 0.0f;
            float H = z != nullptr ? z[i] : 0.0f;

            // if(i==0) Logger::writeToLog("comparator " + String(S1) + ">" +String(T) + " " + String(comparator(S0,S1,T,H)));
            // if(i==0) Logger::writeToLog("comparator " + String(S1) + ">" +String(T) + " " + String(S1>T));

            a[i] = comparator<float>(TS, S1, T, H);
            lastTS_ = a[i] > 0.5f;
            // lastX_ = S1;
            if (b != nullptr) {
                b[i] = ! a[i];
            }
        }
    } else if (b != nullptr) {
        FloatVectorOperations::fill(b, 0.0f , n);
    }

    lastA_ = a != nullptr ? a[0] : 0.0f;
    lastB_ = b != nullptr ? b[0] : 0.0f;
}



///////////////////////////////////////////////////////////////////////////////
// "A = gate X > (L-Y) & X < (H+Y)\n"
// "B = ! A\n"
// "Z Hysterisis"


void AgComparatorW::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    LOW_  = params_[0]->floatVal();
    HIGH_  = params_[1]->floatVal();
    H_  = params_[2]->floatVal();

    if (a != nullptr) {
        for (auto i = 0; i < n; i++) {
            float S1 = x != nullptr ? x[i] : 0.0f;
            float HY = z != nullptr ? z[i] + H_ : (float) H_;
            float TS = lastTS_;

            // high
            float HT = y != nullptr ? LOW_ + y[i] : (float) LOW_;
            bool Ha = comparator<float>(TS, S1, HT, HY);

            // low
            float LT = y != nullptr ? HIGH_ - y[i] : (float) HIGH_;
            bool La = comparator<float>(TS, LT, S1, HY);

            a[i] = Ha && La;

            lastTS_ = a[i] > 0.5f;
            if (b != nullptr) {
                b[i] = ! a[i];
            }

        }
    } else if (b != nullptr) {
        FloatVectorOperations::fill(b, 0.0f , n);
    }

    lastA_ = a != nullptr ? a[0] : 0.0f;
    lastB_ = b != nullptr ? b[0] : 0.0f;
}

void AgComparatorW::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Low: " + String(LOW_), x, y);
    y += space;
    g.drawSingleLineText("High : " + String(HIGH_), x, y);
    y += space;
    g.drawSingleLineText("Hysterisis : " + String(H_), x, y);

    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
    // g.drawSingleLineText("A : " + String::formatted("%4.2f", A), x, y);
}



///////////////////////////////////////////////////////////////////////////////

// "A = X : in range -> out range\n"
// "B = Y : in range -> out range\n"
void AgMapVV::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    minIn_  = params_[0]->floatVal();
    maxIn_  = params_[1]->floatVal();
    minOut_ = params_[2]->floatVal();
    maxOut_ = params_[3]->floatVal();

    float scale  =  (maxOut_ - minOut_) / (maxIn_ - minIn_);
    float offset = minOut_ - (minIn_ * scale) ;

    if (a != nullptr) {
        if (x)  {
            FloatVectorOperations::copy(a, x, n);
            FloatVectorOperations::multiply(a, scale, n);
            FloatVectorOperations::add(a, offset, n);
            for (auto i = 0; i < n; i++) a[i] = constrain(a[i], minOut_, maxOut_);
        }
        else  {
            FloatVectorOperations::fill(a, minOut_, n);
        }
        lastA_ = a[0];
    }

    if (b != nullptr) {
        if (y)  {
            FloatVectorOperations::copy(b, y, n);
            FloatVectorOperations::multiply(b, scale, n);
            FloatVectorOperations::add(b, offset, n);
            for (auto i = 0; i < n; i++) b[i] = constrain(b[i], minOut_, maxOut_);
        }
        else  {
            FloatVectorOperations::fill(b, minOut_, n);
        }
        lastB_ = b[0];
    }
}

void AgMapVV::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Min In : " + String(minIn_), x, y);
    y += space;
    g.drawSingleLineText("Max In : " + String(maxIn_), x, y);
    y += space * 2;
    g.drawSingleLineText("Min Out : " + String(minOut_), x, y);
    y += space;
    g.drawSingleLineText("Max Out : " + String(maxOut_), x, y);

    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
    // g.drawSingleLineText("A : " + String::formatted("%4.2f", A), x, y);
}

