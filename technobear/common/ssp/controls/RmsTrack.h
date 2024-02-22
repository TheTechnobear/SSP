#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace ssp {

class RmsTrack {
public:
    explicit RmsTrack() {
        clear();
    }

    ~RmsTrack() { ; }

    float lvl() { return lvl_; }

    void clear() {
        lvl_ = 0.0f;
        lvlHead_ = 0;
        lvlSum_ = 0.0f;
        for (unsigned i = 0; i < MAX_HISTORY; i++) {
            lvlHistory_[i] = 0.0f;
        }
    }

    void process(float t_level) {
        lvlSum_ -= lvlHistory_[lvlHead_];
        lvlHistory_[lvlHead_] = t_level;
        lvlSum_ += lvlHistory_[lvlHead_];
        lvlHead_ = (lvlHead_ + 1) % MAX_HISTORY;
        lvl_ = lvlSum_ / MAX_HISTORY;
    }

    void process(juce::AudioSampleBuffer &buffer, unsigned ch) {
        unsigned n = buffer.getNumSamples();
        float tlvl = useRMS_
                     ? buffer.getRMSLevel(ch, 0, n)
                     : buffer.getMagnitude(ch, 0, n);
        process(tlvl);
    }

private:
    std::atomic<float> lvl_; // calculated rms/peak level
    bool useRMS_ = false;
    static constexpr unsigned MAX_HISTORY = 100; // 1000 / (48000/129)  = 1 block = 2.8 mS, we want ~ 300mS
    unsigned lvlHead_ = 0;
    float lvlSum_ = 0.0f;
    float lvlHistory_[MAX_HISTORY];
};


}