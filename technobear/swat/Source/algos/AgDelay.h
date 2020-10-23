#pragma once

#include <atomic>

#include "../Algo.h"

class AgDelay : public Algo {
public:
    AgDelay() {
        params_.push_back(std::make_shared<AgFloatParam>("Size", "Delay buffer (mSec)",  50.0f, 10.0f, 5000.0f, 5.0f));
        params_.push_back(std::make_shared<AgFloatParam>("Delay", "Time of delay (mSec)", 10.0f, 0.0f, 1000.0f, 0.1f));
        size_ = params_[0]->floatVal();
        delayTime_ = params_[1]->floatVal();
        if (delayTime_ > size_) {
            delayTime_ = (float) size_;
            params_[1]->floatVal(delayTime_);
        }

        delayLineSz_ = getSampleRate() * (size_ / 1000.0f);
        delayLine_ = new float[delayLineSz_];
        memset(delayLine_, 0, sizeof(float) * delayLineSz_);
        writePos_ = 0;
        smpDelay_ = 0;
    }

    unsigned type() override { return A_DELAY;}
    std::string name() override { return "Delay"; }
    std::string description() {
        return
            "X = Signal\n"
            "Y = Delay (% buffer)\n"
            "A = Delayed X ( Delay + Y) \n"
            "B = Delayed X ( Delay + Y) + X\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;

private:
    std::atomic<float> size_;
    std::atomic<float> delayTime_;

    float *delayLine_;
    unsigned delayLineSz_;
    unsigned writePos_;

    unsigned smpDelay_;
};

