#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include <atomic>
#include <vector>
#include <memory>


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

class AlgoParam {
public:
    AlgoParam(const std::string& n,const std::string& d) : name_(n), desc_(d) {;}
    std::string name() { return name_;}
    std::string desc() { return desc_;}
    virtual void inc() = 0;
    virtual void dec() = 0;
    virtual void reset() = 0;
    virtual float floatVal() = 0;
    virtual void  floatVal(float) = 0;
    virtual ~AlgoParam() {;}
private:
    std::string name_;
    std::string desc_;
};

class AgFloatParam : public AlgoParam {
public:
    AgFloatParam(
        const std::string& n, const std::string& d, 
        float init = 0.0f, float min = 0.0f, float max = 1.0f, float step = 0.01f)
        : AlgoParam(n,d), val_(init), init_(init), min_(min), max_(max), step_(step)  {;}
    void inc() override;
    void dec() override;
    void reset() override;
    float floatVal() override { return val_;}
    void  floatVal(float v) override { val_ = constrain(v, min_, max_);}
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


    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    float cv2Pitch(float r) {
#ifndef __APPLE__        
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first A note
        static constexpr float p2 = 0.21187f; // second A note
#else 
        static constexpr float p1 = 0.0f; // first A note
        static constexpr float p2 = 0.2f; // second A note
#endif         
        static constexpr float scale = 12.0f / (p2 - p1);
        float arg = r;
        arg = arg - p1;
        arg = arg * scale;
        return arg;
    }

    std::vector<std::shared_ptr<AlgoParam>> params_;
};


#include "algos/Algos.h"

