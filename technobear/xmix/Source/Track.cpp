#include "Track.h"


Track::Track() {
    trackIn_ = std::make_shared<InputModule::PluginInterface>();
    modules_[M_IN].alloc("IN", trackIn_.get(), trackIn_->createDescriptor(), nullptr);
    trackOut_ = std::make_shared<OutputModule::PluginInterface>();
    modules_[M_OUT].alloc("OUT", trackOut_.get(), trackOut_->createDescriptor(), nullptr);
}


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
            for (auto &buf : audioBuffers_) {
                // FIXME - NUM CH on audiobuffer
                buf.setSize(MAX_IO, blockSize_);
            }
        }
        sampleRate_ = sampleRate;

        for (auto &m : modules_) { m.prepare(sampleRate_, blockSize_); }
    }
}

void Track::process(juce::AudioSampleBuffer &ioBuffer) {
    size_t n = blockSize_;

    // FIXME - check this does not alter buffer size
    // FIXME - ensure channels are in range of plugin and io
    audioBuffers_[M_IN] = ioBuffer;

    unsigned modIdx = 0;

    for (auto &m : modules_) {
        auto &moduleBuf = audioBuffers_[modIdx];
        for (auto &route : matrix_.connections_) {
            moduleBuf.applyGain(0.0f);
            if (route.dest_.modIdx_ == modIdx) {
                auto &srcBuf = audioBuffers_[route.src_.modIdx_];
                moduleBuf.addFrom(route.dest_.chIdx, 0, srcBuf, route.src_.chIdx, 0, n);
            }
        }

        if (!m.lockModule_.test_and_set()) {
            // FIXME - process()
            // m.process(moduleBuf); // FIXME
            m.lockModule_.clear();
        }
        modIdx++;
    }
    // FIXME - check this does not alter buffer size
    ioBuffer = audioBuffers_[M_OUT];
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


bool Track::requestMatrixConnect(const Matrix::Jack &src, const Matrix::Jack &dest) {
    int modIdx = dest.modIdx_;
    auto &m = modules_[modIdx];
    if (!m.lockModule_.test_and_set()) {
        matrix_.connect(src, dest);

        m.lockModule_.clear();
        return true;
    }
    return false;
}


bool Track::requestMatrixDisconnect(const Matrix::Jack &src, const Matrix::Jack &dest) {
    int modIdx = dest.modIdx_;
    auto &m = modules_[modIdx];
    if (!m.lockModule_.test_and_set()) {
        matrix_.disconnect(src, dest);
        m.lockModule_.clear();
        return true;
    }
    return false;
}
