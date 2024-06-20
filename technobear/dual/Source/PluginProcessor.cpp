
#include "PluginProcessor.h"

#include <dlfcn.h>

#include <fstream>

#include "PluginEditor.h"
#include "ssp/EditorHost.h"
#include "ssp/Log.h"


PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)) {
    init();
}

PluginProcessor::~PluginProcessor() {
    tracks_[0].requestClearTrack();
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


void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);


    tracks_[0].prepare(sampleRate, samplesPerBlock);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    tracks_[0].process(buffer);
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    BaseProcessor::onInputChanged(i, b);
    if (i >= MAX_IN) return;

    int midx = i / MAX_IN;
    int ch = i % MAX_IN;

    auto plugin = tracks_[0].modules_[midx].plugin_;
    auto desc = tracks_[0].modules_[midx].descriptor_;
    if (!plugin || !desc) return;


    if (ch < desc->inputChannelNames.size()) { plugin->inputEnabled(ch, b); }
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    BaseProcessor::onOutputChanged(i, b);
    if (i >= MAX_OUT) return;

    int midx = i / MAX_OUT;
    int ch = i % MAX_OUT;
    auto plugin = tracks_[0].modules_[midx].plugin_;
    auto desc = tracks_[0].modules_[midx].descriptor_;
    if (!plugin || !desc) return;

    if (ch < desc->outputChannelNames.size()) { plugin->outputEnabled(ch, b); }
}

static constexpr int checkBytes = 0x1FF1;
static constexpr int protoVersion = 0x0002;
static const char *DUAL_XML_TAG = "DUAL";
static const char *TRACKS_XML_TAG = "Tracks";
static const char *TRACK_XML_TAG = "Track";


void PluginProcessor::getStateInformation(MemoryBlock &destData) {
    std::unique_ptr<juce::XmlElement> xmlDual = std::make_unique<juce::XmlElement>(DUAL_XML_TAG);

    std::unique_ptr<juce::XmlElement> xmlTracks = std::make_unique<juce::XmlElement>(TRACKS_XML_TAG);
    for (auto &track : tracks_) {
        std::unique_ptr<juce::XmlElement> xmlTrack = std::make_unique<juce::XmlElement>(TRACK_XML_TAG);
        track.getStateInformation(*xmlTrack);
        xmlTracks->addChildElement(xmlTrack.release());
    }
    xmlDual->addChildElement(xmlTracks.release());

    // ssp::log("getStateInformation : " + xmlTrax->toString().toStdString());
    copyXmlToBinary(*xmlDual, destData);
}


void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    while (!tracks_[0].requestClearTrack()) {}

    loadSupportedModules();

    std::unique_ptr<juce::XmlElement> xmlDual(getXmlFromBinary(data, sizeInBytes));

    if (xmlDual != nullptr && xmlDual->hasTagName(DUAL_XML_TAG)) {
        // ssp::log("setStateInformation : " + xmlDual->toString().toStdString());
        auto xmlTracks = xmlDual->getChildByName(TRACKS_XML_TAG);
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
        ssp::log("setStateInformation : no DUAL_XML_TAG tag");
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    static constexpr bool useSysEditor = false, defaultDraw = false;
    return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI(), useSysEditor, defaultDraw);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
