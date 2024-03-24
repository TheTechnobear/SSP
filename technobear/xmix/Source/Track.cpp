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
            bufferWork_.setSize(MAX_WORK_IO, blockSize_);
        }
        sampleRate_ = sampleRate;

        for (auto &m : modules_) { m.prepare(sampleRate_, blockSize_); }
    }
}

void Track::process(juce::AudioSampleBuffer &ioBuffer) {
    size_t n = blockSize_;
    unsigned modIdx = 0;
    for (auto &m : modules_) {
        // prep input
        if (modIdx == Track::M_PRE) {
            auto &buffer = bufferWork_;
            buffer.applyGain(0.f);
            for (int i = 0; i < MAX_IO_IN; i++) { buffer.addFrom(i, 0, ioBuffer, i, 0, n); }
        }

        if (modIdx == Track::M_MOD) {
            auto &buffer = bufferMod_;
            buffer.applyGain(0.f);
            for (int i = 0; i < MAX_IO_IN; i++) { buffer.addFrom(i, 0, ioBuffer, i, 0, n); }
        }

        if (!m.lockModule_.test_and_set()) {
            if (modIdx == Track::M_MOD) {
                m.process(bufferMod_);
            } else {
                m.process(bufferWork_);
            }
            m.lockModule_.clear();
        }

        if (modIdx == Track::M_POST) {
            auto &buffer = ioBuffer;
            buffer.applyGain(0.f);
            for (int i = 0; i < MAX_IO_OUT; i++) { buffer.addFrom(i, 0, bufferWork_, i, 0, n); }
        }

        modIdx++;
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
}

void Track::setStateInformation(juce::MemoryInputStream &inputStream) {
    int check = inputStream.readInt();
    if (check != checkTrackBytes) return;

    for (int m = 0; m < Track::M_MAX; m++) {
        int check = inputStream.readInt();
        if (check != checkModuleBytes) { return; }

        juce::String pluginName = inputStream.readString();
        int size = inputStream.readInt();

        if (!pluginName.isEmpty() && size > 0) {
            juce::MemoryBlock moduleData;
            moduleData.setSize(size);
            inputStream.read(moduleData.getData(), size);

            while (!requestModuleChange(m, pluginName.toStdString())) {}
            auto &plugin = modules_[m].plugin_;
            if (!plugin) continue;

            plugin->setState(moduleData.getData(), moduleData.getSize());
        }
    }
}
