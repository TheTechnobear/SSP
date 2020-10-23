#pragma once

#include <atomic>

// Simple algorithms

#include "../Algo.h"


// algos
class AgConstantN : public Algo {
public:
    AgConstantN() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("Note A", "", 0.0f, 0.0f, 11.0f, 1.0f));
        params_.push_back(std::make_shared<AgFloatParam>("Oct A", "", 5.0f, 0.0f, 10.0f, 1.0f));
        params_.push_back(std::make_shared<AgFloatParam>("Note B", "", 0.0f, 0.0f, 11.0f, 1.0f));
        params_.push_back(std::make_shared<AgFloatParam>("Oct B", "", 5.0f, 0.0f, 10.0f, 1.0f));
        NA_ = params_[0]->floatVal();
        OA_ = params_[1]->floatVal();
        NB_ = params_[2]->floatVal();
        OB_ = params_[3]->floatVal();
    }

    unsigned type() override { return A_CONSTANT_N;}
    std::string name() override { return "Constant Notes"; }
    std::string description() {
        return
            "A = Note/Oct A + X\n"
            "B = Note/Oct B + Y\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;

private:
    std::atomic<float> NA_, OA_;
    std::atomic<float> NB_, OB_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


class AgEqualN : public Algo {
public:
    AgEqualN() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("Note", "", 0.0f, 0.0f, 11.0f, 1.0f));
        params_.push_back(std::make_shared<AgFloatParam>("Oct", "", 5.0f, 0.0f, 10.0f, 1.0f));
        N_ = params_[0]->floatVal();
        O_ = params_[1]->floatVal();
    }

    unsigned type() override { return A_EQUAL_N;}
    std::string name() override { return "Equal Note"; }
    std::string description() {
        return
            "A = X == Note/Oct + Y , 1  else 0\n"
            "B = X != Note/Oct + Y , 1  else 0\n"
            "Z hold"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;

private:
    std::atomic<float> N_, O_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


