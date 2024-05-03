#include "Track.h"

#include "ssp/Log.h"

Track::Track() {
    trackIn_ = std::make_shared<InputModule::PluginInterface>();
    modules_[M_IN].alloc("IN", trackIn_.get(), trackIn_->createDescriptor(), nullptr);
    trackOut_ = std::make_shared<OutputModule::PluginInterface>();
    modules_[M_OUT].alloc("OUT", trackOut_.get(), trackOut_->createDescriptor(), nullptr);
}

std::vector<Matrix::Wire> Track::connections() {
    return matrix_.connections_;
}

bool Track::requestModuleChange(unsigned midx, const std::string &mn) {
    auto &m = modules_[midx];

    if (!m.lock_.test_and_set()) {
        clearModuleConnections(midx);

        if (m.loadModule(mn)) {
            resetModuleConnections(midx);
            m.prepare(sampleRate_, blockSize_);
        }
        m.lock_.clear();
        return true;
    }
    return false;
}

bool Track::requestClearTrack() {
    if (!lock_.test_and_set()) {
        for (int midx = 0; midx < M_MAX; midx++) {
            if (midx == M_IN || midx == M_OUT) continue;
            auto &m = modules_[midx];
            while (!m.lock_.test_and_set()) {};
            clearModuleConnections(midx);
            modules_[midx].free();
            m.lock_.clear();
        }
        lock_.clear();
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

        if (srcMod.descriptor_ && src.chIdx_ < srcMod.descriptor_->outputChannelNames.size() && destMod.descriptor_ &&
            dest.chIdx_ < destMod.descriptor_->inputChannelNames.size()) {
            matrix_.connect(src, dest);
            if (srcCount == 0 && srcMod.plugin_) srcMod.plugin_->outputEnabled(src.chIdx_, true);
            if (destCount == 0 && destMod.plugin_) destMod.plugin_->inputEnabled(dest.chIdx_, true);
        }

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

        if (!mute()) {
            unsigned modIdx = 0;
            for (auto &m : modules_) {
                auto &moduleBuf = m.audioBuffer_;
                if (modIdx > 0) moduleBuf.applyGain(0.0f);
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

// form juce_AudioProcessor.cpp
const juce::uint32 magicXmlNumber = 0x21324356;
void copyXmlToBinary(const juce::XmlElement &xml, juce::MemoryBlock &destData) {
    {
        juce::MemoryOutputStream out(destData, false);
        out.writeInt(magicXmlNumber);
        out.writeInt(0);
        xml.writeTo(out, juce::XmlElement::TextFormat().singleLine());
        out.writeByte(0);
    }

    // go back and write the string length..
    static_cast<juce::uint32 *>(destData.getData())[1] =
        juce::ByteOrder::swapIfBigEndian((juce::uint32)destData.getSize() - 9);
}

std::unique_ptr<juce::XmlElement> getXmlFromBinary(const void *data, const int sizeInBytes) {
    if (sizeInBytes > 8 && juce::ByteOrder::littleEndianInt(data) == magicXmlNumber) {
        auto stringLength = (int)juce::ByteOrder::littleEndianInt(juce::addBytesToPointer(data, 4));

        if (stringLength > 0)
            return parseXML(juce::String::fromUTF8(static_cast<const char *>(data) + 8,
                                                   juce::jmin((sizeInBytes - 8), stringLength)));
    }
    return {};
}


void Track::getStateInformation(juce::XmlElement &outStream) {
    outStream.setAttribute("mute", mute_);
    outStream.setAttribute("level", level_);

    std::unique_ptr<juce::XmlElement> xmlModules = std::make_unique<juce::XmlElement>("Modules");

    for (auto &m : modules_) {
        std::unique_ptr<juce::XmlElement> xmlModule = std::make_unique<juce::XmlElement>("Module");

        auto &plugin = m.plugin_;
        if (!plugin) {
            xmlModule->setAttribute("pluginName", "");
            xmlModule->setAttribute("dataSz", (int)0);
        } else {
            void *data;
            size_t dataSz;
            plugin->getState(&data, &dataSz);
            xmlModule->setAttribute("pluginName", m.pluginName_.c_str());
            xmlModule->setAttribute("dataSz", (int)dataSz);

            if (dataSz > 0 && data) {
                std::unique_ptr<juce::XmlElement> xmlPlugData = std::make_unique<juce::XmlElement>("data");
                auto pluginData = getXmlFromBinary(data, dataSz);
                if (pluginData) {
                    xmlPlugData->addChildElement(pluginData.release());
                    xmlModule->addChildElement(xmlPlugData.release());
                }
            }
        }

        xmlModules->addChildElement(xmlModule.release());
    }
    outStream.addChildElement(xmlModules.release());

    std::unique_ptr<juce::XmlElement> xmlMatrix = std::make_unique<juce::XmlElement>("Matrix");
    matrix_.getStateInformation(*xmlMatrix);
    outStream.addChildElement(xmlMatrix.release());
}

void Track::setStateInformation(juce::XmlElement &inStream) {
    mute_ = inStream.getBoolAttribute("mute", false);
    level_ = inStream.getDoubleAttribute("level", 1.0f);

    auto xmlModules = inStream.getChildByName("Modules");
    if (xmlModules) {
        int midx = 0;
        for (auto xmlModule : xmlModules->getChildIterator()) {
            juce::String pluginName = xmlModule->getStringAttribute("pluginName");
            int size = xmlModule->getIntAttribute("dataSz");

            if (!pluginName.isEmpty() && size > 0) {
                while (!requestModuleChange(midx, pluginName.toStdString())) {}

                auto &plugin = modules_[midx].plugin_;
                if (plugin) {
                    auto xmlPlugData = xmlModule->getChildByName("data");
                    if (xmlPlugData) {
                        juce::MemoryBlock moduleData;
                        auto pluginData = xmlPlugData->getFirstChildElement();
                        if (pluginData) {
                            copyXmlToBinary(*pluginData, moduleData);
                            plugin->setState(moduleData.getData(), moduleData.getSize());
                        }
                    }
                }
            }
            midx++;
            if (midx >= M_MAX) break;
        }
    } else {
        ssp::log("setStateInformation : no Modules tag");
    }

    auto xmlMatrix = inStream.getChildByName("Matrix");
    if (xmlMatrix) {
        matrix_.setStateInformation(*xmlMatrix);

        auto wires = matrix_.connections_;
        matrix_.connections_.clear();
        for (auto &w : wires) { requestMatrixConnect(w.src_, w.dest_); }

    } else {
        ssp::log("setStateInformation : no Matrix tag");
    }

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


void Track::clearModuleConnections(int midx) {
    auto &wires = matrix_.connections_;
    wires.erase(
        std::remove_if(wires.begin(), wires.end(),
                       [&](const Matrix::Wire &w) { return w.src_.modIdx_ == midx || w.dest_.modIdx_ == midx; }),
        wires.end());
}