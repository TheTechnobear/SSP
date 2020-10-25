#pragma once

#include <atomic>

// Simple algorithms

#include "../Algo.h"


// algos
class AgConstant : public Algo {
public:
    AgConstant() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("A", "Constant", 1.0f, -1.0f, 1.0f, 0.005f));
        params_.push_back(std::make_shared<AgFloatParam>("B", "Constant", 0.5f, -1.0f, 1.0f, 0.01f));
        A_ = params_[0]->floatVal();
        B_ = params_[1]->floatVal();
    }

    unsigned type() override { return A_CONSTANT;}
    std::string name() override { return "Constant"; }
    std::string description() override {
        return
            "A = Constant A + X\n"
            "B = Constant B + Y\n"
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
    std::string description() override {
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
    std::string description() override {
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
    std::string description() override {
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


class AgComparator : public Algo {
public:
    AgComparator() {
        lastTS_ = false;
        lastA_ = lastB_ = 0.0f;
    }

    unsigned type() override { return A_COMPARATOR;}
    std::string name() override { return "Comparator"; }
    std::string description() override {
        return
            "A = gate X > Y\n"
            "B = ! A\n"
            "Z Hysterisis"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override {
        Algo::paint(g);
        drawAB(g, lastA_, lastB_);
    }

private:
    std::atomic<bool>  lastTS_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


class AgComparatorW : public Algo {
public:
    AgComparatorW() {
        lastTS_ = false;
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("Low", "Threshold", -0.7f, -1.0f, 1.0f, 0.01f));
        params_.push_back(std::make_shared<AgFloatParam>("High", "Threshold", 0.7f, -1.0f, 1.0f, 0.01f));
        params_.push_back(std::make_shared<AgFloatParam>("H",  "Hysterisis", 0.001f, -1.0f, 1.0f, 0.001f));
        LOW_  = params_[0]->floatVal();
        HIGH_  = params_[1]->floatVal();
        H_  = params_[2]->floatVal();
    }

    unsigned type() override { return A_COMPARATOR_W;}
    std::string name() override { return "Windowed Comparator"; }
    std::string description() override {
        return
            "A = gate X > (L-Y) & X < (H+Y)\n"
            "B = ! A\n"
            "Z Hysterisis"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;
private:
    std::atomic<float> H_;
    std::atomic<float> LOW_;
    std::atomic<float> HIGH_;

    std::atomic<bool>  lastTS_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};



class AgMapVV : public Algo {
public:
    AgMapVV() {
        lastA_ = lastB_ = 0.0f;
        params_.push_back(std::make_shared<AgFloatParam>("MinIn",   "", -1.0f, -1.0f, 1.0f, 0.01f));
        params_.push_back(std::make_shared<AgFloatParam>("MaxIn",   "", 1.0f, -1.0f, 1.0f, 0.01f));
        params_.push_back(std::make_shared<AgFloatParam>("MinOut",  "", 0.0f, -1.0f, 1.0f, 0.01f));
        params_.push_back(std::make_shared<AgFloatParam>("MaxOut",  "", 1.0f, -1.0f, 1.0f, 0.01f));
        minIn_  = params_[0]->floatVal();
        maxIn_  = params_[1]->floatVal();
        minOut_ = params_[2]->floatVal();
        maxOut_ = params_[3]->floatVal();
    }

    unsigned type() override { return A_MAP_VV;}
    std::string name() override { return "Map range v to v"; }
    std::string description() override {
        return
            "A = X : in range to out range\n"
            "B = Y : in range to out range\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;

private:
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
    std::atomic<float> minIn_;
    std::atomic<float> maxIn_;
    std::atomic<float> minOut_;
    std::atomic<float> maxOut_;
};

