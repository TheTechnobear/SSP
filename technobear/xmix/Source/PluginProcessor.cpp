
#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "ssp/EditorHost.h"

#ifdef __APPLE__
#include <sstream>
#include <thread>
void log(const std::string &m) {
    std::stringstream msg;
    msg << std::this_thread::get_id() << " : " << m;
    juce::Logger::writeToLog(msg.str());
}
#else
#include <fstream>
#include <thread>
void log(const std::string &m) {
    std::ofstream s("/dev/kmsg");
    s << std::this_thread::get_id() << " : " << m << std::endl;
}
#endif


PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();
}

PluginProcessor::~PluginProcessor() {
    for (auto &track : tracks_) {
        for (auto &module : track.modules_) { module.free(); }
    }
}


bool PluginProcessor::requestModuleChange(unsigned t, unsigned m, const std::string &mn) {
    if (t >= MAX_TRACKS || m >= Track::M_MAX) return false;
    auto &track = tracks_[t];
    return track.requestModuleChange(m, mn);
}


void PluginProcessor::scanPlugins() {
    log("plugin scan - start");
    Module::scanPlugins(supportedModules_);
    for (auto m : supportedModules_) { log("module: " + m); }
    log("plugin scan - send");
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);
    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    return String("In ") + String(channelIndex + 1);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    return String("Out ") + String(channelIndex + 1);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);


    // buffers are allocated in loadModule, but sampleRate or blocksize could change
    // so these may need updating
    for (auto &track : tracks_) {
        track.prepare(sampleRate, samplesPerBlock);
        for (auto &m : track.modules_) {
            auto &plugin = m.plugin_;
            if (!plugin) continue;

            m.audioSampleBuffer_.setSize(m.audioSampleBuffer_.getNumChannels(), samplesPerBlock);
            plugin->prepare(sampleRate, samplesPerBlock);
        }
    }
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();
    int modIdx = 0;
    bool processed[Track::M_MAX] = { false, false, false, false };
    // TODO - audio processing

    // prepare input & process audio
    for (auto &track : tracks_) {
        for (auto &m : track.modules_) {
            processed[modIdx] = false;
            if (!m.lockModule_.test_and_set()) {
                processed[modIdx] = true;

                if (modIdx == Track::M_MAIN) {
                    auto &plugin = m.plugin_;
                    if (plugin) {
                        int nCh = I_MAX;
                        int nPluginCh = m.descriptor_->inputChannelNames.size();
                        for (unsigned i = 0; i < nCh && i < nPluginCh; i++) {
                            m.audioSampleBuffer_.copyFrom(i, 0, buffer.getReadPointer(i), n);
                        }

                        float *const *buffers = m.audioSampleBuffer_.getArrayOfWritePointers();
                        // juce has changed, to using a const pointer to float*
                        // now inconsistent with ssp sdk, but will work fine
                        plugin->process((float **)buffers, m.audioSampleBuffer_.getNumChannels(), n);
                    }
                }
            }
            modIdx++;
            // note: we cannot copy output yet, since this will overwrite input for next module!
        }

        // now write the audio back out
        modIdx = 0;
        for (auto &m : track.modules_) {
            if (processed[modIdx]) {
                if (modIdx == Track::M_MAIN) {
                    auto &plugin = m.plugin_;
                    if (plugin) {
                        int nCh = O_MAX;
                        int nPluginCh = m.descriptor_->outputChannelNames.size();
                        for (unsigned i = 0; i < O_MAX; i++) {
                            if (i < nPluginCh) {
                                buffer.copyFrom(i, 0, m.audioSampleBuffer_.getReadPointer(i), n);
                            } else {
                                // zero output where not enough channels
                                buffer.applyGain(i, 0, n, 0.0f);
                            }
                        }
                    }
                }
                m.lockModule_.clear();
            }
            modIdx++;
        }
    }
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    BaseProcessor::onInputChanged(i, b);
    // TODO - audio processing
    int ch = i;
    for (auto& track : tracks_) {
        int midx = Track::M_MAIN;
        auto plugin = track.modules_[midx].plugin_;
        if (!plugin) continue;
        if (ch < track.modules_[midx].descriptor_->inputChannelNames.size()) { plugin->inputEnabled(ch, b); }
    }
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    BaseProcessor::onOutputChanged(i, b);

    int ch = i;
    for (auto& track : tracks_) {
        int midx = Track::M_MAIN;
        auto plugin = track.modules_[midx].plugin_;
        if (!plugin) continue;
        if (ch < track.modules_[midx].descriptor_->outputChannelNames.size()) { plugin->outputEnabled(ch, b); }
    }
}


static constexpr int checkBytes = 0x1FF1;
static constexpr int protoVersion = 0x0001;

void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    // store plugin information
    MemoryOutputStream outStream(destData, false);

    outStream.writeInt(checkBytes);
    outStream.writeInt(protoVersion);

    outStream.writeInt(supportedModules_.size());
    for (auto mn : supportedModules_) { outStream.writeString(String(mn)); }

    auto &track = tracks_[0];
    int i = 0;
    for (auto &m : track.modules_) {
        outStream.writeInt(checkBytes);
        auto &plugin = m.plugin_;
        if (!plugin) {
            outStream.writeString("");
            outStream.writeInt(0);
            continue;
        }

        void *data;
        size_t dataSz;
        plugin->getState(&data, &dataSz);

        outStream.writeString(String(m.pluginName_.c_str()));
        outStream.writeInt(dataSz);
        outStream.write(data, dataSz);
        i++;
    }
}


void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    MemoryBlock srcData(data, sizeInBytes);
    MemoryInputStream inputStream(data, sizeInBytes, false);

    int check = inputStream.readInt();
    if (check != checkBytes) return;
    int proto = inputStream.readInt();
    if (proto != protoVersion) return;

    supportedModules_.clear();
    int nModules = inputStream.readInt();
    if (nModules == 0) {
        scanPlugins();
    } else {
        while (nModules--) {
            String m = inputStream.readString();
            supportedModules_.push_back(m.toStdString());
        }
    }

    //TODO : multi track
    int t = 0;
    auto &track = tracks_[t];
    for (int m = 0; m < Track::M_MAX; m++) {
        int check = inputStream.readInt();
        if (check != checkBytes) { return; }

        String pluginName = inputStream.readString();
        int size = inputStream.readInt();

        if (!pluginName.isEmpty() && size > 0) {
            MemoryBlock moduleData;
            moduleData.setSize(size);
            inputStream.read(moduleData.getData(), size);

            while (!requestModuleChange(t, m, pluginName.toStdString())) {}
            auto &plugin = track.modules_[m].plugin_;
            if (!plugin) continue;

            plugin->setState(moduleData.getData(), moduleData.getSize());
        }
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    static constexpr bool useSysEditor = false, defaultDraw = false;
    return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI(), useSysEditor, defaultDraw);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
