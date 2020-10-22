#pragma once

#include <atomic>

// Simple algorithms

#include "../Algo.h"


// algos
class AgConstant : public Algo {
public:
    AgConstant() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("A", "Constant 1", 1.0f, -1.0f, 1.0f, 0.005f));
        params_.push_back(std::make_shared<AgFloatParam>("B", "Constant 2", 0.5f, -1.0f, 1.0f, 0.01f));
        A_ = params_[0]->floatVal();
        B_ = params_[1]->floatVal();
    }

    unsigned type() override { return A_CONSTANT;}
    std::string name() override { return "Constant"; }
    std::string description() {
        return
            "A = Constant 1\n"
            "B = Constant 2\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override {
        Algo::paint(g);
        drawAB(g, lastA_, lastB_);
    }

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
    }

    // general
    unsigned type() override { return A_P_ADDER;}
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
    void paint (Graphics& g) override {
        Algo::paint(g);
        drawAB(g, lastA_, lastB_);
    }
    // void encoder(unsigned enc, int dir) override;
    // void button(unsigned btn, bool state) override;
    // void encswitch(unsigned btn, bool state) override;
    // void drawHelp() override;

private:
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


class AgMinMax : public Algo {
public:
    AgMinMax() {
        lastA_ = lastB_ = 0.0f;
    }

    // general
    unsigned type() override { return A_MIN_MAX;}
    std::string name() override { return "Min / Max"; }
    std::string description() {
        return
            "A = min(X,Y)\n"
            "B = max(X,Y)\n"
            "Z gate"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override {
        Algo::paint(g);
        drawAB(g, lastA_, lastB_);
    }
private:
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


class AgSwitch : public Algo {
public:
    AgSwitch() {
        lastA_ = lastB_ = 0.0f;
    }

    unsigned type() override { return A_SWITCH;}
    std::string name() override { return "Switch"; }
    std::string description() {
        return
            "A = Z > 1 , X else Y\n"
            "B = Z > 1 , Y else Z\n"
            "Z switch"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override {
        Algo::paint(g);
        drawAB(g, lastA_, lastB_);
    }

private:
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};



class AgEqual : public Algo {
public:
    AgEqual() {
        lastA_ = lastB_ = 0.0f;
    }

    unsigned type() override { return A_EQUAL;}
    std::string name() override { return "Equal"; }
    std::string description() {
        return
            "A = X == Y , 1  else 0\n"
            "B = X != Y , 1  else 0\n"
            "Z hold"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override {
        Algo::paint(g);
        drawAB(g, lastA_, lastB_);
    }

private:
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


