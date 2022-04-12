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

    for (int i = 0; i < CI_MAX; i++) {
        float value = powf(2.0f, -i);
        clockInDivMults_.push_back(value);
    }
    jassert(clockInDivMults_.size() == CI_MAX);

    for (int i = 0; i < CO_MAX; i++) {
        float value = powf(2.0f, CO_X1 - i);
        clockOutDivMults_.push_back(value);
    }
    jassert(clockOutDivMults_.size() == CO_MAX);
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::DivParam::DivParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    val(*apvt.getParameter(getPID(pre, sn, ID::val))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    source(*apvt.getParameter(ID::source)),
    clkindiv(*apvt.getParameter(ID::clkindiv)),
    bpm(*apvt.getParameter(ID::bpm)),
    midippqn(*apvt.getParameter(ID::midippqn)),
    usetrigs(*apvt.getParameter(ID::usetrigs)) {
    for (unsigned i = 0; i < MAX_CLK_OUT; i++) {
        divisions_.push_back(std::make_unique<DivParam>(apvt, ID::div, i));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    StringArray source;
    source.add("Internal");
    source.add("Clk In");
    source.add("Midi");
    jassert(source.size() == SRC_MAX);

    StringArray clkInDivs;
    clkInDivs.add("1");
    clkInDivs.add("1/2");
    clkInDivs.add("1/4");
    clkInDivs.add("1/8");
    clkInDivs.add("1/16");
    clkInDivs.add("1/32");
    clkInDivs.add("1/64");

    jassert(clkInDivs.size() == CI_MAX);

    StringArray midippqn;
    midippqn.add("24");
    midippqn.add("48");
    midippqn.add("96");
    midippqn.add("192");
    jassert(midippqn.size() == MPPQN_MAX);

    StringArray clkOutDivs;
    clkOutDivs.add("x 64");
    clkOutDivs.add("x 32");
    clkOutDivs.add("x 16");
    clkOutDivs.add("x 8");
    clkOutDivs.add("x 4");
    clkOutDivs.add("x 2");
    clkOutDivs.add("1");
    clkOutDivs.add("/ 2");
    clkOutDivs.add("/ 4");
    clkOutDivs.add("/ 8");
    clkOutDivs.add("/ 16");
    clkOutDivs.add("/ 32");
    clkOutDivs.add("/ 64");
    jassert(clkOutDivs.size() == CO_MAX);


    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::source, "Clock", source, 0));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::clkindiv, "Clk Div", clkInDivs, CI_1d4));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::bpm, "Int BPM", 1.0f, 360, 120.0f));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::midippqn, "Midi PPQN", midippqn, MPPQN_24));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::usetrigs, "UseTrigs", false));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::div, "Divisions", ID::separator);
    for (unsigned sn = 0; sn < MAX_CLK_OUT; sn++) {
        char ar[2];
        ar[0] = 'A' + sn;
        ar[1] = 0;
        String desc = "Div " + String(ar);
        sg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getPID(ID::div, sn, ID::val), desc, clkOutDivs, (CO_X1 + sn) % clkOutDivs.size()));
    }
    params.add(std::move(sg));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "Clk In",
        "Reset",
        "Run",
        "Midi Clk"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Clk A",
        "Clk B",
        "Clk C",
        "Clk D",
        "Clk E",
        "Clk F",
        "Clk G",
        "Clk H",
        "Reset",
        "Run"
    };

    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    sampleRate_ = newSampleRate;
    jassert(sampleRate_ != 0);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    static constexpr float trigLevel = 0.5f;
    unsigned sz = buffer.getNumSamples();

    auto src = Source(normValue(PluginProcessor::params_.source));

    bool src_cv = src == SRC_CLKIN;
    bool src_midi = src == SRC_MIDI;
    bool src_internal = src == SRC_INTERNAL;
    bool forceUpdate = false;
    bool reset = false;
    bool runStateChange = false;
    unsigned clkN = 0;

    // first see if anything has changed from most significant.
    if (src != source_) {
        source_ = src;
        forceUpdate = true;
    }

    auto clockInDiv = ClkInDiv(normValue(params_.clkindiv));
    if (clockInDiv != clockInDiv_) {
        clockInDiv_ = clockInDiv;
        forceUpdate = true;
    }

    if (src_cv) {
        if (forceUpdate) {
            useTrigs_ = params_.usetrigs.getValue() > 0.5f;
            float samples = 0.0f;
            calcClkInSampleTarget(lastSampleCount_, clockInDiv_, samples);
            unsigned trigs = clockInDivMults_[clockInDiv_] * 4.0f;
            setClockTargets(samples, trigs, useTrigs_);
            reset = true;
        }
    } else if (src_internal) {
        auto bpm = normValue(params_.bpm);
        if (forceUpdate || bpm != bpm_) {
            useTrigs_ = false;
            unsigned trigs = 1;
            float samples = 0.0f;
            bpm_ = bpm;
            calcInternalSampleTarget(sampleRate_, clockInDiv_, bpm_, samples);
            setClockTargets(samples, trigs, useTrigs_);
            reset = true;
        }

    } else if (src_midi) {
        auto ppqn = MidiPPQN(normValue(params_.midippqn));
        if (forceUpdate || ppqn != ppqn_) {
            // todo: midi testing!
            useTrigs_ = params_.usetrigs.getValue() > 0.5f;
            ppqn_ = ppqn;
            float samples = 0.0f;
            calcMidiSampleTarget(lastSampleCount_, clockInDiv_, ppqn_, samples);
            unsigned trigs = midiPPQNRate_[ppqn] * clockInDivMults_[clockInDiv_] * 4.0f;
            setClockTargets(samples, trigs, useTrigs_);
            reset = true;
        }
    }

    if (toggleUseTrigs_) {
        toggleUseTrigs_ = false;
        auto src = Source(normValue(PluginProcessor::params_.source));
        bool nut = !useTrigs_ && (src == SRC_CLKIN || src == SRC_MIDI);
        if (nut != useTrigs_) {
            useTrigs_ = nut;
            for (auto &clk: clocks_) {
                clk.useTrigs(useTrigs_);
            }
        }
        params_.usetrigs.setValueNotifyingHost(useTrigs_);
    }

    clkN = 0;
    // set clock multipliers
    for (auto &clk: clocks_) {
        auto clkDiv = ClkOutDiv(normValue(params_.divisions_[clkN]->val));
        float mult = clockOutDivMults_[clkDiv];
        if (mult != clk.multiplier()) clk.multiplier(mult);
        clkN++;
    }

    if (resetRequest_) {
        resetRequest_ = false;
        reset = true;
    }

    if (toggleRunRequest_) {
        toggleRunRequest_ = false;
        runStateChange = true;
        runState_ = !runState_;
        reset = runState_ ? true : reset;  // reset to sync clock
    }

    if (reset) {
        // TODO: ? should reset be sync'd to trigger, since this would keep useTrigs in sync
        // need to consider how this is done...
        auto &clkTrig = clkTrigTime_[O_RESET];
        clkTrig = clockTrigTime;
        for (Clock &clk: clocks_) {
            clk.reset();
        }
    }

    // ok, now we can do the work!
    for (unsigned s = 0; s < sz; s++) {
        float cv[I_MAX];
        float trig[I_MAX];
        for (unsigned i = 0; i < I_MAX; i++) {
            cv[i] = buffer.getSample(i, s);
            trig[i] = cv[i] > trigLevel && lastCv_[i] < trigLevel;
            lastCv_[i] = cv[i];
        }

        bool runStateReset = false;
        if (inputEnabled[I_RUN]) {
            bool runState = cv[I_RUN] > trigLevel;
            if (runState_ != runState) {
                runStateChange = true;
                runStateReset = runState;
                runState_ = runState;
            }
        }

        if (trig[I_RESET] || runStateReset) {
            auto &clkTrig = clkTrigTime_[O_RESET];
            clkTrig = clockTrigTime;
            for (Clock &clk: clocks_) {
                clk.reset();
            }
        }


        // tick clocks, and see if they fire
        clkN = 0;
        for (Clock &clk: clocks_) {
            auto &clkTrig = clkTrigTime_[O_CLK_1 + clkN];
            auto &uiTrig = uiTrigTime_[O_CLK_1 + clkN];
            if (runState_) {
                bool trigFired = false, smpFired = false;
                if (src_cv && trig[I_CLK]) trigFired = clk.trigTick();
                if (src_midi && trig[I_MIDICLK]) trigFired = clk.trigTick();
                smpFired = clk.sampleTick();
                bool fired = clk.useTrigs() ? trigFired : smpFired;
                if (fired) {
                    clkTrig = clockTrigTime;
                    uiTrig = uiTrigTime;
                }
            } //running

            buffer.setSample(O_CLK_1 + clkN, s, clkTrig > 0);
            clkTrig -= clkTrig > 0 ? 1 : 0;
            uiTrig -= uiTrig > 0 ? 1 : 0;
            clkN++;
        } // for each clock


        { // reset output
            auto &clkTrig = clkTrigTime_[O_RESET];
            buffer.setSample(O_RESET, s, clkTrig > 0);
            clkTrig -= clkTrig > 0 ? 1 : 0;
        }

        // run output
        buffer.setSample(O_RUN, s, runState_);

        // do we need to recalc sample targets?
        if (src_cv && trig[I_CLK]) {
            lastSampleCount_ = sampleCount_;
            float samples = 0.0f;
            calcClkInSampleTarget(lastSampleCount_, clockInDiv_, samples);
            updateClockSampleTargets(samples);
            sampleCount_ = 0;
        }

        if (src_midi && trig[I_MIDICLK]) {
            lastSampleCount_ = sampleCount_;
            float samples = 0.0f;
            calcMidiSampleTarget(lastSampleCount_, clockInDiv_, ppqn_, samples);
            updateClockSampleTargets(samples);
            sampleCount_ = 0;
        }
        sampleCount_++;
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


/// Clock helpers - just setting values etc
bool PluginProcessor::isUsingTrigs() {
    return useTrigs_;
}

void PluginProcessor::getClockStates(bool *states) {
    for (unsigned i = 0; i < MAX_CLK_OUT; i++) {
        states[i] = uiTrigTime_[O_CLK_1 + i] > 0;
    }
}


void PluginProcessor::setClockTargets(unsigned samples, unsigned trigs, bool useTrigs) {
    for (Clock &clk: clocks_) {
        clk.targetSamples(samples);
        clk.useTrigs(useTrigs);
        clk.targetTrigs(trigs);
    }
}


void PluginProcessor::updateClockSampleTargets(unsigned samples) {
    for (Clock &clk: clocks_) {
        clk.targetSamples(samples);
    }
}
/// CLOCK calculations etc

void PluginProcessor::calcInternalSampleTarget(const float &sampleRate,
                                               const ClkInDiv &div,
                                               const float &bpm,
                                               float &samples) {
    // when:
    // change in sample rate
    // bpm change.
    //
    // how:
    // sample rate = samples per second
    // bpm = quarter notes per minute.
    // so SR*60 = samples per minute, div bpm = samplers per beat (=1/4 note)
    samples = ((sampleRate * 60.0f) / bpm) * clockInDivMults_[div] * 4.0f;
}

void PluginProcessor::calcMidiSampleTarget(const float &lastClock,
                                           const ClkInDiv &div,
                                           const MidiPPQN &ppqn,
                                           float &samples) {
    // when:
    // every time we get a new midi trig (so new clk value)
    // ppqn change
    //
    // how:
    // ppqn = number of pulses per quarter note
    // samples per quarter note = lastClock * ppqn
    samples = (lastClock * midiPPQNRate_[ppqn]) * clockInDivMults_[div] * 4.0f;
}

void PluginProcessor::calcClkInSampleTarget(const float &lastClock,
                                            const ClkInDiv &div,
                                            float &samples) {
    // when:
    // every time we get a new clock trig (so new clk value)
    // change in clk in div
    //
    // how:
    // lastClk = target
    samples = lastClock * clockInDivMults_[div] * 4.0f;
}
