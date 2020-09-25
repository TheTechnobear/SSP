#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include <atomic>



inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

class AlgoParam {
public:
    AlgoParam(const std::string& n) : name_(n) {;}
    std::string name() { return name_;}
    virtual void inc() = 0;
    virtual void dec() = 0;
    virtual void reset() = 0;
    virtual float floatVal() = 0;
    virtual ~AlgoParam() {;}
private:
    std::string name_;
};

class AgFloatParam : public AlgoParam {
public:
    AgFloatParam(
        const std::string& n, float init = 0.0f, float min = 0.0f, float max = 1.0f, float step = 0.01f)
        : AlgoParam(n), val_(init), init_(init), min_(min), max_(max), step_(step)  {;}
    void inc() override;
    void dec() override;
    void reset() override;
    float floatVal() override { return val_;}
private:
    float val_;
    float init_, min_, max_, step_;
};



// base for all algorithms
class Algo {
public:
    Algo() { ; }
    virtual ~Algo() {;}

    // general
    virtual std::string name() = 0;
    virtual std::string description() = 0;

    // audio thread
    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) = 0;
    //note: x,y,z,a,b will be null(!) if not connected!

    // UI thread
    virtual void paint (Graphics&);
    virtual void encoder(unsigned enc, int dir);
    virtual void encswitch(unsigned enc, bool state);
    virtual void button(unsigned btn, bool state);
    virtual void drawHelp(Graphics&);


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

    std::vector<std::shared_ptr<AlgoParam>> params_;
};


#include "algos/Algos.h"

