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
    for (int i = 0; i < O_MAX; i++) {
        lastCV_[i] = nextCV_[i] = 0.0f;
    }
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    cv_a(*apvt.getParameter(ID::cv_a)),
    cv_b(*apvt.getParameter(ID::cv_b)),
    cv_c(*apvt.getParameter(ID::cv_c)),
    cv_d(*apvt.getParameter(ID::cv_d)),
    cv_e(*apvt.getParameter(ID::cv_e)),
    cv_f(*apvt.getParameter(ID::cv_f)),
    cv_g(*apvt.getParameter(ID::cv_g)),
    cv_h(*apvt.getParameter(ID::cv_h)),
    slew(*apvt.getParameter(ID::slew)),
    pb_range(*apvt.getParameter(ID::pb_range)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_a, "CC A", 1.0f, 120.0f, 1.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_b, "CC B", 1.0f, 120.0f, 2.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_c, "CC C", 1.0f, 120.0f, 3.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_d, "CC D", 1.0f, 120.0f, 4.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_e, "CC E", 1.0f, 120.0f, 5.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_f, "CC F", 1.0f, 120.0f, 6.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_g, "CC G", 1.0f, 120.0f, 7.0f, 1.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::cv_h, "CC H", 1.0f, 120.0f, 8.0f, 1.0f));

    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::slew, "Slew CC", false));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pb_range, "PB Range", 0.0f, 48.0f, 2.0f, 1.0f));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    switch (channelIndex) {
        case O_CV_A:
            return "CV A";
        case O_CV_B:
            return "CV B";
        case O_CV_C:
            return "CV C";
        case O_CV_D:
            return "CV D";
        case O_CV_E:
            return "CV E";
        case O_CV_F:
            return "CV F";
        case O_CV_G:
            return "CV G";
        case O_CV_H:
            return "CV H";
        case O_VOCT:
            return "VOct";
        case O_GATE:
            return "Gate";
        case O_VEL:
            return "Vel";
        default:;
    }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    static constexpr unsigned max_cc = O_CV_H - O_CV_A;
    for (int i = 0; i < O_MAX; i++) {
        if (!isOutputEnabled(O_CV_A + i)) continue;

        auto &lv = lastCV_[i];
        auto &nv = nextCV_[i];
        // optionally, slew cc when they change
        bool slew = i < max_cc && lv != nv && params_.slew.getValue() > 0.5f;
        if (slew) {
            float sv = (nv - lv) / sz;
            for (int smp = 0; smp < sz; smp++) buffer.setSample(O_CV_A + i, smp, lv + (sv * smp));
        } else {
            for (int smp = 0; smp < sz; smp++) buffer.setSample(O_CV_A + i, smp, nv);
        }
        lv = nv;
    }
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())

void PluginProcessor::handleIncomingMidiMessage(MidiInput *source, const MidiMessage &msg) {
    BaseProcessor::handleIncomingMidiMessage(source, msg);
    if (midiChannel_ == 0 || msg.getChannel() == midiChannel_) {
        if (msg.isNoteOn()) {
            lastNote_ = msg.getNoteNumber();
            float voct = pitch2Cv(lastNote_ - 60.f) + pitchbend_;
            float vel = msg.getFloatVelocity();
            nextCV_[O_VOCT - O_CV_A] = voct;
            nextCV_[O_GATE - O_CV_A] = 1.0f;
            nextCV_[O_VEL - O_CV_A] = vel;
        } else if (msg.isNoteOff()) {
            if (msg.getNoteNumber() == lastNote_) {
                // only handle note off whens its the current note for legato play
                nextCV_[O_GATE - O_CV_A] = 0.0f;
                nextCV_[O_VEL - O_CV_A] = 0.0f;
            }
        } else if (msg.isAllNotesOff()) {
            // all notes off
            nextCV_[O_GATE - O_CV_A] = 0.0f;
            nextCV_[O_VEL - O_CV_A] = 0.0f;
            pitchbend_ = 0.0f;
            lastNote_ = 0;
        } else if (msg.isPitchWheel()) {
            if (nextCV_[O_GATE - O_CV_A] > 0.5f) {
                float pbv = float(msg.getPitchWheelValue() - 8192);
                float pb = GET_P_VAL(params_.pb_range) * (pbv / (8192.0f - (pbv > 0.0f)));
                pitchbend_ = pitch2Cv(pb);
                float voct = pitch2Cv(lastNote_ - 60.f) + pitchbend_;
                nextCV_[O_VOCT - O_CV_A] = voct;
            }
        } else if (msg.isController()) {
            auto nc = msg.getControllerNumber();
            if (nc == GET_P_VAL(params_.cv_a)) {
                nextCV_[O_CV_A - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_b)) {
                nextCV_[O_CV_B - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_c)) {
                nextCV_[O_CV_C - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_d)) {
                nextCV_[O_CV_D - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_e)) {
                nextCV_[O_CV_E - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_f)) {
                nextCV_[O_CV_F - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_g)) {
                nextCV_[O_CV_G - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            } else if (nc == GET_P_VAL(params_.cv_h)) {
                nextCV_[O_CV_H - O_CV_A] = float(msg.getControllerValue() / 127.0f);
            }
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


Percussa::SSP::PluginDescriptor* PluginProcessor::createDescriptor() {
    auto desc = new Percussa::SSP::PluginDescriptor;

    desc->name = JucePlugin_Name;
    desc->descriptiveName = JucePlugin_Desc;
    desc->manufacturerName = JucePlugin_Manufacturer;
    desc->version = JucePlugin_VersionString;
    desc->uid = (int)JucePlugin_VSTUniqueID;
    for(int i=0;i<I_MAX;i++) {
        desc->inputChannelNames.push_back(getInputBusName(i).toStdString());
    }
    for(int i=0;i<O_MAX;i++) {
        desc->outputChannelNames.push_back(getOutputBusName(i).toStdString());
    }
    return desc;
}