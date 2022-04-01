#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

String getHarmonicPid(unsigned id) {
    return String(ID::harmonics) + String(ID::separator) + String(id);
}

String getAmpPid(unsigned hid) {
    return getHarmonicPid(hid) + String(ID::separator) + String(ID::amp);

}

PluginProcessor::Harmonic::Harmonic(AudioProcessorValueTreeState &apvt, unsigned id) :
    id_(id), pid_(getHarmonicPid(id)),
    amp(*apvt.getParameter(getAmpPid(id))) {
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    pitch(*apvt.getParameter(ID::pitch)) {
    for (unsigned hid = 0; hid < MAX_HARMONICS; hid++) {
        auto harmonic = std::make_unique<Harmonic>(apvt, hid);
        harmonics_.push_back(std::move(harmonic));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -48.0f, +48.0f, 0.0f));

    auto harmonics = std::make_unique<AudioProcessorParameterGroup>(ID::harmonics,
                                                                    String(ID::harmonics),
                                                                    ID::separator);
    for (unsigned hid = 0; hid < MAX_HARMONICS; hid++) {
        harmonics->addChild(std::make_unique<ssp::BaseFloatParameter>(getAmpPid(hid), "Amp " + String(hid), 0, 1.0f, 0.0f));
    }
    params.add(std::move(harmonics));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "VOct",
        "Amp",
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Main",
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    oscillator_.Init(newSampleRate);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    static constexpr float baseNote = 60.0f;
    float pitch =
        params_.pitch.convertFrom0to1(params_.pitch.getValue())
        + cv2Pitch(buffer.getSample(I_VOCT, 0))
        + baseNote;
    float freq = daisysp::mtof(pitch);

//    oscillator_.SetAmplitudes(float[]);
//    oscillator_.SetFirstHarmIdx(0);
    unsigned h=0;
    for(auto &harmonic : params_.harmonics_) {
        oscillator_.SetSingleAmp(harmonic->amp.getValue(),h);
        h++;
    }

    oscillator_.SetFreq(freq);
    for(int s=0;s<sz;s++) {
        buffer.setSample(O_MAIN,s,oscillator_.Process());
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
