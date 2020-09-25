#pragma once

#include <atomic>

// Simple algorithms

#include "Algo.h"

class AgConstant : public Algo {
public:
    AgConstant() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("A", "Constant 1", 1.0f, 0.0f, 1.0f, 0.1f));
        params_.push_back(std::make_shared<AgFloatParam>("B", "Constant 2", 0.5f, 0.0f, 1.0f, 0.1f));
        A_=params_[0]->floatVal();
        B_=params_[1]->floatVal();
    }

    std::string name() override { return "Constant"; }
    std::string description() {
        return
            "A = Parameter A\n"
            "B = Parameter B\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics&) override;

private:
    std::atomic<float> A_;
    std::atomic<float> B_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};



class AgPrecAdder : public Algo {
public:
    AgPrecAdder() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("Smooth", "Smoothing of Z", 0.0f, 0.0f, 1.0f, 0.1f));
    }

    // general
    std::string name() override { return "Precision Adder"; }
    std::string description() {
        return
            "A = X + Y + Z\n"
            "B = X - Y - Z\n"
            ;
    }

    // audio thread
    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;

    // UI thread
    void paint (Graphics&) override;
    // void encoder(unsigned enc, int dir) override;
    // void button(unsigned btn, bool state) override;
    // void encswitch(unsigned btn, bool state) override;
    // void drawHelp() override;

private:
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};
