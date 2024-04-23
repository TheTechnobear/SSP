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
    for (int i = 0; i < I_MAX; i++) {
        lastMidi_[i] = 0;
    }
    pitchbend_ = 8192;
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

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pb_range, "PB Range", 0.0f, 48.0f, 2.0f, 1.0f));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    switch (channelIndex) {
        case I_CV_A:
            return "CV A";
        case I_CV_B:
            return "CV B";
        case I_CV_C:
            return "CV C";
        case I_CV_D:
            return "CV D";
        case I_CV_E:
            return "CV E";
        case I_CV_F:
            return "CV F";
        case I_CV_G:
            return "CV G";
        case I_CV_H:
            return "CV H";
        case I_VOCT:
            return "VOct";
        case I_GATE:
            return "Gate";
        case I_VEL:
            return "Vel";
        default:;
    }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    return "ZZOut-" + String(channelIndex);
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue());

int PluginProcessor::getCCNum(int idx) {
    switch (idx) {
        case I_CV_A:
            return GET_P_VAL(params_.cv_a);
        case I_CV_B:
            return GET_P_VAL(params_.cv_b);
        case I_CV_C:
            return GET_P_VAL(params_.cv_c);
        case I_CV_D:
            return GET_P_VAL(params_.cv_d);
        case I_CV_E:
            return GET_P_VAL(params_.cv_e);
        case I_CV_F:
            return GET_P_VAL(params_.cv_f);
        case I_CV_G:
            return GET_P_VAL(params_.cv_g);
        case I_CV_H:
            return GET_P_VAL(params_.cv_h);
        default:;
    }
    return 0;
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if (midiOutDevice_ == nullptr || !(midiOutDevice_->isBackgroundThreadRunning())) {
        return;
    }

    unsigned sz = buffer.getNumSamples();

    static constexpr unsigned max_cc = I_CV_H - I_CV_A;

    MidiBuffer midimsgs;

    for (int smp = 0; smp < sz; smp++) {
        for (int i = 0; i < max_cc; i++) {
            int iidx = I_CV_A + i;
            if (!isInputEnabled(iidx)) continue;
            auto &lv = lastMidi_[i];

            float smpv = buffer.getSample(i, smp);
            int mv = std::max(std::min(int(smpv * 127), 127), 0);
            if (mv != lv) {
                int ccNum = getCCNum(iidx);
                if (ccNum > 0) {
                    int mch = midiChannel() ? midiChannel() : 1;
                    midimsgs.addEvent(MidiMessage::controllerEvent(mch, ccNum, mv), smp);
                }
                lv = mv;
            }
        }
    }

    // note handling
    if (isInputEnabled(I_VOCT) && isInputEnabled((I_GATE))) {
        for (int smp = 0; smp < sz; smp++) {
            auto &note = lastMidi_[I_VOCT - I_CV_A];
            auto &gate = lastMidi_[I_GATE - I_CV_A];
            auto &vel = lastMidi_[I_VEL - I_CV_A];

            float note_frac = cv2Pitch(buffer.getSample(I_VOCT, smp)) + 60.0f;
            int nnote = int(note_frac);
            int ngate = buffer.getSample(I_GATE, smp) > 0.5f;
            int nvel = isInputEnabled(I_VEL) ? int(buffer.getSample(I_VEL, smp) * 127) : 100;

            if (ngate > 0) {
                // gate is high, lets check voct is not changing, and so pitchbend
                float pbr = GET_P_VAL(params_.pb_range);
                int pb=8192;
                if(pbr>0) {
                    float frac = note_frac - ( (gate != ngate) ? nnote : note) ; // bend from note
                    frac = std::max(std::min(frac, pbr), -pbr);
                    pb = MidiMessage::pitchbendToPitchwheelPos(frac, pbr);
                }
                if (pb != pitchbend_) {
                    int mch = midiChannel() ? midiChannel() : 1;
                    midimsgs.addEvent(MidiMessage::pitchWheel(mch, pb), smp);
                    pitchbend_ = pb;
                }
            }

            if (gate != ngate) {
                // gate change = note on or off!
                int mch = midiChannel() ? midiChannel() : 1;
                if (ngate) {
                    midimsgs.addEvent(MidiMessage::noteOn(mch, nnote, (uint8) nvel), smp);
                    note = nnote;
                } else {
                    midimsgs.addEvent(MidiMessage::noteOff(mch, note), smp);
                }
            }
//            note = nnote; // note only changes when we send new note out
            vel = nvel;
            gate = ngate;

        }
    }

    if (!midimsgs.isEmpty()) {
        midiOutDevice_->sendBlockOfMessagesNow(midimsgs);
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
