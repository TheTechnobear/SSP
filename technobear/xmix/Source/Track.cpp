#include "Track.h"


void Track::alloc(int sampleRate, int blockSize) {
    prepare(sampleRate, blockSize);
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

void Track::prepare(int sampleRate, int blockSize) {
    if (blockSize_ != blockSize || sampleRate_ != sampleRate) {
        if (blockSize != blockSize_) {
            blockSize_ = blockSize;
            bufferIO_.setSize(MAX_IO, blockSize_);
            bufferMod_.setSize(MAX_MOD_IO, blockSize_);
            bufferWork_[0].setSize(MAX_WORK_IO, blockSize_);
            bufferWork_[1].setSize(MAX_WORK_IO, blockSize_);
        }
        sampleRate_ = sampleRate;

        for (auto &m : modules_) { m.prepare(sampleRate_, blockSize_); }
    }
}

void Track::process(juce::AudioSampleBuffer &ioBuffer) {
    size_t n = blockSize_;

    bufferIO_ = ioBuffer;
    bufferWork_[0].applyGain(0.f);
    bufferWork_[1].applyGain(0.f);

    unsigned modIdx = 0;

    for (auto &m : modules_) {
        unsigned workIdx = modIdx % 2;
        auto &moduleBuf = modIdx == Track::M_MOD ? bufferMod_ : bufferWork_[workIdx];

        // prep input
        for (auto &route : matrix_.modules_[modIdx].routes_) {
            switch (route.src_) {
                case Matrix::SRC_INPUT: {
                    auto &buffer = ioBuffer;
                    moduleBuf.addFrom(route.destChannel_, 0, buffer, route.srcChannel_, 0, n);
                    break;
                }
                case Matrix::SRC_MOD: {
                    auto &buffer = bufferMod_;
                    moduleBuf.addFrom(route.destChannel_, 0, buffer, route.srcChannel_, 0, n);
                    break;
                }
                case Matrix::SRC_WORK: {
                    unsigned prevWorkIdx = (modIdx + 1) % 2;
                    auto &buffer = bufferWork_[prevWorkIdx];
                    moduleBuf.addFrom(route.destChannel_, 0, buffer, route.srcChannel_, 0, n);
                    break;
                }
            }
        }


        if (!m.lockModule_.test_and_set()) {
            m.process(moduleBuf);
            m.lockModule_.clear();
        }
        modIdx++;
    }

    // prep output
    for (auto &route : matrix_.output_.routes_) {
        switch (route.src_) {
            case Matrix::SRC_INPUT: {
                auto &buffer = bufferIO_;
                ioBuffer.addFrom(route.destChannel_, 0, buffer, route.srcChannel_, 0, n);
                break;
            }
            case Matrix::SRC_MOD: {
                auto &buffer = bufferMod_;
                ioBuffer.addFrom(route.destChannel_, 0, buffer, route.srcChannel_, 0, n);
                break;
            }
            case Matrix::SRC_WORK: {
                unsigned workIdx = (M_MAX - 1) % 2;
                auto &buffer = bufferWork_[workIdx];
                ioBuffer.addFrom(route.destChannel_, 0, buffer, route.srcChannel_, 0, n);
                break;
            }
        }
    }
}

static constexpr int checkTrackBytes = 0x1FF2;
static constexpr int checkModuleBytes = 0x1FF3;


void Track::getStateInformation(juce::MemoryOutputStream &outStream) {
    outStream.writeInt(checkTrackBytes);
    for (auto &m : modules_) {
        outStream.writeInt(checkModuleBytes);
        auto &plugin = m.plugin_;
        if (!plugin) {
            outStream.writeString("");
            outStream.writeInt(0);
            continue;
        }

        void *data;
        size_t dataSz;
        plugin->getState(&data, &dataSz);

        outStream.writeString(juce::String(m.pluginName_.c_str()));
        outStream.writeInt(dataSz);
        outStream.write(data, dataSz);
    }
    matrix_.getStateInformation(outStream);
}

void Track::setStateInformation(juce::MemoryInputStream &inStream) {
    int check = inStream.readInt();
    if (check != checkTrackBytes) return;

    for (int m = 0; m < Track::M_MAX; m++) {
        int check = inStream.readInt();
        if (check != checkModuleBytes) { return; }

        juce::String pluginName = inStream.readString();
        int size = inStream.readInt();

        if (!pluginName.isEmpty() && size > 0) {
            juce::MemoryBlock moduleData;
            moduleData.setSize(size);
            inStream.read(moduleData.getData(), size);

            while (!requestModuleChange(m, pluginName.toStdString())) {}
            auto &plugin = modules_[m].plugin_;
            if (!plugin) continue;

            plugin->setState(moduleData.getData(), moduleData.getSize());
        }
    }

    matrix_.setStateInformation(inStream);
}


bool Track::requestMatrixModuleAdd(unsigned modIdx, Matrix::Src s, unsigned sCh, unsigned dCh) {
    auto &m = modules_[modIdx];
    if (!m.lockModule_.test_and_set()) {
        auto &mR = matrix_.modules_[modIdx];
        mR.addRoute(s, sCh, dCh);

        m.lockModule_.clear();
        return true;
    }
    return false;
}

bool Track::requestMatrixModuleRemove(unsigned modIdx, unsigned routeIdx) {
    auto &m = modules_[modIdx];
    if (!m.lockModule_.test_and_set()) {
        auto &mR = matrix_.modules_[modIdx];
        mR.removeRoute(routeIdx);
        m.lockModule_.clear();
        return true;
    }
    return false;
}

bool Track::requestMatrixOutputAdd(Matrix::Src s, unsigned sCh, unsigned dCh) {
    auto &m = modules_[M_POST];
    if (!m.lockModule_.test_and_set()) {
        matrix_.output_.addRoute(s, sCh, dCh);

        m.lockModule_.clear();
        return true;
    }
    return false;
}

bool Track::requestMatrixOutputRemove(unsigned routeIdx) {
    auto &m = modules_[M_POST];
    if (!m.lockModule_.test_and_set()) {
        matrix_.output_.removeRoute(routeIdx);

        m.lockModule_.clear();
        return true;
    }
    return false;
}
