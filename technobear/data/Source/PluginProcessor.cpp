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
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()), messageQueue_(MAX_MSGS) {
    init();
    // lastInIdx_ = 0;
    // lastOutIdx_ = 0;
    // inCount_ = 0;
    // outCount_ = 0;
}

PluginProcessor::~PluginProcessor() {
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::SigParams::SigParams(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    y_offset(*apvt.getParameter(getPID(pre, sn, ID::y_offset))),
    y_scale(*apvt.getParameter(getPID(pre, sn, ID::y_scale))),
    show(*apvt.getParameter(getPID(pre, sn, ID::show))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    t_scale(*apvt.getParameter(ID::t_scale)),
    ab_xy(*apvt.getParameter(ID::ab_xy)),
    cd_xy(*apvt.getParameter(ID::cd_xy)) {
    for (unsigned i = 0; i < MAX_SIG_IN; i++) {
        sigparams_.push_back(std::make_unique<SigParams>(apvt, ID::sig, i));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);


    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::t_scale, "Time", 1.0f, 100000.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::ab_xy, "AB XY", false));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::cd_xy, "CD XY", false));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::sig, "Signal", ID::separator);
    for (unsigned sn = 0; sn < MAX_SIG_IN; sn++) {
//        String prefix = ID::sig + String(ID::separator) + String(sn) + juce::String(ID::separator);
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::sig, sn, ID::y_scale), "Scale", -5.0f, 5.0f, 1.0f));
        sg->addChild(std::make_unique<ssp::BaseFloatParameter>(getPID(ID::sig, sn, ID::y_offset), "Offset", -5.0f, 5.0f, 0.0f));
        sg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::sig, sn, ID::show), "Visible", true));
    }
    params.add(std::move(sg));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In A",
        "In B",
        "In C",
        "In D",
        "Trig"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out A",
        "Out B",
        "Out C",
        "Out D"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::onInputChanged(unsigned i, bool b) {
    BaseProcessor::onInputChanged(i, b);
    // inCount_ = 0;
    // for (unsigned x = I_SIG_1; x < (I_SIG_1 + MAX_SIG_IN); x++) {
    //     if (inputEnabled[x]) inCount_++;
    // }
}

void PluginProcessor::onOutputChanged(unsigned i, bool b) {
    BaseProcessor::onOutputChanged(i, b);
    // outCount_ = 0;
    // for (unsigned x = O_SIG_A; x < (O_SIG_A + MAX_SIG_OUT); x++) {
    //     if (outputEnabled[x]) outCount_++;
    // }
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // inputBuffer_.setSize(1, samplesPerBlock);
    // lastBuffer_.setSize(1, samplesPerBlock);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    // test only :)
    unsigned n = buffer.getNumSamples();
    for (unsigned i = 0; i < n; i++) {
        DataMsg msg;
        msg.count_ = 4;
        msg.sample_[0] = buffer.getSample(I_SIG_A, i);
        msg.sample_[1] = buffer.getSample(I_SIG_B, i);
        msg.sample_[2] = buffer.getSample(I_SIG_C, i);
        msg.sample_[3] = buffer.getSample(I_SIG_D, i);
        if (!messageQueue_.try_enqueue(msg)) {
            // queue is full, perhaps editor is not consuming....\
            // or is not consuming fast enough
        }
    }


#if 0
    unsigned n = buffer.getNumSamples();
    unsigned n2 = n / 2;
    float cvInS = buffer.getSample(I_IN_SEL, 0);
    float cvOutS = buffer.getSample(I_OUT_SEL, 0);

    // determine indexes of io
    unsigned iIdx = 0, oIdx = 0;
    float inSel = (params_.inSel.getValue() * 2.0f) - 1.0f;
    float outSel = (params_.outSel.getValue() * 2.0f) - 1.0f;
    bool useActive = params_.active.getValue();
    unsigned inSIdx = unsigned(((constrain(inSel + cvInS, -1.0f, 0.999f) + 1.0f) * (useActive ? inCount_ : 8.0f)) / 2.0f);
    unsigned outSIdx = unsigned(((constrain(outSel + cvOutS, -1.0f, 0.999f) + 1.0f) * (useActive ? outCount_ : 8.0f)) / 2.0f);

    if (useActive) {
        for (unsigned x = 0; x < 8; x++) {
            if (inputEnabled[I_SIG_1 + x]) {
                if (inSIdx == iIdx) {
                    break;
                }
                iIdx++;
            }
        }
        for (unsigned y = 0; y < 8; y++) {
            if (outputEnabled[O_SIG_A + y] > 0.5f) {
                if (outSIdx == oIdx) {
                    break;
                }
                oIdx++;
            }
        }
    } else {
        iIdx = inSIdx;
        oIdx = outSIdx;
    }

    // what io are active
    bool iActive = inputEnabled[I_SIG_1 + iIdx];
    bool oActive = outputEnabled[O_SIG_A + oIdx];
    bool liActive = inputEnabled[I_SIG_1 + lastInIdx_];
    bool loActive = outputEnabled[O_SIG_A + lastOutIdx_];

    bool soft = params_.soft.getValue();
    bool ramps = soft && (lastInIdx_ != iIdx || lastOutIdx_ != oIdx);

    // copy input to output
    // if soft, then we need to ramp inputs and outputs when they change
    if (iActive) {
        inputBuffer_.copyFrom(0, 0, buffer, I_SIG_1 + iIdx, 0, n);
    } else {
        inputBuffer_.applyGain(0, 0, n, 0.0f);
    }


    if (soft) {
        if (liActive) {
            // only need in buf if its differ
            lastBuffer_.copyFrom(0, 0, buffer, I_SIG_1 + lastInIdx_, 0, n);
        } else {
            lastBuffer_.applyGain(0, 0, n, 0.0f);
        }
    }

    if (ramps) {
        lastBuffer_.applyGainRamp(0, 0, n2, 1.0f, 0.0f); // ramp down
        lastBuffer_.applyGain(0, n2, n2, 0.0f);

        inputBuffer_.applyGain(0, 0, n2, 0.0f);
        inputBuffer_.applyGainRamp(0, n2, n2, 0.f, 1.0f);// ramp up
    }

    buffer.clear();

    if (oActive) {
        buffer.copyFrom(O_SIG_A + oIdx, 0, inputBuffer_, 0, 0, n);
        if (iIdx != lastInIdx_) {
            buffer.addFrom(O_SIG_A + oIdx, 0, lastBuffer_, 0, 0, n2);
        }
    }

    if (oIdx != lastOutIdx_ && loActive) {
        buffer.copyFrom(O_SIG_A + lastOutIdx_, 0, lastBuffer_, 0, 0, n);
    }

    lastInIdx_ = iIdx;
    lastOutIdx_ = oIdx;
#endif
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


