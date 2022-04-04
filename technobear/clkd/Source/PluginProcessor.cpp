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
    midippqn(*apvt.getParameter(ID::midippqn)) {
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
    clkOutDivs.add("1/2");
    clkOutDivs.add("1/4");
    clkOutDivs.add("1/8");
    clkOutDivs.add("1/16");
    clkOutDivs.add("1/32");
    clkOutDivs.add("1/64");
    clkOutDivs.add("1/128");
    jassert(clkOutDivs.size() == CO_MAX);


    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::source, "Clock", source, 0));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::clkindiv, "Clk Div", clkInDivs, CI_D4));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::bpm, "Int BPM", 1.0f, 360, 120.0f));
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::midippqn, "Midi PPQN", midippqn, MPPQN_24));

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
        "Clk H"
    };

    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

//void PluginProcessor::updateClkOut(float clk) {
//    for (unsigned i = 0; i < CO_MAX; i++) {
//        if (i < CO_X1)
//            clkOutDiv_[i] = clk * powf(2.0f, i - CO_X1);
//        else if (i > CO_X1)
//            clkOutDiv_[i] = clk / powf(2.0f, CO_X1 - i);
//        else
//            clkOutDiv_[i] = clk;
//    }
//}



void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    sampleRate_ = newSampleRate;
}

void PluginProcessor::setClockSampleTargets(unsigned samples) {
    for (Clock &clk: clocks_) {
        clk.targetSamples(samples);
    }
}
void PluginProcessor::resetClocks() {
    for (Clock &clk: clocks_) {
        clk.reset();
    }
}

float PluginProcessor::calcMultiplier(ClkInDiv& cid, ClkOutDiv& cod) {
    //TODO
    return 0.0f;
}
float  PluginProcessor::calcMultiplier(Clock& clk, ClkInDiv& cid) {
    // TODO
    return 0.0f;
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    static constexpr float trigLevel = 0.5f;
    unsigned sz = buffer.getNumSamples();

    auto src = Source(normValue(PluginProcessor::params_.source));
    auto ppqn = unsigned(normValue(params_.midippqn));

    bool src_cv = src == SRC_CLKIN;
    bool src_midi = src == SRC_MIDI;
    bool src_internal = src == SRC_INTERNAL;
    bool forceUpdate = false;


    // first see if anything has changed from most significant.
    if(src!= source_) {
        source_ = src;
        for (Clock &clk: clocks_) {
            switch(src) {
                case SRC_CLKIN : {
                    // TODO use trigs multiple > clock div
                    clk.useTrigs(false);
                    forceUpdate = true;
                    break;
                }
                case SRC_INTERNAL : {
                    // never use trigs ..  we dont get them ;)
                    clk.useTrigs(false);
                    forceUpdate = true;
                    break;
                }
                case SRC_MIDI : {
                    // TODO use trigs if PPQN / clock div allows
                    clk.useTrigs(false);
                    forceUpdate = true;
                    break;
                }
                default: break;
            }
        }
        resetClocks(); // the previous counters don't make any sense!
    }

    auto clkDivIn = ClkInDiv(normValue(params_.clkindiv));
    if (clkDivIn!= clockInDiv_) {
        clockInDiv_ = clkDivIn;
        for(auto& clk : clocks_) {
            calcMultiplier(clk, clockInDiv_);
        }
        resetClocks(); // everything has changed ;)
    }

    if (src_internal) {
        if (forceUpdate || normValue(params_.bpm) != bpm_) {
            //TODO recalc sample
            float samples = (sampleRate_ * 60.0f) / bpm_; // TODO use clock div.... this is quarters notes
            setClockSampleTargets(samples);
            // ? resetClocks();
        }

    }
    if(src_midi) {
        auto ppqn = MidiPPQN(normValue(params_.midippqn));
        if(forceUpdate || ppqn!=ppqn_) {
            // TODO : what need updating for midi when ppqn changes!


            resetClocks(); // unlikely anything makes sense any more
        }
    }

//    if(src_internal)  {
//        // I dont think we really need to do anything here...
//    }

    unsigned clkN = 0;
    for(auto& clk : clocks_) {
        auto clkDiv = ClkOutDiv(normValue(params_.divisions_[clkN]->val));
        float mult = calcMultiplier(clkDivIn,  clkDiv);
        if(mult != clk.multiplier()) clk.multiplier(mult);
        clkN++;
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
        sampleCount_++;

        if(src_cv && trig[I_CLK]) {
            sampleCount_ = 0;
            lastSampleCount_ = sampleCount_;
            setClockSampleTargets(lastSampleCount_);
        }

        if(src_midi && trig[I_MIDICLK]) {
            sampleCount_ = 0;
            lastSampleCount_ = sampleCount_;
            setClockSampleTargets(lastSampleCount_);
        }

        unsigned clkN = 0;
        for (Clock &clk: clocks_) {
            bool trigFired = false, smpFired = false;
            if (trig[I_RESET]) {
                clk.reset();
                buffer.setSample(O_CLK_1 + clkN, s, 0);
            } else {
                if (src_cv && trig[I_CLK]) trigFired = clk.trigTick();
                if (src_midi && trig[I_MIDICLK]) trigFired = clk.trigTick();
                smpFired = clk.sampleTick();
                bool fired = clk.useTrigs() ? trigFired : smpFired;
                buffer.setSample(O_CLK_1 + clkN, s, fired);
            }
            clkN++;
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

