#pragma once

#include <atomic>

#include "Algo.h"


class AgPrecAdder : public Algo {
public:
    // audio thread
    //note: x,y,z,a,b will be null(!) if not connected!
    AgPrecAdder() {
        params_.push_back(std::make_shared<AgFloatParam>("Smooth", 0.0f, 0.0f, 1.0f, 0.1f));
    }

    // general
    std::string name() override { return "Precision Adder"; }
    std::string description() {
        return
            "A = X + Y + Z \n"
            "B = X - Y - Z\n"
            ;
    }

    // audio thread
    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;

    // UI thread
    // void paint (Graphics&) override;
    // void encoder(unsigned enc, int dir) override;
    // void button(unsigned btn, bool state) override;
    // void encswitch(unsigned btn, bool state) override;
    // void drawHelp() override;

};
