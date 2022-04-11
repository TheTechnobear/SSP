#pragma once

class Clock {
public:

    bool trigTick() {
        currentTrig_++;
        bool res = currentTrig_ >= targetTrig;
        if (res) resetTrig();
        return res;
    }


    void reset() {
        resetTrig();
        resetSample();
    }

    bool sampleTick() {
        currentSmp_++;
        bool res = currentSmp_ >= targetSmp_;
        if (res) resetSample();
        return res;
    }

    void useTrigs(bool b) { useTrigs_ = b; }

    bool useTrigs() { return useTrigs_; }

    void targetSamples(float samples) {
        baseSmp_ = samples;
        multiplier(multiplier_);
    }

    void targetTrigs(float trigs) {
        baseSmp_ = trigs;
        multiplier(multiplier_);
    }

    float multiplier() { return multiplier_; }

    void multiplier(float m) {
        multiplier_ = m;
        targetSmp_ = baseSmp_ * multiplier_;
        targetTrig = baseTrig_ * multiplier_;
    }


private:
    void resetSample() {
        currentSmp_ = 0.0f;
    }
    void resetTrig() {
        currentTrig_ = 0.0f;
    }

    float multiplier_ = 1.0f;
    bool useTrigs_ = false;

    float currentTrig_ = 0;
    float targetTrig = 0;
    float baseTrig_ = 0.0f;

    float currentSmp_ = 0.0f;
    float targetSmp_ = 0.0f;
    float baseSmp_ = 0.0f;
};