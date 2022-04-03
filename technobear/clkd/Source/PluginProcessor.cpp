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

void PluginProcessor::updateClockRates(bool force) {
    bool updatebpm = force;
    bool updatecvin = force;
    bool updateppqn = force;

    auto src = Source(normValue(PluginProcessor::params_.source));

    if (src != PluginProcessor::source_) {
        source_ = src;
        switch (source_) {
            case SRC_INTERNAL :
                updatebpm = true;
                break;
            case SRC_CLKIN :
                updatecvin = true;
                break;
            case SRC_MIDI :
                updateppqn = true;
                break;
            default:
                break;
        }
    }

    unsigned clk = sampleRate_;
    switch (source_) {
        case SRC_INTERNAL : {
            auto n = normValue(params_.bpm);
            if (updatebpm || bpm_ != n) {
                bpm_ = n;
                float clk = ((sampleRate_ * 60.0f) / bpm_) * 4.0f; // bar = x1
//                updateClkOut(clk);
            }
            break;
        }
        case SRC_CLKIN : {
            auto n = ClkInDiv(normValue(params_.clkindiv));
            if (updatecvin || clkin_ != n) {
                clkin_ = n;
            }
            break;
        }
        case SRC_MIDI : {
            auto n = MidiPPQN(normValue(params_.midippqn));
            if (updateppqn || ppqn_ != n) {
                ppqn_ = n;
            }
            break;
        }
        default:
//            clkInDiv_[CO_MAX];
//            unsigned clkOutDiv_[CO_MAX];
            break;
    }
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    sampleRate_ = newSampleRate;
    updateClockRates(true);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    static constexpr float trigLevel = 0.5f;
    unsigned sz = buffer.getNumSamples();

    auto src = Source(normValue(PluginProcessor::params_.source));
    auto ppqn = unsigned(normValue(params_.midippqn));

    bool src_cv = src == SRC_CLKIN;
    bool src_midi = src == SRC_MIDI;
    bool src_internal = src == SRC_INTERNAL;

    for(unsigned s=0;s<sz;s++) {
        float clkIn = buffer.getSample(I_CLK,s);
        float resetIn = buffer.getSample(I_RESET,s);
        float midiIn = buffer.getSample(I_MIDICLK,s);

        bool clkTrig = clkIn > trigLevel && lastClkIn_ < trigLevel;
        bool resetTrig = resetIn > trigLevel && lastResetIn_ < trigLevel;
        bool midiTrig = resetIn > trigLevel && lastMidiIn_ < trigLevel;

        sampleCount_ = (sampleCount_ + 1 ) % MAX_SAMPLE_COUNT;

        // TODO : need to think about this more !
        // need to be clear on definitions
        // X4 is 4 times speed of incoming clock ... i.e. give it a 1/4 input , you get a 1/16
        // D4 is a 1/4  incoming clock ... i.e. give it a 1/4 input , you get a 1/1 = a bar!
        // .. so 1/16 does not mean a 1/16 notes ;)

        // by definition multiples have to be done with samples, since we have to 'predict' clock
        // it therefore makes sense to use a sample count.

        // reset sample count.. at highest division... hmm, but ive no idea when that is ;)
        // will have to make a decision on this and handle 'wrap around'




        if(src_cv && clkTrig) {
            cvSampleCount_ = sampleCount_;
        }

        if(src_midi && midiTrig) {
            midiSampleCount_ = sampleCount_;

            subClockCount_++;
            if(midiPPQNRate_[ppqn]==subClockCount_) {
                subClockCount_ = 0;
                clockCount_++;
            }
        }

        if(src_internal) {
            subClockCount_++;
            if(subClockCount_ == sampleRate_) {
                clockCount_++;
            }
        }


        if(resetTrig) {
            clockCount_ = 0;
        }


        lastClkIn_ = clkIn;
        lastResetIn_= resetIn;
        lastMidiIn_ = midiIn;

    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

