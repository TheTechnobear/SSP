#pragma once


#include <algorithm>
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>


#include "Module.h"
#include "Matrix.h"


struct Track {
    void alloc(int sampleRate, int blockSize);
    void free();

    void prepare(int sampleRate, int blockSize);
    void process(juce::AudioSampleBuffer &ioBuffer);

    void getStateInformation(juce::MemoryOutputStream& outStream);
    void setStateInformation(juce::MemoryInputStream& inStream);


    juce::AudioSampleBuffer bufferIO_;
    juce::AudioSampleBuffer bufferMod_;
    juce::AudioSampleBuffer bufferWork_[2]; // flipflop for module

    bool requestModuleChange(unsigned midx, const std::string &mn);
    bool loadModule(std::string, Module &m);

    int sampleRate_ = 0;
    int blockSize_ = 0;

    static constexpr int MAX_IO_IN = 8;
    static constexpr int MAX_IO_OUT = 2;
    static constexpr int MAX_IO = std::max(MAX_IO_IN, MAX_IO_OUT);

    static constexpr int MAX_MOD_IN = 8;
    static constexpr int MAX_MOD_OUT = 8;
    static constexpr int MAX_MOD_IO = std::max(MAX_MOD_IN, MAX_MOD_OUT);

    static constexpr int MAX_WORK_IN = 16;
    static constexpr int MAX_WORK_OUT = 16;
    static constexpr int MAX_WORK_IO = std::max(MAX_WORK_IN, MAX_WORK_OUT);

    enum ModuleIdx { M_MOD, M_PRE, M_MAIN, M_POST, M_MAX };

    Module modules_[M_MAX];

    static constexpr unsigned MAX_MODULES = Track::M_MAX;

    Matrix matrix_;
};
