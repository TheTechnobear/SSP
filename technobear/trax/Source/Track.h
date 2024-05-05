#pragma once


#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

#include <algorithm>
#include <memory>
#include <mutex>

#include "Matrix.h"
#include "Module.h"
#include "modules/InputModule.h"
#include "modules/OutputModule.h"

class Track {
public:
    explicit Track();

    std::vector<Matrix::Wire> connections();

    void prepare(int sampleRate, int blockSize);
    void process(juce::AudioSampleBuffer &ioBuffer);

    void getStateInformation(juce::XmlElement &outStream);
    void setStateInformation(juce::XmlElement &inStream);

    bool requestModuleChange(unsigned midx, const std::string &mn);
    bool requestMatrixConnect(const Matrix::Jack &src, const Matrix::Jack &dest, float gain = 1.0f,
                              float offset = 0.0f);
    bool requestMatrixDisconnect(const Matrix::Jack &src, const Matrix::Jack &dest);
    bool requestMatrixAttenuate(const Matrix::Jack &src, const Matrix::Jack &dest, bool isOffset, float delta);
    bool requestClearTrack();

    enum ModuleIdx {
        M_IN,
        M_SLOT_1,
        M_SLOT_2,
        M_SLOT_3,
        M_SLOT_4,
        M_SLOT_5,
        M_SLOT_6,
        M_SLOT_7,
        M_SLOT_8,
        M_OUT,
        M_MAX
    };

    Module modules_[M_MAX];

    void mute(bool m) { mute_ = m; }
    bool mute() const { return mute_; }

    void level(float l) { level_ = l; }
    float level() const { return level_; }

    std::mutex mutex_;
    std::condition_variable cv_;
    bool ready_ = false;
    bool processed_ = false;
    static constexpr unsigned MAX_MODULES = Track::M_MAX;
    static constexpr unsigned MAX_USER_MODULES = M_OUT - M_IN - 1;

private:
    static constexpr int MAX_IO_IN = 8;
    static constexpr int MAX_IO_OUT = 2;
    static constexpr int MAX_IO = std::max(MAX_IO_IN, MAX_IO_OUT);

    bool loadModule(std::string, Module &m);
    void alloc(int sampleRate, int blockSize);
    void free();

    void resetModuleConnections(int midx);
    void clearModuleConnections(int midx);
    int sampleRate_ = 48000;
    int blockSize_ = 128;
    bool mute_ = false;
    float level_ = 1.0f;

    std::shared_ptr<InputModule::PluginInterface> trackIn_;
    std::shared_ptr<OutputModule::PluginInterface> trackOut_;
    Matrix matrix_;
    std::atomic_flag lock_ = ATOMIC_FLAG_INIT;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Track)
};
