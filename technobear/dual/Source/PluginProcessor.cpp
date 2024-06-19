
#include "PluginProcessor.h"

#include <dlfcn.h>

#include <fstream>

#include "PluginEditor.h"
#include "ssp/EditorHost.h"

#include "ssp/Log.h"

// #ifdef __APPLE__
// const static int dlopenmode = RTLD_LOCAL | RTLD_NOW;
// const char *pluginSuffix = ".vst3/Contents/MacOS/";

// #ifdef FORCE_COMPACT_UI
// const char *pluginPath = "plugins/";
// #else  // use full UI
// const char *pluginPath = "~/Library/Audio/Plug-Ins/VST3/";
// #endif // FORCE_COMPACT_UI

// #else
// // linux
// const static int dlopenmode = RTLD_LOCAL | RTLD_NOW | RTLD_DEEPBIND;
// const char *pluginPath = "plugins/";
// #endif



PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();
}

PluginProcessor::~PluginProcessor() {
    for (auto &module : modules_) { module.free(); }
}


bool PluginProcessor::requestModuleChange(unsigned t, unsigned m, const std::string &mn) {
    if (t >= MAX_TRACKS || m >= Track::M_MAX) return false;
    auto &track = tracks_[t];
    bool ret = track.requestModuleChange(m, mn);
    return ret;
}


void PluginProcessor::loadSupportedModules(bool force) {
    if (!force) {
        if (supportedModules_.empty()) { Module::loadSupportedModules(supportedModules_); }
    }

    if (force || supportedModules_.empty()) {
        ssp::log("plugin scan - start");
        Module::scanPlugins(supportedModules_);
        for (auto m : supportedModules_) { ssp::log("module: " + m.name); }
        ssp::log("plugin scan - send");
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);
    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    String bname =
        String("M") + String((channelIndex / MAX_IN) + 1) + String(" In ") + String((channelIndex % MAX_IN) + 1);
    return bname;
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    String bname =
        String("M") + String((channelIndex / MAX_OUT) + 1) + String(" Out ") + String((channelIndex % MAX_OUT) + 1);
    return bname;
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);


    // buffers are allocated in loadModule, but sampleRate or blocksize could change
    // so these may need updating
    for (auto &m : modules_) {
        auto &plugin = m.plugin_;
        if (!plugin) continue;

        m.audioBuffer_.setSize(m.audioBuffer_.getNumChannels(), samplesPerBlock);
        plugin->prepare(sampleRate, samplesPerBlock);
    }
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    size_t n = buffer.getNumSamples();
    int modIdx = 0;
    bool processed[M_MAX] = { false, false };
    // prepare input & process audio
    for (auto &m : modules_) {
        processed[modIdx] = false;
        if (!m.lock_.test_and_set()) {
            processed[modIdx] = true;
            auto &plugin = m.plugin_;
            if (plugin) {
                int chOffset = modIdx * MAX_IN;
                int nCh = m.descriptor_->inputChannelNames.size();
                for (unsigned i = 0; i < nCh && i < MAX_IN; i++) {
                    m.audioBuffer_.copyFrom(i, 0, buffer.getReadPointer(i + chOffset), n);
                }

                float *const *buffers = m.audioBuffer_.getArrayOfWritePointers();
                // juce has changed, to using a const pointer to float*
                // now inconsistent with ssp sdk, but will work fine
                plugin->process((float **)buffers, nCh, n);
            }
        }
        modIdx++;
        // note: we cannot copy output yet, since this will overwrite input for next module!
    }

    // now write the audio back out
    modIdx = 0;
    for (auto &m : modules_) {
        if (processed[modIdx]) {
            auto &plugin = m.plugin_;
            if (plugin) {
                int chOffset = modIdx * MAX_OUT;
                int nCh = m.descriptor_->outputChannelNames.size();
                for (unsigned i = 0; i < MAX_OUT; i++) {
                    if (i < nCh) {
                        buffer.copyFrom(i + chOffset, 0, m.audioBuffer_.getReadPointer(i), n);
                    } else {
                        // zero output where not enough channels
                        buffer.applyGain(i + chOffset, 0, n, 0.0f);
                    }
                }
            }
            m.lock_.clear();
        }
        modIdx++;
    }
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    BaseProcessor::onInputChanged(i, b);
    if (i >= MAX_IN) return;

    int midx = i / MAX_IN;
    int ch = i % MAX_IN;
    auto plugin = modules_[midx].plugin_;
    if (!plugin) return;

    if (ch < modules_[midx].descriptor_->inputChannelNames.size()) { plugin->inputEnabled(ch, b); }
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    BaseProcessor::onOutputChanged(i, b);
    if (i >= MAX_OUT) return;

    int midx = i / MAX_OUT;
    int ch = i % MAX_OUT;
    auto plugin = modules_[midx].plugin_;
    if (!plugin) return;

    if (ch < modules_[midx].descriptor_->outputChannelNames.size()) { plugin->outputEnabled(ch, b); }
}

static constexpr int checkBytes = 0x1FF1;
static constexpr int protoVersion = 0x0002;

void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    // store plugin information
    MemoryOutputStream outStream(destData, false);

    outStream.writeInt(checkBytes);
    outStream.writeInt(protoVersion);

    int i = 0;
    for (auto &m : modules_) {
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
    loadSupportedModules();

    MemoryBlock srcData(data, sizeInBytes);
    MemoryInputStream inputStream(data, sizeInBytes, false);

    int check = inputStream.readInt();
    if (check != checkBytes) return;
    int proto = inputStream.readInt();
    if (proto != protoVersion) return;

    for (int i = 0; i < M_MAX; i++) {
        int check = inputStream.readInt();
        if (check != checkBytes) { return; }

        String fname = inputStream.readString();
        int size = inputStream.readInt();

        if (!fname.isEmpty() && size > 0) {
            MemoryBlock moduleData;
            moduleData.setSize(size);
            inputStream.read(moduleData.getData(), size);

            while (!requestModuleChange(0,i, fname.toStdString())) {}
            auto &plugin = modules_[i].plugin_;
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
