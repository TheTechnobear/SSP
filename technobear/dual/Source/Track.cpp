#include "Track.h"

#include "PluginProcessor.h"
#include "ssp/Log.h"

Track::Track() {
}

bool Track::requestModuleChange(unsigned midx, const std::string &mn) {
    auto &m = modules_[midx];

    if (!m.lock_.test_and_set()) {
        if (m.loadModule(mn)) { m.prepare(sampleRate_, blockSize_); }
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
            modules_[midx].free();
            m.lock_.clear();
        }
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
        size_t n = ioBuffer.getNumSamples();
        bool processed[M_MAX] = { false, false };
        // prepare input & process audio
        int modIdx = 0;
        for (auto &m : modules_) {
            if (!m.plugin_ || !m.descriptor_) {
                modIdx++;
                continue;
            }

            auto &moduleBuf = m.audioBuffer_;
            int chOffset = modIdx * PluginProcessor::MAX_IN;
            int nCh = m.descriptor_->inputChannelNames.size();
            for (unsigned i = 0; i < nCh && i < PluginProcessor::MAX_IN; i++) {
                m.audioBuffer_.copyFrom(i, 0, ioBuffer.getReadPointer(i + chOffset), n);
            }

            if (!m.lock_.test_and_set()) {
                m.process(moduleBuf);
                m.lock_.clear();
                processed[modIdx] = true;
            }
            modIdx++;
            // note: we cannot copy output yet, since this will overwrite input for next module!
        }

        // now write the audio back out
        modIdx = 0;
        for (auto &m : modules_) {
            if (processed[modIdx]) {
                int chOffset = modIdx * PluginProcessor::MAX_OUT;
                int nCh = m.descriptor_->outputChannelNames.size();
                for (unsigned i = 0; i < PluginProcessor::MAX_OUT; i++) {
                    if (i < nCh) {
                        ioBuffer.copyFrom(i + chOffset, 0, m.audioBuffer_.getReadPointer(i), n);
                    } else {
                        // zero output where not enough channels
                        ioBuffer.applyGain(i + chOffset, 0, n, 0.0f);
                    }
                }
                m.lock_.clear();
            }
            modIdx++;
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
}

void Track::setStateInformation(juce::XmlElement &inStream) {
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
}


void Track::alloc(int sampleRate, int blockSize) {
    prepare(sampleRate, blockSize);
}


void Track::free() {
}
