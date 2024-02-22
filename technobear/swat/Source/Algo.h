#pragma once


#include <atomic>
#include <vector>
#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

class AlgoParam {
public:
    AlgoParam(const std::string& n, const std::string& d) : name_(n), desc_(d) {;}
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
        : AlgoParam(n, d), val_(init), init_(init), min_(min), max_(max), step_(step)  {;}
    void inc() override;
    void dec() override;
    void reset() override;
    float floatVal() override { return val_;}
    void  floatVal(float v) override { val_ = constrain(v, min_, max_);}
private:
    float val_;
    float init_, min_, max_, step_;
};


class AgIntParam : public AlgoParam {
public:
    AgIntParam(
        const std::string& n, const std::string& d,
        int init = 0 , int min = 0, int max = 127, int step = 1)
        : AlgoParam(n, d), val_(init), init_(init), min_(min), max_(max), step_(step)  {;}
    void inc() override;
    void dec() override;
    void reset() override;
    float floatVal() override { return (float) val_;}
    void  floatVal(float v) override { val_ = constrain(int(v), min_, max_);}
private:
    int val_;
    int init_, min_, max_, step_;
};



// base for all algorithms
class Algo {
public:
    Algo() { ; }
    virtual ~Algo() {;}

    // general
    virtual unsigned type() = 0;
    virtual std::string name() = 0;
    virtual std::string description() = 0;

    // audio thread
    virtual void process( const float* x, const float* y, const float* z,
                          float* a, float* b, unsigned n) = 0;
    //note: x,y,z,a,b will be null(!) if not connected!

    // UI thread
    virtual void paint (juce::Graphics&);
    virtual void encoder(unsigned enc, int dir);
    virtual void encswitch(unsigned enc, bool state);
    virtual void button(unsigned btn, bool state);
    virtual void drawHelp(juce::Graphics&);


    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    static double   getSampleRate() { return sampleRate_;}
    static void     setSampleRate(double sr) { sampleRate_ = sr;}

    std::vector<std::shared_ptr<AlgoParam>> params_;
    static double sampleRate_;
};

// simple helper
void drawAB(juce::Graphics& g, float A, float B);



#include "algos/Algos.h"

