#pragma once

#include <atomic>

// base for all algorithms
class Algo {
public:
    Algo() : x_(0.0f), y_(0.0f), z_(0.0f) { ; }
    //note: x,y,z,a,b will be null(!) if not connected!
    virtual void process( const float* x, const float* y, const float* z,
                  float* a, float* b, unsigned n) = 0;
    virtual ~Algo() {;}

    float x() { return x_;}
    float y() { return y_;}
    float z() { return z_;}
    void  x(float v) { x_=v;}
    void  y(float v) { y_=v;}
    void  z(float v) { z_=v;}


    std::atomic<float> x_;
    std::atomic<float> y_;
    std::atomic<float> z_;

    float cv2Pitch(float r) {
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first C note
        static constexpr float p2 = 0.21187f; // second C note
        static constexpr float scale = 12.0f / (p2 - p1);
        float arg = r;
        arg = arg - p1;
        arg = arg * scale;
        return arg;
    }
};


class Algo_Test : public Algo {
public:
    void process(   const float* x, const float* y, const float* z,
                    float* a, float* b, unsigned n) override;

};
