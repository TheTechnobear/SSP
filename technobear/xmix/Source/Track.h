#pragma once


#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

#include <algorithm>
#include <memory>

#include "Matrix.h"
#include "Module.h"
#include "modules/InputModule.h"
#include "modules/OutputModule.h"

class Track {
public:
    explicit Track();

    void createDefaultRoute(unsigned in1, unsigned in2);
    std::vector<Matrix::Wire> connections();

    void alloc(int sampleRate, int blockSize);
    void free();

    void prepare(int sampleRate, int blockSize);
    void process(juce::AudioSampleBuffer &ioBuffer);

    void getStateInformation(juce::MemoryOutputStream &outStream);
    void setStateInformation(juce::MemoryInputStream &inStream);


    bool requestModuleChange(unsigned midx, const std::string &mn);
    bool loadModule(std::string, Module &m);


    bool requestMatrixConnect(const Matrix::Jack &src, const Matrix::Jack &dest);
    bool requestMatrixDisconnect(const Matrix::Jack &src, const Matrix::Jack &dest);

    int sampleRate_ = 48000;
    int blockSize_ = 128;

    static constexpr int MAX_IO_IN = 8;
    static constexpr int MAX_IO_OUT = 2;
    static constexpr int MAX_IO = std::max(MAX_IO_IN, MAX_IO_OUT);

    enum ModuleIdx { M_IN, M_MOD, M_USER_1, M_USER_2, M_USER_3, M_OUT, M_MAX };

    Module modules_[M_MAX];

    static constexpr unsigned MAX_MODULES = Track::M_MAX;

private:
    std::shared_ptr<InputModule::PluginInterface> trackIn_;
    std::shared_ptr<OutputModule::PluginInterface> trackOut_;
    Matrix matrix_;
};
