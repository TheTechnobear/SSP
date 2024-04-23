#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMiniEditor.h"
#include "ssp/EditorHost.h"


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
    for (int i = 0; i < (O_MAX / 2); i++) {
        nextTR_[i] = false;
        nextVel_[i] = 0.0f;
    }

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    tr_a(*apvt.getParameter(ID::tr_a)),
    tr_b(*apvt.getParameter(ID::tr_b)),
    tr_c(*apvt.getParameter(ID::tr_c)),
    tr_d(*apvt.getParameter(ID::tr_d)),
    tr_e(*apvt.getParameter(ID::tr_e)),
    tr_f(*apvt.getParameter(ID::tr_f)),
    tr_g(*apvt.getParameter(ID::tr_g)),
    tr_h(*apvt.getParameter(ID::tr_h)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_a, "NOTE A", 0.0f, 127.0f, 60.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_b, "NOTE B", 0.0f, 127.0f, 61.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_c, "NOTE C", 0.0f, 127.0f, 62.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_d, "NOTE D", 0.0f, 127.0f, 63.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_e, "NOTE E", 0.0f, 127.0f, 64.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_f, "NOTE F", 0.0f, 127.0f, 65.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_g, "NOTE G", 0.0f, 127.0f, 66.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::tr_h, "NOTE H", 0.0f, 127.0f, 67.0f, 1.0f));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    switch (channelIndex) {
        case O_TR_A:
            return "Trig A";
        case O_VEL_A:
            return "Vel A";
        case O_TR_B:
            return "Trig B";
        case O_VEL_B:
            return "Vel B";
        case O_TR_C:
            return "Trig C";
        case O_VEL_C:
            return "Vel C";
        case O_TR_D:
            return "Trig D";
        case O_VEL_D:
            return "Vel D";
        case O_TR_E:
            return "Trig E";
        case O_VEL_E:
            return "Vel E";
        case O_TR_F:
            return "Trig F";
        case O_VEL_F:
            return "Vel F";
        case O_TR_G:
            return "Trig G";
        case O_VEL_G:
            return "Vel G";
        case O_TR_H:
            return "Trig H";
        case O_VEL_H:
            return "Vel H";
        default:;
    }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    static constexpr unsigned max_cc = O_TR_H - O_TR_A;
    for (int i = 0; i < (O_MAX / 2); i++) {
        int bidx = O_TR_A + (i * 2);
        if (!isOutputEnabled(bidx)) continue;
        int smp = 0;
        bool tr = nextTR_[i];
        float v = nextVel_[i]; 
        if (tr) {
            for (; smp < 64; smp++) {
                buffer.setSample(bidx, smp, tr);
                buffer.setSample(bidx + 1, smp, v);
            }
            nextTR_[i] = false;
        }
        for (; smp < sz; smp++) {
            buffer.setSample(bidx, smp, 0.0f);
            buffer.setSample(bidx + 1, smp, v);
        }
    }
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())

void PluginProcessor::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &msg) {
    BaseProcessor::handleIncomingMidiMessage(source, msg);
    if (midiChannel_ == 0 || msg.getChannel() == midiChannel_) {
        if (msg.isNoteOn()) { // only care about note on
            auto note = msg.getNoteNumber();
            if (note == GET_P_VAL(params_.tr_a)) {
                nextTR_[0] = true;
                nextVel_[0] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_b)) {
                nextTR_[1] = true;
                nextVel_[1] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_c)) {
                nextTR_[2] = true;
                nextVel_[2] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_d)) {
                nextTR_[3] = true;
                nextVel_[3] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_e)) {
                nextTR_[4] = true;
                nextVel_[4] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_f)) {
                nextTR_[5] = true;
                nextVel_[5] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_g)) {
                nextTR_[6] = true;
                nextVel_[6] = msg.getFloatVelocity();
            } else if (note == GET_P_VAL(params_.tr_h)) {
                nextTR_[7] = true;
                nextVel_[7] = msg.getFloatVelocity();
            }
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
#ifdef FORCE_COMPACT_UI
    return new ssp::EditorHost(this, new PluginMiniEditor(*this),true);
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

