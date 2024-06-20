#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

#include <algorithm>
#include <memory>
#include <mutex>

#include "Module.h"

class Track {
public:
    explicit Track();

    void prepare(int sampleRate, int blockSize);
    void process(juce::AudioSampleBuffer &ioBuffer);

    void getStateInformation(juce::XmlElement &outStream);
    void setStateInformation(juce::XmlElement &inStream);

    bool requestModuleChange(unsigned midx, const std::string &mn);
    bool requestClearTrack();

    enum ModuleIdx {       
        M_LEFT,
        M_RIGHT,
        M_MAX
    };

    static constexpr int M_IN = -1;
    static constexpr int M_OUT = M_MAX;



    Module modules_[M_MAX];

    std::mutex mutex_;
    std::condition_variable cv_;
    bool ready_ = false;
    bool processed_ = false;
    static constexpr unsigned MAX_MODULES = Track::M_MAX;

private:
    static constexpr int MAX_IO_IN = 8;
    static constexpr int MAX_IO_OUT = 2;
    static constexpr int MAX_IO = std::max(MAX_IO_IN, MAX_IO_OUT);

    bool loadModule(std::string, Module &m);
    void alloc(int sampleRate, int blockSize);
    void free();

    int sampleRate_ = 48000;
    int blockSize_ = 128;

    std::atomic_flag lock_ = ATOMIC_FLAG_INIT;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Track)
};
