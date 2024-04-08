#include "Track.h"


Track::Track() {
    trackIn_ = std::make_shared<InputModule::PluginInterface>();
    modules_[M_IN].alloc("IN", trackIn_.get(), trackIn_->createDescriptor(), nullptr);
    trackOut_ = std::make_shared<OutputModule::PluginInterface>();
    modules_[M_OUT].alloc("OUT", trackOut_.get(), trackOut_->createDescriptor(), nullptr);
}


void Track::createDefaultRoute(unsigned in1, unsigned in2) {
    // requestMatrixConnect(Matrix::Jack(M_IN, in1), Matrix::Jack(M_USER_1, 0));
    // requestMatrixConnect(Matrix::Jack(M_IN, in2), Matrix::Jack(M_USER_1, 1));
    // requestMatrixConnect(Matrix::Jack(M_USER_1, 0), Matrix::Jack(M_USER_2, 0));
    // requestMatrixConnect(Matrix::Jack(M_USER_1, 1), Matrix::Jack(M_USER_2, 1));
    // requestMatrixConnect(Matrix::Jack(M_USER_2, 0), Matrix::Jack(M_USER_3, 0));
    // requestMatrixConnect(Matrix::Jack(M_USER_2, 1), Matrix::Jack(M_USER_3, 1));
    // requestMatrixConnect(Matrix::Jack(M_USER_3, 0), Matrix::Jack(M_OUT, 0));
    // requestMatrixConnect(Matrix::Jack(M_USER_3, 1), Matrix::Jack(M_OUT, 1));
}

std::vector<Matrix::Wire> Track::connections() {
    return matrix_.connections_;
}


bool Track::requestModuleChange(unsigned midx, const std::string &mn) {
    auto &m = modules_[midx];

    if (!m.lock_.test_and_set()) {
        if (m.loadModule(mn)) {
            resetModuleConnections(midx);
            m.prepare(sampleRate_, blockSize_);
        }
        m.lock_.clear();
        return true;
    }
    return false;
}

bool Track::requestMatrixConnect(const Matrix::Jack &src, const Matrix::Jack &dest) {
    if (!lock_.test_and_set()) {
        int srcCount = 0;
        int destCount = 0;
        for (auto &w : matrix_.connections_) {
            if (w.src_ == src) srcCount++;
            if (w.dest_ == dest) destCount++;
        }

        auto &srcMod = modules_[src.modIdx_];
        auto &destMod = modules_[dest.modIdx_];
        matrix_.connect(src, dest);
        if (srcCount == 0 && srcMod.plugin_) srcMod.plugin_->outputEnabled(src.chIdx_, true);
        if (destCount == 0 && destMod.plugin_) destMod.plugin_->inputEnabled(dest.chIdx_, true);
        lock_.clear();
        return true;
    }
    return false;
}


bool Track::requestMatrixDisconnect(const Matrix::Jack &src, const Matrix::Jack &dest) {
    if (!lock_.test_and_set()) {
        int srcCount = 0;
        int destCount = 0;
        for (auto &w : matrix_.connections_) {
            if (w.src_ == src) srcCount++;
            if (w.dest_ == dest) destCount++;
        }

        auto &srcMod = modules_[src.modIdx_];
        auto &destMod = modules_[dest.modIdx_];
        matrix_.disconnect(src, dest);
        if (srcCount == 1 && srcMod.plugin_) srcMod.plugin_->outputEnabled(src.chIdx_, false);
        if (destCount == 1 && destMod.plugin_) destMod.plugin_->inputEnabled(dest.chIdx_, false);
        lock_.clear();
        return true;
    }
    return false;
}


void Track::prepare(int sampleRate, int blockSize) {
    blockSize_ = blockSize;
    sampleRate_ = sampleRate;

    for (auto &m : modules_) { m.prepare(sampleRate_, blockSize_); }
}

void Track::process(juce::AudioSampleBuffer &ioBuffer) {
   if (!lock_.test_and_set()) {
        size_t n = blockSize_;
        auto &inMod = modules_[M_IN];
        for (int c = 0; c < MAX_IO_IN; c++) { inMod.audioBuffer_.copyFrom(c, 0, ioBuffer, c, 0, n); }

        if(!mute()) {
            unsigned modIdx = 0;
            for (auto &m : modules_) {
                auto &moduleBuf = m.audioBuffer_;
                moduleBuf.applyGain(0.0f);
                for (auto &route : matrix_.connections_) {
                    if (route.dest_.modIdx_ == modIdx) {
                        auto &srcBuf = modules_[route.src_.modIdx_].audioBuffer_;
                        moduleBuf.addFrom(route.dest_.chIdx_, 0, srcBuf, route.src_.chIdx_, 0, n);
                    }
                }

                if (!m.lock_.test_and_set()) {
                    m.process(moduleBuf);
                    m.lock_.clear();
                }
                modIdx++;
            }
            auto &outMod = modules_[M_OUT];
            for (int c = 0; c < MAX_IO_OUT; c++) { ioBuffer.copyFrom(c, 0, outMod.audioBuffer_, c, 0, n); }
        } else {
            // muted, so don't process
            ioBuffer.applyGain(0.0f);
        }
        lock_.clear();
    }
}

static constexpr int checkTrackBytes = 0x1FF2;
static constexpr int checkModuleBytes = 0x1FF3;


void Track::getStateInformation(juce::MemoryOutputStream &outStream) {
    outStream.writeInt(checkTrackBytes);
    outStream.writeBool(mute_);
    outStream.writeFloat(level_);
    
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

    mute_ = inStream.readBool();
    level_ = inStream.readFloat();


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
    for (int midx = 0; midx < M_MAX; midx++) { resetModuleConnections(midx); }
}


void Track::alloc(int sampleRate, int blockSize) {
    prepare(sampleRate, blockSize);
}


void Track::free() {
}

void Track::resetModuleConnections(int midx) {
    auto &m = modules_[midx];
    if (m.descriptor_ && m.plugin_) {
        int inSz = m.descriptor_->inputChannelNames.size();
        for (int c = 0; c < inSz; c++) { m.plugin_->inputEnabled(c, false); }
        int outSz = m.descriptor_->outputChannelNames.size();
        for (int c = 0; c < outSz; c++) { m.plugin_->outputEnabled(c, false); }

        for (auto &w : matrix_.connections_) {
            if (w.dest_.modIdx_ == midx && w.dest_.chIdx_ < inSz) m.plugin_->inputEnabled(w.dest_.chIdx_, true);
            if (w.src_.modIdx_ == midx && w.src_.chIdx_ < outSz) m.plugin_->outputEnabled(w.src_.chIdx_, true);
        }
    }
}
