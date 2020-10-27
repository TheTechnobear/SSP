#pragma once

#include <atomic>

// Display algos

#include "../Algo.h"

class AgDisplay : public Algo {
public:
    AgDisplay() {
        lastX_=lastY_=lastZ_=0.0f;
    }

    unsigned type() override { return A_DISPLAY;}
    std::string name() override { return "Display"; }
    std::string description() override {
        return
            "Display X, Y, Z\n"
            "No Outputs\n"
            ;
    }

    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) override;
    void paint (Graphics&) override;

private:
    String getNoteValue(float f);

    std::atomic<float> lastX_;
    std::atomic<float> lastY_;
    std::atomic<float> lastZ_;
};

