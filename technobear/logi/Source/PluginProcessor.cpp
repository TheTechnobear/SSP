#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"


PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
}

String getPID(StringRef pre, unsigned sn, StringRef id) {
    String pid = pre + String(ID::separator) + String(sn) + String(ID::separator) + id;
    return pid;
}


PluginProcessor::GateParam::GateParam(AudioProcessorValueTreeState &apvt, StringRef pre, unsigned sn) :
    inv(*apvt.getParameter(getPID(pre, sn, ID::inv))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    oper(*apvt.getParameter(ID::oper)) {
    for (unsigned i = 0; i < I_MAX; i++) {
        gateparams_.push_back(std::make_unique<GateParam>(apvt, ID::gates, i));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    StringArray op_types;
    op_types.add("AND");
    op_types.add("OR");
    op_types.add("XOR");
    op_types.add("NAND");
    op_types.add("NOT A");
    op_types.add("NOT B");
    jassert(op_types.size() == OT_MAX);
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::oper, "op", op_types, 0));

    auto sg = std::make_unique<AudioProcessorParameterGroup>(ID::gates, "gates", ID::separator);
    for (unsigned sn = 0; sn < I_MAX; sn++) {
        char ar[2];
        ar[0] = 'A' + (sn % 2);
        ar[1] = '1' + (sn / 2);
        String desc = "Inv " + String(ar);
        sg->addChild(std::make_unique<ssp::BaseBoolParameter>(getPID(ID::gates, sn, ID::inv), desc, false));
    }
    params.add(std::move(sg));


    return params;
}

const String PluginProcessor::getInputBusName(int channelIndex) {
    switch (channelIndex) {
        case I_IN_A1:
            return "In A1";
        case I_IN_B1:
            return "In B1";
        case I_IN_A2:
            return "In A2";
        case I_IN_B2:
            return "In B2";
        case I_IN_A3:
            return "In A3";
        case I_IN_B3:
            return "In B3";
        case I_IN_A4:
            return "In A4";
        case I_IN_B4:
            return "In B4";
        default:;
    }

    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    switch (channelIndex) {
        case O_OUT_ALL:
            return "Out ALL";
        case O_OUT_1:
            return "Out 1";
        case O_OUT_2:
            return "Out 2";
        case O_OUT_3:
            return "Out 3";
        case O_OUT_4:
            return "Out 4";
        default:;
    }
    return "ZZOut-" + String(channelIndex);
}

//TODO : could add buttons , which invert input?

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    static constexpr unsigned N_PAIRS = I_MAX / 2;
    static constexpr float trigLevel = 0.5f;
    const unsigned sz = buffer.getNumSamples();
    const OperType op = OperType(normValue(params_.oper));
    const bool def = defValue_[op];

    for (unsigned i = 0; i < N_PAIRS; i++) {
        auto ain = i * 2, bin = ain + 1;
        auto abuf = buffer.getReadPointer(ain);
        auto bbuf = buffer.getReadPointer(bin);
        auto ainv = params_.gateparams_[ain]->inv.getValue() > trigLevel;
        auto binv = params_.gateparams_[bin]->inv.getValue() > trigLevel;
        unsigned gout = i + 1;
        for (int smp = 0; smp < sz; smp++) {
            bool res;
            bool a = inputEnabled[ain] ? abuf[smp] > trigLevel : def;
            bool b = inputEnabled[bin] ? bbuf[smp] > trigLevel : def;
            if (ainv) a = !a;
            if (binv) b = !b;
            switch (op) {
                case OT_AND :
                    res = a && b;
                    break;
                case OT_OR :
                    res = a || b;
                    break;
                case OT_XOR:
                    res = a != b;
                    break;
                case OT_NAND :
                    res = !(a && b);
                    break;
                case OT_NOT_A :
                    res = !a;
                    break;
                case OT_NOT_B :
                    res = !b;
                    break;
                default:
                    break;
            }
            buffer.setSample(gout, smp, res);
            if (smp == 0) {
                lastIn_[ain] = a;
                lastIn_[bin] = b;
                lastOut_[gout] = res;
            }
        }
    } // pair
    // build up the main output by combining the pairs


    auto buf1 = buffer.getReadPointer(O_OUT_1);
    auto buf2 = buffer.getReadPointer(O_OUT_2);
    auto buf3 = buffer.getReadPointer(O_OUT_3);
    auto buf4 = buffer.getReadPointer(O_OUT_4);
    for (int smp = 0; smp < sz; smp++) {
        bool res;
        bool a = buf1[smp] > trigLevel;
        bool b = buf2[smp] > trigLevel;
        bool c = buf3[smp] > trigLevel;
        bool d = buf4[smp] > trigLevel;
        switch (op) {
            case OT_AND :
                res = a && b && c && d;
                break;
            case OT_OR :
                res = a || b || c || d;
                break;
            case OT_XOR:
                res = (a != b) != (c != d);
                break;
            case OT_NAND :
                res = !(a && b && b && c);
                break;
            case OT_NOT_A :
            case OT_NOT_B :
                res = a && b && c && d;
                break;
            default:
                break;
        }
        buffer.setSample(O_OUT_ALL, smp, res);
        if (smp == 0) lastOut_[O_OUT_ALL] = res;
    }
}

void PluginProcessor::getValues(bool *inputs, bool *outputs) {
    for (unsigned i = 0; i < I_MAX; i++) {
        inputs[i] = lastIn_[i];
    }
    for (unsigned i = 0; i < O_MAX; i++) {
        outputs[i] = lastOut_[i];
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


