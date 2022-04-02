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
    pitch(*apvt.getParameter(ID::pitch)),
    first(*apvt.getParameter(ID::first)),
    centre(*apvt.getParameter(ID::centre)),
    spread(*apvt.getParameter(ID::spread)) {
    for (unsigned hid = 0; hid < MAX_HARMONICS; hid++) {
        auto harmonic = std::make_unique<Harmonic>(apvt, hid);
        harmonics_.push_back(std::move(harmonic));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -48.0f, +48.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::first, "H.First", 1.0f, 16.0f, 1.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::centre, "T.Centre", 1.0f, 16.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::spread, "T.Spread", 0.5f, 16.0f, 0.5f));

    auto harmonics = std::make_unique<AudioProcessorParameterGroup>(ID::harmonics,
                                                                    String(ID::harmonics),
                                                                    ID::separator);
    for (unsigned hid = 0; hid < MAX_HARMONICS; hid++) {
        harmonics->addChild(std::make_unique<ssp::BaseFloatParameter>(getAmpPid(hid), "Amp " + String(hid), 0, 1.0f, hid == 0 ? 1.0f : 0.0f));
    }
    params.add(std::move(harmonics));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "VOct",
        "Amp",
        "Centre",
        "Spread",
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
        normValue(params_.pitch)
        + cv2Pitch(buffer.getSample(I_VOCT, 0))
        + baseNote;
    float freq = daisysp::mtof(pitch);

    oscillator_.SetFirstHarmIdx(normValue(params_.first));

    // control rate
    float cvCentre = (buffer.getSample(I_CENTRE,0)  * 8.0f) +  8.0f; // 0..16
    float cvSpread = (buffer.getSample(I_SPREAD,0)  * 8.0f) +  8.0f; // 0..16
    float centre = daisysp::fclamp((normValue(params_.centre) - 1.0f) + cvCentre , 0.0f, 16.0f)  * -1.0f;
    float spread = daisysp::fclamp(normValue(params_.spread) + cvSpread, 0.1,16.0f);

    float amps[MAX_HARMONICS];
    float sum = 0.0f;
    float hspread = spread / 8.0f;
    static constexpr float rHarmonics = 1.0f / float(MAX_HARMONICS);
    for (unsigned h = 0; h < MAX_HARMONICS; h++) {
        auto &harmonic = *params_.harmonics_[h];
        float ph = (h + centre) * rHarmonics;
        float sph = (ph / hspread);
        float tph = daisysp::fclamp(sph, -1.0f, 1.0f);
        float tilt = daisysp::fmax(cosf(tph * PI_F), 0);
        amps[h] = tilt;
        amps[h] += harmonic.amp.getValue(); // 0..1 is fine
        sum += amps[h];
    }


    float rsum = sum > 0.0f ? 1.0f / sum : 1.0f;
    for (unsigned h = 0; h < MAX_HARMONICS; h++) {
        amps[h] = amps[h] * rsum;
    }

    oscillator_.SetFreq(freq);
    oscillator_.SetAmplitudes(amps);

    bool iAmp = isInputEnabled(I_AMP);
    for (int s = 0; s < sz; s++) {
        float amp = iAmp ? buffer.getSample(I_AMP, s) : 1.0f;
        buffer.setSample(O_MAIN, s, oscillator_.Process() * amp);
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
