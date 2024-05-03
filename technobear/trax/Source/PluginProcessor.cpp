
#include "PluginProcessor.h"

#include "Matrix.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"
#include "ssp/Log.h"

PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();

    int trackIdx = 0;
    for (auto &track : tracks_) {
        // create thread for each track, place into trackThreads_
        trackThreads_.emplace_back([this, trackIdx] {
            bool running = true;
            while (running) {
                std::unique_lock<std::mutex> lk(tracks_[trackIdx].mutex_);
                tracks_[trackIdx].cv_.wait(lk, [this, trackIdx] { return tracks_[trackIdx].ready_; });
                tracks_[trackIdx].ready_ = false;
                if (!shouldExit()) {
                    processTrack(tracks_[trackIdx], threadBuffers_[trackIdx]);
                } else {
                    running = false;
                }
                tracks_[trackIdx].processed_ = true;
                lk.unlock();
                tracks_[trackIdx].cv_.notify_one();
            }
        });
        trackIdx++;
    }
#ifndef __APPLE__
    for (int i = 0; i < trackThreads_.size(); i++) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        int cpu = i % 2 == 0 ? 2 : 3;
        CPU_SET(cpu, &cpuset);
        int rc = pthread_setaffinity_np(trackThreads_[i].native_handle(), sizeof(cpu_set_t), &cpuset);
        if (rc != 0) {
            ssp::log("Trax : Error calling pthread_setaffinity_np: " + std::to_string(rc));
        } else {
            ssp::log("Trax : Set affinity for dsp thread " + std::to_string(i) + " to CPU " + std::to_string(cpu));
        }
    }
#endif

    for (auto &t : trackThreads_) { t.detach(); }
}

bool PluginProcessor::shouldExit() {
    return shouldExit_;
}

PluginProcessor::~PluginProcessor() {
    shouldExit_ = true;

    for (auto &track : tracks_) {
        std::lock_guard lk(track.mutex_);
        track.ready_ = true;
        track.cv_.notify_one();
    }

    for (auto &t : trackThreads_) {
        if (t.joinable()) t.join();
    }

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
    ssp::log("plugin scan - start");
    Module::scanPlugins(supportedModules_);
    for (auto m : supportedModules_) { ssp::log("module: " + m); }
    ssp::log("plugin scan - send");
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

        std::lock_guard lk(track.mutex_);
        track.processed_ = false;
        track.ready_ = true;
        track.cv_.notify_one();
        trackIdx++;
    }

    for (auto &track : tracks_) {
        std::unique_lock<std::mutex> lk(track.mutex_);
        track.cv_.wait(lk, [&track] { return track.processed_; });
    }

    // pluginprocessr thread
    // once all process done... sum outputs for mix
    buffer.applyGain(0.0f);  // zero it out
    static constexpr float MIX_CHANNEL_GAIN = 1.0f / float(MAX_TRACKS);
    // static constexpr float MIX_CHANNEL_GAIN = 1.0f;
    trackIdx = 0;
    for (auto &track : tracks_) {
        auto &tBuf = threadBuffers_[trackIdx];
        float gain = track.level();

        for (int i = 0; i < O_MAX; i++) {
            tBuf.applyGainRamp(i, 0, n, lastGain_[trackIdx], gain);
            rmsData_[trackIdx][i].process(tBuf, i);
            buffer.addFrom(i, 0, tBuf, i, 0, n, MIX_CHANNEL_GAIN);
        }
        lastGain_[trackIdx] = gain;
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

static const char *TRAX_XML_TAG = "TRAX";
static const char *MODLIST_XML_TAG = "ModuleList";
static const char *TRACKS_XML_TAG = "Tracks";
static const char *TRACK_XML_TAG = "Track";


void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    std::unique_ptr<juce::XmlElement> xmlTrax = std::make_unique<juce::XmlElement>(TRAX_XML_TAG);


    std::unique_ptr<juce::XmlElement> xmlModList = std::make_unique<juce::XmlElement>(MODLIST_XML_TAG);
    for (auto mn : supportedModules_) {
        std::unique_ptr<juce::XmlElement> xmlMod = std::make_unique<juce::XmlElement>("Module");
        xmlMod->setAttribute("name", mn);
        xmlModList->addChildElement(xmlMod.release());
    }
    xmlTrax->addChildElement(xmlModList.release());


    std::unique_ptr<juce::XmlElement> xmlTracks = std::make_unique<juce::XmlElement>(TRACKS_XML_TAG);
    for (auto &track : tracks_) {
        std::unique_ptr<juce::XmlElement> xmlTrack = std::make_unique<juce::XmlElement>(TRACK_XML_TAG);
        track.getStateInformation(*xmlTrack);
        xmlTracks->addChildElement(xmlTrack.release());
    }
    xmlTrax->addChildElement(xmlTracks.release());

    // ssp::log("getStateInformation : " + xmlTrax->toString().toStdString());
    copyXmlToBinary(*xmlTrax, destData);
}


void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    for(auto &track : tracks_) {
        while(!track.requestClearTrack()) {}
    }   


    std::unique_ptr<juce::XmlElement> xmlTrax(getXmlFromBinary(data, sizeInBytes));

    if (xmlTrax != nullptr && xmlTrax->hasTagName(TRAX_XML_TAG)) {
        // ssp::log("setStateInformation : " + xmlTrax->toString().toStdString());
        auto xmlModList = xmlTrax->getChildByName(MODLIST_XML_TAG);
        if (xmlModList != nullptr) {
            supportedModules_.clear();
            for (auto xmlMod : xmlModList->getChildIterator()) {
                auto mn = xmlMod->getStringAttribute("name").toStdString();
                supportedModules_.push_back(mn);
                // ssp::log("setStateInformation : supported module : " + mn);
            }
        } else {
            ssp::log("setStateInformation : no MODLIST_XML_TAG tag");
        }

        auto xmlTracks = xmlTrax->getChildByName(TRACKS_XML_TAG);
        if (xmlTracks != nullptr) {
            int trackIdx = 0;
            for (auto xmlTrack : xmlTracks->getChildIterator()) {
                tracks_[trackIdx].setStateInformation(*xmlTrack);
                trackIdx++;
                if (trackIdx >= MAX_TRACKS) break;
            }
        } else {
            ssp::log("setStateInformation : no TRACKS_XML_TAG tag");
        }
    } else {
        ssp::log("setStateInformation : no TRAX_XML_TAG tag");
    }

    if (supportedModules_.empty()) { scanPlugins(); }
}

void PluginProcessor::savePreset() {
    ssp::log("savePreset : " + requestedPreset_.toStdString());
    presetName_ = requestedPreset_;
    requestPresetSave_ = false;

    File f(presetName_);

    MemoryBlock destData;
    getStateInformation(destData);

    f.replaceWithData(destData.getData(), destData.getSize());
}

void PluginProcessor::loadPreset() {
    ssp::log("loadPreset : " + requestedPreset_.toStdString());
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
#ifdef FORCE_COMPACT_UI
    return new ssp::EditorHost(this, new PluginEditor(*this), true, useSysEditor, defaultDraw);
#else
    return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI(), useSysEditor, defaultDraw);
#endif
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
