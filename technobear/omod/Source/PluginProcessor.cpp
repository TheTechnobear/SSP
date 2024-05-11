#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "PluginMiniEditor.h"
#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

#define MAX_FREQ 24000.0f
#define MIN_FREQ (1.0f / 600.0f)
// min freq only used for cv freq

PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

String getSlaveOscPid(unsigned tid) {
    return String(ID::slaveosc) + String(ID::separator) + String(tid);
}

String getSlaveOscParamId(unsigned tid, StringRef id) {
    return getSlaveOscPid(tid) + String(ID::separator) + id;
}

PluginProcessor::SlaveOscParams::SlaveOscParams(AudioProcessorValueTreeState &apvt, unsigned id)
    : id_(id),
      pid_(getSlaveOscPid(id)),
      wave(*apvt.getParameter(getSlaveOscParamId(id, ID::wave))),
      ratio(*apvt.getParameter(getSlaveOscParamId(id, ID::ratio))),
      amp(*apvt.getParameter(getSlaveOscParamId(id, ID::amp))),
      phase(*apvt.getParameter(getSlaveOscParamId(id, ID::phase))) {
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt)
    : wave(*apvt.getParameter(ID::wave)),
      freq(*apvt.getParameter(ID::freq)),
      amp(*apvt.getParameter(ID::amp)),
      phase(*apvt.getParameter(ID::phase)),
      lfo(*apvt.getParameter(ID::lfo)) {
    for (unsigned oid = 0; oid < MAX_S_OSC; oid++) {
        auto sosc = std::make_unique<SlaveOscParams>(apvt, oid);
        slaveOscsParams_.push_back(std::move(sosc));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    StringArray waves;
    waves.add("Sine");
    waves.add("Triangle");
    waves.add("Saw");
    waves.add("Ramp");
    waves.add("Square");
    waves.add("Blep Tri");
    waves.add("Blep Saw");
    waves.add("Blep Sqr");
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::wave, "Wave", waves, 0));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::freq, "Freq", 0.0f, MAX_FREQ, 100.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::amp, "Amp", 0.0f, 4.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::phase, "Phase", 0.0f, 360.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::lfo, "Lfo", true));

    auto taps = std::make_unique<AudioProcessorParameterGroup>(ID::slaveosc, String(ID::slaveosc), ID::separator);
    for (unsigned oid = 0; oid < MAX_S_OSC; oid++) {
        char la[2];
        la[0] = 'A' + oid;
        la[1] = 0;
        String desc = "Sub " + String(la) + " ";
        auto tap =
            std::make_unique<AudioProcessorParameterGroup>(getSlaveOscPid(oid), getSlaveOscPid(oid), ID::separator);
        tap->addChild(
            std::make_unique<ssp::BaseChoiceParameter>(getSlaveOscParamId(oid, ID::wave), desc + "Wave", waves, 0));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getSlaveOscParamId(oid, ID::ratio), desc + "Ratio",
                                                                0.0f, 10.0f, (1.0f / (oid + 1))));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getSlaveOscParamId(oid, ID::amp), desc + "Amp", 0.0f,
                                                                4.0f, 1.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getSlaveOscParamId(oid, ID::phase), desc + "Phase",
                                                                0.0f, 360.0f, 0.0f));
        taps->addChild(std::move(tap));
    }
    params.add(std::move(taps));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = { "Freq", "Reset", "Clock", "VOct" };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = { "Main",     "Out A", "Out B", "Out C", "Out D", "Out E", "Out F", "Out G",
                                        "EoC Main", "EoC A", "EoC B", "EoC C", "EoC D", "EoC E", "EoC F", "EoC G" };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    mainOsc_.Init(newSampleRate);
    mainOsc_.SetFreq(0.0f);
    for (int oid = 0; oid < MAX_S_OSC; oid++) {
        slaveOscs_[oid].osc_.Init(newSampleRate);
        slaveOscs_[oid].osc_.SetFreq(0.0f);
    }
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();
    float freqmult = (params_.lfo.getValue() > 0.5f) ? 0.01f : 1.0f;

    float mainfreq = normValue(params_.freq) * freqmult;
    float freqRange = mainfreq;
    float mainwave = normValue(params_.wave);
    float mainamp = normValue(params_.amp);
    float mainphase = params_.phase.getValue();
    bool usingClock = isInputEnabled(I_CLOCK);

    mainOsc_.SetWaveform(mainwave);
    mainOsc_.SetAmp(mainamp);

    if (mainphase != mainPhase_) {
        float phaseAdd = mainphase - mainPhase_;
        if (phaseAdd < 0.0f) phaseAdd += 1.0f;
        mainPhase_ = mainphase;
        mainOsc_.PhaseAdd(phaseAdd);
    }

    struct {
        float ratio_ = 0.0f;
    } slavevalues[MAX_S_OSC];

    // control rate handling
    for (int oid = 0; oid < MAX_S_OSC; oid++) {
        if (!isOutputEnabled(O_OUT_A + oid)) continue;

        auto &soscparams = params_.slaveOscsParams_[oid];
        auto &sosc = slaveOscs_[oid];
        float ratio = normValue(soscparams->ratio);
        float wave = normValue(soscparams->wave);
        float amp = normValue(soscparams->amp);
        float phase = soscparams->phase.getValue();


        slavevalues[oid].ratio_ = ratio;
        sosc.osc_.SetWaveform(wave);
        sosc.osc_.SetAmp(amp);

        if (sosc.phase_ != phase) {
            float phaseAdd = phase - sosc.phase_;
            if (phaseAdd < 0.0f) phaseAdd += 1.0f;
            sosc.phase_ = phase;
            sosc.osc_.PhaseAdd(phaseAdd);
        }
    }

    if (!usingClock) {
        lastClock_ = 0.0f;
        clockSampleCnt_ = 0;
        clockFreq_ = 0.0f;
    }

    static constexpr float trigLevel = 0.5f;
    // sample rate processing
    for (int s = 0; s < sz; s++) {
        float fin = buffer.getSample(I_FREQ, s);
        float clockSmp = buffer.getSample(I_CLOCK, s);
        float resetSmp = buffer.getSample(I_RESET, s);
        bool resetTrig = requestReset_;

        requestReset_ = false;

        if (resetSmp > trigLevel && lastReset_ <= trigLevel) { resetTrig = true; }
        lastReset_ = resetSmp;

        float mainoscfreq = mainfreq;

        if (isInputEnabled(I_VOCT)) {
            float voct = buffer.getSample(I_VOCT, s);
            float pitch = cv2Pitch(voct);
            mainoscfreq += daisysp::mtof(pitch + 60.0f);
            mainoscfreq = constrain(mainoscfreq, 0.0f, MAX_FREQ);
        }

        if (isInputEnabled(I_FREQ)) {
            mainoscfreq += (fin * freqRange);
            mainoscfreq = constrain(mainoscfreq, MIN_FREQ, MAX_FREQ);
        }

        if (usingClock) {
            if (clockSmp > trigLevel && lastClock_ <= trigLevel) {
                if (clockSampleCnt_ > 0)
                    clockFreq_ = getSampleRate() / clockSampleCnt_;
                else
                    clockFreq_ = 0.0f;
                clockSampleCnt_ = 0;
            }
            lastClock_ = clockSmp;
            clockSampleCnt_++;
            mainoscfreq = clockFreq_;
        }

        if (mainoscfreq != mainFreq_) {
            mainFreq_ = mainoscfreq;
            mainOsc_.SetFreq(mainFreq_);
        }

        if (resetTrig) mainOsc_.Reset(mainPhase_);
        buffer.setSample(O_OUT_MAIN, s, mainOsc_.Process());
        buffer.setSample(O_EOC_MAIN, s, mainOsc_.IsEOC());
        for (int oid = 0; oid < MAX_S_OSC; oid++) {
            if (!(isOutputEnabled(O_OUT_A + oid) || (isOutputEnabled(O_EOC_A + oid)))) { continue; };

            auto &sosc = slaveOscs_[oid];
            auto &soscparam = slavevalues[oid];

            float freq = mainoscfreq * soscparam.ratio_;
            if (freq != sosc.freq_) {
                sosc.freq_ = freq;
                sosc.osc_.SetFreq(sosc.freq_);
            }

            if (resetTrig) sosc.osc_.Reset(sosc.phase_);

            buffer.setSample(O_OUT_A + oid, s, sosc.osc_.Process());
            buffer.setSample(O_EOC_A + oid, s, sosc.osc_.IsEOC());
        }
    }

    for (int oid = 0; oid < MAX_S_OSC; oid++) {
        if (!(isOutputEnabled(O_OUT_A + oid) || (isOutputEnabled(O_EOC_A + oid)))) {
            // clear disabled outputs
            buffer.applyGain(O_OUT_A + oid, 0, sz, 0.0f);
            buffer.applyGain(O_EOC_A + oid, 0, sz, 0.0f);
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
#ifdef FORCE_COMPACT_UI
    return new ssp::EditorHost(this, new PluginMiniEditor(*this), true);
#else
    if (useCompactUI()) {
        return new ssp::EditorHost(this, new PluginMiniEditor(*this), useCompactUI());

    } else {
        return new ssp::EditorHost(this, new PluginEditor(*this), useCompactUI());
    }
#endif
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
