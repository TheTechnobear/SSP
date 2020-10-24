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



String noteString(int f) {
    int voct = f + 60;
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    return String(tonics[note]) + String(oct);
}


// Algos ////////////////////////////////////////////////////////////////////////

// "A = X + Note A\n"
// "B = Y + Note B\n"
void AgTranspose::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    NA_ = params_[0]->floatVal();
    NB_ = params_[1]->floatVal();

    if (a != nullptr) {
        if (x) FloatVectorOperations::copy(a, x, n);
        else FloatVectorOperations::fill(a, 0.0f, n);
        FloatVectorOperations::add(a, pitch2Cv(NA_), n);
        lastA_ = a[0];
    }


    if (b != nullptr) {
        if (y) FloatVectorOperations::copy(b, y, n);
        else FloatVectorOperations::fill(b, 0.0f, n);
        FloatVectorOperations::add(b, pitch2Cv(NB_), n);
        lastB_ = b[0];
    }
}


void AgTranspose::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Note A : " + String(noteString(NA_)), x, y);
    y += space;
    g.drawSingleLineText("Note B : " + String(noteString(NB_)), x, y);

    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}


///////////////////////////////////////////////////////////////////////////////

// "A = X == Note + Y , 1  else 0\n"
// "B = X != Note + Y , 1  else 0\n"
// "Z hold"
void AgEqualN::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    N_ = params_[0]->floatVal();

    bool gate = true;
    float x0 = 0.0f, y0 = N_;
    if (z != nullptr) gate = z[0];

    if (x != nullptr) x0 = x[0];
    if (y != nullptr) y0 = pitch2Cv(N_) + y[0] ;

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

    g.drawSingleLineText("Note : " + noteString(N_), x, y);

    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
}




///////////////////////////////////////////////////////////////////////////////

// "A = X : in range -> out range\n"
// "B = Y : in range -> out range\n"
void AgMapNV::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    minIn_  = params_[0]->floatVal();
    maxIn_  = params_[1]->floatVal();
    minOut_ = params_[2]->floatVal();
    maxOut_ = params_[3]->floatVal();


    float minInCV= pitch2Cv(minIn_);
    float maxInCV= pitch2Cv(maxIn_);

    float scale  =  (maxOut_ - minOut_) / (maxInCV - minInCV);
    float offset = minOut_ - (minInCV * scale) ;

    if (a != nullptr) {
        if (x)  {
            FloatVectorOperations::copy(a, x, n);
            FloatVectorOperations::multiply(a, scale, n);
            FloatVectorOperations::add(a, offset, n);
            for(auto i=0;i < n;i++) a[i]=constrain(a[i],minOut_,maxOut_);
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
            for(auto i=0;i < n;i++) b[i]=constrain(b[i],minOut_,maxOut_);
        }
        else  {
            FloatVectorOperations::fill(b, minOut_, n);
        }
        lastB_ = b[0];
    }
}


void AgMapNV::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Min In : " + noteString(minIn_), x, y);
    y += space;
    g.drawSingleLineText("Max In : " + noteString(maxIn_), x, y);
    y += space * 2;
    g.drawSingleLineText("Min Out : " + String(minOut_), x, y);
    y += space;
    g.drawSingleLineText("Max Out : " + String(maxOut_), x, y);

    y += space * 4;
    g.drawSingleLineText("A : " + String(lastA_), x, y);
    y += space;
    g.drawSingleLineText("B : " + String(lastB_), x, y);
    // g.drawSingleLineText("A : " + String::formatted("%4.2f", lastA_), x, y);
}


///////////////////////////////////////////////////////////////////////////////

// "A = X : in range -> out range\n"
// "B = Y : in range -> out range\n"
void AgMapNN::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    minIn_  = params_[0]->floatVal();
    maxIn_  = params_[1]->floatVal();
    minOut_ = params_[2]->floatVal();
    maxOut_ = params_[3]->floatVal();

    float minInCV= pitch2Cv(minIn_);
    float maxInCV= pitch2Cv(maxIn_);
    float minOutCV= pitch2Cv(minOut_);
    float maxOutCV= pitch2Cv(maxOut_);

    float scale  =  (maxOutCV - minOutCV) / (maxInCV - minInCV);
    float offset = minOutCV - (minInCV * scale) ;

    if (a != nullptr) {
        if (x)  {
            FloatVectorOperations::copy(a, x, n);
            FloatVectorOperations::multiply(a, scale, n);
            FloatVectorOperations::add(a, offset, n);
            for(auto i=0;i < n;i++) a[i]=constrain(a[i],minOutCV,maxOutCV);
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
            for(auto i=0;i < n;i++) b[i]=constrain(b[i],minOutCV,maxOutCV);
        }
        else  {
            FloatVectorOperations::fill(b, minOut_, n);
        }
        lastB_ = b[0];
    }
}


void AgMapNN::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Min In : " + noteString(minIn_), x, y);
    y += space;
    g.drawSingleLineText("Max In : " + noteString(maxIn_), x, y);
    y += space * 2;
    g.drawSingleLineText("Min Out : " + noteString(minOut_), x, y);
    y += space;
    g.drawSingleLineText("Max Out : " + noteString(maxOut_), x, y);
    y += space * 4;


    g.drawSingleLineText("A : " + noteString(lastA_), x, y);
    y += space;
    g.drawSingleLineText("B : " + noteString(lastB_), x, y);
}




