
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
    for (unsigned t = 0; t < MAX_TRACKS; t++) { tracks_[t].createDefaultRoute(t, t + MAX_TRACKS); }
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

    for (int i = 0; i < MAX_TRACKS; i++) { threadBuffers_[i].setSize(IO_MAX, samplesPerBlock); }
    for (auto &track : tracks_) { prepareTrack(track, sampleRate, samplesPerBlock); }
}

void PluginProcessor::prepareTrack(Track &track, double sampleRate, int samplesPerBlock) {
    track.prepare(sampleRate, samplesPerBlock);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    auto n = buffer.getNumSamples();

    // process thread
    int trackIdx = 0;
    for (auto &track : tracks_) {
        auto &tBuf = threadBuffers_[trackIdx];
        for (int c = 0; c < I_MAX; c++) { tBuf.copyFrom(c, 0, buffer, c, 0, n); }
        processTrack(track, tBuf);
        trackIdx++;
    }

    // pluginprocessr thread
    // once all process done... sum outputs for mix
    buffer.applyGain(0.0f);  // zero it out
    trackIdx = 0;
    for (auto &track : tracks_) {
        auto &tBuf = threadBuffers_[trackIdx];
        for (int i = 0; i < O_MAX; i++) {
            static constexpr float MIX_CHANNEL_GAIN = 1.0f / float(MAX_TRACKS);
            rmsData_[trackIdx][i].process(tBuf, i);
            buffer.addFrom(i, 0, tBuf, i, 0, n, MIX_CHANNEL_GAIN);
        }
        trackIdx++;
    }
}

void PluginProcessor::processTrack(Track &track, AudioSampleBuffer &ioBuffer) {
    track.process(ioBuffer);
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

void PluginProcessor::savePreset() {
    log("savePreset : " + requestedPreset_.toStdString());
    presetName_ = requestedPreset_;
    requestPresetSave_ = false;

    File f(presetName_);

    MemoryBlock destData;
    getStateInformation(destData);

    f.replaceWithData(destData.getData(), destData.getSize());
}

void PluginProcessor::loadPreset() {
    log("loadPreset : " + requestedPreset_.toStdString());
    presetName_ = requestedPreset_;
    requestPresetLoad_ = false;

    File f(presetName_);
    if (f.exists()) {
        MemoryMappedFile mmf(f, MemoryMappedFile::AccessMode::readOnly, false);
        setStateInformation(mmf.getData(), mmf.getSize());
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    static constexpr bool useSysEditor = false, defaultDraw = false;
    return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI(), useSysEditor, defaultDraw);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
