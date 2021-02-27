#include "PluginProcessor.h"
#include "PluginEditor.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {

    memset(lastTrig_, 0, sizeof(lastTrig_));
    memset(lastSig_, 0, sizeof(lastSig_));
    randomGen_.setSeedRandomly();
}


PluginProcessor::~PluginProcessor() {
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    root(*apvt.getParameter(ID::root)),
    scale(*apvt.getParameter(ID::scale)),
    quant(*apvt.getParameter(ID::quant)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    auto &q = quantizer_;
    StringArray tonics;
    for (auto i = 0; i < MAX_TONICS; i++) {
        tonics.add(q.getTonicName(i));
    }
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::root, "Root", tonics, 0));

    StringArray scales;
    for (auto i = 0; i < MAX_SCALES; i++) {
        scales.add(q.getScaleName(i));
    }
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::scale, "Scale", scales, 0));

    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::quant, "Quant", false));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In 1",
        "Trig 1",
        "In 2",
        "Trig 2",
        "In 3",
        "Trig 3",
        "In 4",
        "Trig 4",
        "Scale",
        "Root"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out 1",
        "Trig 1",
        "Out 2",
        "Trig 2",
        "Out 3",
        "Trig 3",
        "Out 4",
        "Trig 4",
        "Scale",
        "Root"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

float PluginProcessor::processCV(float v, unsigned scale, unsigned root) {
    if (params_.quant.getValue() > 0.5f) {
        constexpr float halfSemi = 0.5;
        constexpr bool roundUp = true;
        // cv2pitch, returns fractional semitones e.g 24.0 = C2
        float voct = cv2Pitch(v) + 60.f + (roundUp ? halfSemi : 0.0f); // -5v = 0

        int oct = voct / 12;
        unsigned note = unsigned(voct) % MAX_TONICS;
        // Logger::writeToLog("float " + String(v) + " voct " + String(voct) + " oct " + String(oct) + " note " + String(note));

        quantizer_.quantize(root, scale, oct, note);

        float pv = float(oct * 12.0f) + float(note) - 60.0f;
        float qv = pitch2Cv(pv);
        return qv;
    }
    return v;
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned n = buffer.getNumSamples();

    bool inTrigE[MAX_SIG];
    bool inCvE[MAX_SIG];
    bool outTrigE[MAX_SIG];
    bool outCvE[MAX_SIG];

    for (unsigned i = 0; i < MAX_SIG; i++) {
        unsigned sigo = i * 2;
        inTrigE[i] = inputEnabled[I_TRIG_1 + sigo];
        inCvE[i] = inputEnabled[I_SIG_1 + sigo];
        outTrigE[i] = outputEnabled[O_SIG_1 + sigo];
        outCvE[i] = outputEnabled[O_TRIG_1 + sigo];
    }


    for (unsigned idx = 0; idx < n; idx++) {
        // for each sample
        bool trigAbove = false;
        bool trig[MAX_SIG];
        for (unsigned i = 0; i < MAX_SIG; i++) {
            unsigned sigo = i * 2;
            bool trigged = false;
            if (inTrigE[i]) {
                // trig in enabled

                trig[i] = buffer.getSample(I_TRIG_1 + sigo, idx) > 0.5f;
                if (trig[i] != lastTrig_[i] && trig[i]) {
                    trigged = true;
                    trigAbove = true;
                } else {
                    trigAbove = false;
                }
            } else {
                // trig not enabled
                if (i != 0) trig[i] = trig[i - 1];
                else trig[i] = 0;

                if (trigAbove) {
                    trigged = true;
                }
            } // if trig enabled

            if (trigged) {
                // triggered...
                float v = 0.0f;
                if (inCvE[i]) {
                    v = buffer.getSample(I_SIG_1 + sigo, idx);
                } else {
                    // no cv input... use random
                    v = (randomGen_.nextFloat() * 2.0f) - 1.0f;
                }

                unsigned root = constrain(params_.root.convertFrom0to1(params_.root.getValue() + buffer.getSample(I_ROOT, idx)),
                                          0.0f, MAX_TONICS);
                unsigned scale = constrain(params_.scale.convertFrom0to1(params_.scale.getValue() + +buffer.getSample(I_SCALE, idx)),
                                           0.0f, MAX_SCALES);

                v = processCV(v, scale, root);
                lastSig_[i] = v;
            } //if triggered

            lastTrig_[i] = trig[i];
            buffer.setSample(O_TRIG_1 + sigo, idx, trig[i]);
            buffer.setSample(O_SIG_1 + sigo, idx, lastSig_[i]);
        } // for sig pair
    } // for each sample
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

