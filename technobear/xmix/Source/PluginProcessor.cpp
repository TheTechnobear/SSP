
#include "PluginProcessor.h"

#include "Matrix.h"
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

    for (auto &track : tracks_) { prepareTrack(track, sampleRate, samplesPerBlock); }
}

void PluginProcessor::prepareTrack(Track &track, double sampleRate, int samplesPerBlock) {
    track.prepare(sampleRate, samplesPerBlock);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    auto n = buffer.getNumSamples();

    AudioSampleBuffer threadBuffers[4];

    for(auto& tBuf : threadBuffers) {
        tBuf = buffer;
    }


    // process thread
    int trackIdx = 0;
    for (auto &track : tracks_) {
        auto& threadBuf = threadBuffers[trackIdx];
        threadBuf = buffer;
        processTrack(track, threadBuf);
        trackIdx++;
    }

    // pluginprocessr thread
    // once all process done... sum outputs for mix
    buffer.applyGain(0.0f);  // zero it out
    for (int i = 0; i < O_MAX; i++) {
        trackIdx = 0;
        for (auto &track : tracks_) {
            auto& threadBuf = threadBuffers[trackIdx];
            threadBuf = buffer;

            rmsData_[trackIdx][i].process(threadBuf, i);
            static constexpr float MIX_CHANNEL_GAIN = 1.0f / float(MAX_TRACKS);
            buffer.addFrom(i, 0, threadBuf, i, 0, n, MIX_CHANNEL_GAIN);
            trackIdx++;
        }
    }
}

void PluginProcessor::processTrack(Track &track, AudioSampleBuffer &ioBuffer) {
    for (auto &track : tracks_) { track.process(ioBuffer); }
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    BaseProcessor::onInputChanged(i, b);
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    BaseProcessor::onOutputChanged(i, b);
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

    int trackIdx = 0;
    for (auto &track : tracks_) {
        outStream.writeInt(trackIdx);
        track.getStateInformation(outStream);
        trackIdx++;
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

    int trackIdx = 0;
    for (auto &track : tracks_) {
        int check = inputStream.readInt();
        if (check == trackIdx) { track.setStateInformation(inputStream); }
        trackIdx++;
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    static constexpr bool useSysEditor = false, defaultDraw = false;
    return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI(), useSysEditor, defaultDraw);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
