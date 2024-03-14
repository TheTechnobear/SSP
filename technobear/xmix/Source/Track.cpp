#include "Track.h"


void Track::alloc(int sampleRate, int blockSize) {
    prepare(sampleRate, blockSize);
}

void Track::prepare(int sampleRate, int blockSize) {
    if (blockSize != blockSize_) {
        bufferIO_.setSize(MAX_IO, blockSize_);
        bufferMod_.setSize(MAX_MOD_IO, blockSize_);
        bufferWork_.setSize(MAX_WORK_IO, blockSize_);
    }
    if (blockSize_ != blockSize || sampleRate_ != sampleRate) {
        sampleRate_ = sampleRate;
        blockSize_ = blockSize;

        for (auto &m : modules_) { m.prepare(sampleRate_, blockSize_); }
    }
}

void Track::free() {
}


bool Track::requestModuleChange(unsigned midx, const std::string &mn) {
    auto &m = modules_[midx];

    if (!m.lockModule_.test_and_set()) {
        if (m.loadModule(mn)) { m.prepare(sampleRate_, blockSize_); }
        m.lockModule_.clear();
        return true;
    }
    return false;
}
