#include "AgDelay.h"
#include "Algos.h"

#include "../JuceLibraryCode/JuceHeader.h"



// Algos ////////////////////////////////////////////////////////////////////////

// "X = Signal\n"
// "Y = Read Pos % \n"
// "A = Delayed X ( Delay + Y) \n"
// "B = Delayed X ( Delay + Y) + X\n"
void AgDelay::process(
    const float* x, const float* y, const float* z,
    float* a, float* b,
    unsigned n) {

    size_ = params_[0]->floatVal();

    unsigned dlSz = getSampleRate() * (size_ / 1000.0f);
    if (delayLineSz_ != dlSz) {

        if(dlSz < n) {
            // delay line must be at least one buffer length
            dlSz = n;
            float minSz = (n * 1000.0f) / getSampleRate();
            size_ = minSz;
            params_[0]->floatVal(size_);
        }

        delayLineSz_ = dlSz;
        delete [] delayLine_;
        delayLine_ = new float[delayLineSz_];
        memset(delayLine_, 0, sizeof(float) * delayLineSz_);
        writePos_ = 0;
    }

    // write delay buffer
    if (x != nullptr) {
        if (writePos_ + n < delayLineSz_ ) {
            // straight copy into buffer
            assert(delayLineSz_ > n); // assume dly line > buffer
            FloatVectorOperations::copy(delayLine_ + writePos_, x, n);
            writePos_ += n;
        } else {
            unsigned n0 = delayLineSz_ - writePos_;
            if (n0 > 0) {
                FloatVectorOperations::copy(delayLine_ + writePos_, x, n0);
            }
            unsigned n1 = n - n0;
            if (n1 > 0) {
                FloatVectorOperations::copy(delayLine_ , x + n0, n1);
            }
            writePos_ = n1;
        }
    }


    delayTime_ = params_[1]->floatVal();
    if (delayTime_ > size_) {
        delayTime_ = (float) size_;
        params_[1]->floatVal(delayTime_);
    }


    // output delay signal
    float y0 = 0.0f;
    if (y != nullptr) y0 = constrain(y[0], -1.0f, 1.0f);
    int  dls = (getSampleRate() * (delayTime_ / 1000.0f))  +   (y0 * delayLineSz_);
    dls = dls < 0 ? 0 : dls;
    smpDelay_ = dls % delayLineSz_;

    unsigned readPos = (writePos_ - smpDelay_ + delayLineSz_) % delayLineSz_;


    if (a != nullptr) {
        if (delayLineSz_ - readPos > n) {
            // straight copy into buffer
            FloatVectorOperations::copy(a, delayLine_ + readPos, n);
        } else {
            unsigned n0 = delayLineSz_ - readPos;
            if (n0 > 0) {
                FloatVectorOperations::copy(a, delayLine_ + readPos, n0);
            }
            unsigned n1 = n - n0;
            if (n1 > 0) {
                FloatVectorOperations::copy(a, delayLine_ , n1);
            }
        }
    }


    if (b != nullptr) {
        if (delayLineSz_ - readPos > n) {
            // straight copy into buffer
            FloatVectorOperations::copy(b, delayLine_ + readPos, n);
        } else {
            unsigned n0 = delayLineSz_ - readPos;
            if (n0 > 0) {
                FloatVectorOperations::copy(b, delayLine_ + readPos, n0);
            }
            unsigned n1 = n - n0;
            if (n1 > 0) {
                FloatVectorOperations::copy(b, delayLine_ , n1);
            }
        }
        if (x) FloatVectorOperations::add(b, x, n);
    }
}

void AgDelay::paint (Graphics& g) {
    Algo::paint(g);
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Max Time : " + String(size_), x, y);
    y += space * 2;
    g.drawSingleLineText("Delay Time : " + String(delayTime_), x, y);

    y += space * 4;
    g.drawSingleLineText("Delay(smps) : " + String(smpDelay_), x, y);
    y += space * 2;
    float timeDelay = (smpDelay_ * 1000.0f) / getSampleRate();
    g.drawSingleLineText("Delay(mSec) : " + String::formatted("%4.2f", timeDelay), x, y);
}