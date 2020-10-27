#pragma once

#include <atomic>

#include "../Algo.h"

class AgLogicAnd : public Algo {
public:
    AgLogicAnd() {
        lastA_ = lastB_ = false;
    }

    // general
    unsigned type() override { return A_LOGIC_AND;}
    std::string name() override { return "Logic - AND"; }
    std::string description() override {
        return
            "A = X AND Y\n"
            "B = ! (X AND Y)\n"
            "Z is gate\n"
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

private:
    std::atomic<bool> lastA_;
    std::atomic<bool> lastB_;
};



class AgLogicOr : public Algo {
public:
    AgLogicOr() {
        lastA_ = lastB_ = false;
    }

    // general
    unsigned type() override { return A_LOGIC_OR;}
    std::string name() override { return "Logic - OR"; }
    std::string description() override {
        return
            "A = X OR Y\n"
            "B = ! (X OR Y)\n"
            "Z is gate\n"
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

private:
    std::atomic<bool> lastA_;
    std::atomic<bool> lastB_;
};


class AgLogicXor : public Algo {
public:
    AgLogicXor() {
        lastA_ = lastB_ = false;
    }

    // general
    unsigned type() override { return A_LOGIC_XOR;}
    std::string name() override { return "Logic - XOR"; }
    std::string description() override {
        return
            "A = X XOR Y\n"
            "B = ! (X XOR Y)\n"
            "Z is gate\n"
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

private:
    std::atomic<bool> lastA_;
    std::atomic<bool> lastB_;
};


