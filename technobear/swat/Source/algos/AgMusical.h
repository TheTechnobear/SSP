#pragma once

#include <atomic>

// Simple algorithms

#include "../Algo.h"


// algos
class AgTranspose : public Algo {
public:
    AgTranspose() {
        lastA_ = lastB_ = 0.0f;
        int minPitch  = -60;
        int maxPitch  = 60;
        int zeroPitch = 0 ;
        int oneSemi   = 1;
        params_.push_back(std::make_shared<AgIntParam>("NoteA",  "", zeroPitch, minPitch, maxPitch, oneSemi));
        params_.push_back(std::make_shared<AgIntParam>("NoteB",  "", zeroPitch, minPitch, maxPitch, oneSemi));

        NA_ = params_[0]->floatVal();
        NB_ = params_[1]->floatVal();
    }

    unsigned type() override { return A_TRANSPOSE;}
    std::string name() override { return "Transpose"; }
    std::string description() override {
        return
            "A = X + Note A \n"
            "B = Y + Note B\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;

private:
    std::atomic<int> NA_;
    std::atomic<int> NB_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};


class AgEqualN : public Algo {
public:
    AgEqualN() {
        lastA_ = lastB_ = 0.0f;
        int minPitch  = -60;
        int maxPitch  = 60;
        int zeroPitch = 0 ;
        int oneSemi   = 1;
        params_.push_back(std::make_shared<AgIntParam>("Note",  "Reference Note", zeroPitch, minPitch, maxPitch, oneSemi));
        N_ = params_[0]->floatVal();
    }

    unsigned type() override { return A_EQUAL_N;}
    std::string name() override { return "Equal Note"; }
    std::string description() override {
        return
            "A = X == Note + Y , 1  else 0\n"
            "B = X != Note + Y , 1  else 0\n"
            "Z hold"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics& g) override;

private:
    std::atomic<int> N_;
    std::atomic<float> lastA_;
    std::atomic<float> lastB_;
};

class AgMapNV : public Algo {
public:
    AgMapNV() {
        lastA_ = lastB_ = 0.0f;
        int minPitch  = -60;
        int maxPitch  = 60;
        // int zeroPitch = 0 ;
        int oneSemi   = 1;
        params_.push_back(std::make_shared<AgIntParam>("MinIn",   "", minPitch, minPitch, maxPitch, oneSemi));
        params_.push_back(std::make_shared<AgIntParam>("MaxIn",   "", maxPitch, minPitch, maxPitch, oneSemi));
        params_.push_back(std::make_shared<AgFloatParam>("MinOut",  "", 0.0f, -1.0f, 1.0f, 0.01f));
        params_.push_back(std::make_shared<AgFloatParam>("MaxOut",  "", 1.0f, -1.0f, 1.0f, 0.01f));
        minIn_  = params_[0]->floatVal();
        maxIn_  = params_[1]->floatVal();
        minOut_ = params_[2]->floatVal();
        maxOut_ = params_[3]->floatVal();
    }

    unsigned type() override { return A_MAP_NV;}
    std::string name() override { return "Map range note to v"; }
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
    std::atomic<int> minIn_;
    std::atomic<int> maxIn_;
    std::atomic<float> minOut_;
    std::atomic<float> maxOut_;
};


class AgMapNN : public Algo {
public:
    AgMapNN() {
        lastA_ = lastB_ = 0.0f;
        int minPitch  = -60;
        int maxPitch  = 60;
        int zeroPitch = 0 ;
        int oneSemi   = 1;
        params_.push_back(std::make_shared<AgIntParam>("MinIn",   "", minPitch, minPitch, maxPitch, oneSemi));
        params_.push_back(std::make_shared<AgIntParam>("MaxIn",   "", maxPitch, minPitch, maxPitch, oneSemi));
        params_.push_back(std::make_shared<AgIntParam>("MinOut",  "", zeroPitch, minPitch, maxPitch, oneSemi));
        params_.push_back(std::make_shared<AgIntParam>("MaxOut",  "", maxPitch, minPitch, maxPitch, oneSemi));
        minIn_  = params_[0]->floatVal();
        maxIn_  = params_[1]->floatVal();
        minOut_ = params_[2]->floatVal();
        maxOut_ = params_[3]->floatVal();
    }

    unsigned type() override { return A_MAP_NN;}
    std::string name() override { return "Map range note to v"; }
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
    std::atomic<int> minIn_;
    std::atomic<int> maxIn_;
    std::atomic<int> minOut_;
    std::atomic<int> maxOut_;
};


