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


String getDrumPid(StringRef drm, StringRef param) {
    return drm + String(ID::separator) + param;
}

PluginProcessor::DrumBaseParam::DrumBaseParam(AudioProcessorValueTreeState &apvt, StringRef drm) :
    Sustain(*apvt.getParameter(getDrumPid(drm, ID::Sustain))),
    Accent(*apvt.getParameter(getDrumPid(drm, ID::Accent))),
    Freq(*apvt.getParameter(getDrumPid(drm, ID::Freq))),
    Tone(*apvt.getParameter(getDrumPid(drm, ID::Tone))),
    Decay(*apvt.getParameter(getDrumPid(drm, ID::Decay))),
    Gain(*apvt.getParameter(getDrumPid(drm, ID::Gain))) {
}


PluginProcessor::ABParam::ABParam(AudioProcessorValueTreeState &apvt, StringRef drm) :
    DrumBaseParam(apvt, drm),
    AB_AttackFM(*apvt.getParameter(getDrumPid(drm, ID::AB_AttackFM))),
    AB_SelfFM(*apvt.getParameter(getDrumPid(drm, ID::AB_SelfFM))) {
}

PluginProcessor::SBParam::SBParam(AudioProcessorValueTreeState &apvt, StringRef drm) :
    DrumBaseParam(apvt, drm),
    SB_Dirt(*apvt.getParameter(getDrumPid(drm, ID::SB_Dirt))),
    SB_FMDecay(*apvt.getParameter(getDrumPid(drm, ID::SB_FMDecay))),
    SB_EnvFM(*apvt.getParameter(getDrumPid(drm, ID::SB_EnvFM))) {
}


PluginProcessor::ASParam::ASParam(AudioProcessorValueTreeState &apvt, StringRef drm) :
    DrumBaseParam(apvt, drm),
    AS_Snappy(*apvt.getParameter(getDrumPid(drm, ID::AS_Snappy))) {
}

PluginProcessor::SSParam::SSParam(AudioProcessorValueTreeState &apvt, StringRef drm) :
    DrumBaseParam(apvt, drm),
    SS_FM(*apvt.getParameter(getDrumPid(drm, ID::SS_FM))),
    SS_Snappy(*apvt.getParameter(getDrumPid(drm, ID::SS_Snappy))) {
}

PluginProcessor::HH1Param::HH1Param(AudioProcessorValueTreeState &apvt, StringRef drm) :
    DrumBaseParam(apvt, drm),
    HH1_Noise(*apvt.getParameter(getDrumPid(drm, ID::HH1_Noise))) {
}

PluginProcessor::HH2Param::HH2Param(AudioProcessorValueTreeState &apvt, StringRef drm) :
    DrumBaseParam(apvt, drm),
    HH2_Noise(*apvt.getParameter(getDrumPid(drm, ID::HH2_Noise))) {
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    ab_(apvt, ID::AB),
    sb_(apvt, ID::SB),
    as_(apvt, ID::AS),
    ss_(apvt, ID::SS),
    hh1_(apvt, ID::HH1),
    hh2_(apvt, ID::HH2) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    {
        auto drum = std::make_unique<AudioProcessorParameterGroup>(ID::AB, String(ID::AB), ID::separator);
        drum->addChild(std::make_unique<ssp::BaseBoolParameter>(getDrumPid(ID::AB, ID::Sustain), "AB Sustain", false));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::Accent), "AB Accent", 0, 1.0f, 0.1f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::Freq), "AB Freq", 1, 5000.0f, 50));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::Tone), "AB Tone", 0, 1.0f, 0.1f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::Decay), "AB Decay", 0, 1.0f, 0.3f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::Gain), "AB Gain", 0, 10.0f, 3.0f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::AB_SelfFM), "AB Self FM", 0, 1.0f, 1.0f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AB, ID::AB_AttackFM), "AB Attack FM", 0, 1.0f, 0.5f));
        params.add(std::move(drum));
    }
    {
        auto drum = std::make_unique<AudioProcessorParameterGroup>(ID::SB, String(ID::SB), ID::separator);
        drum->addChild(std::make_unique<ssp::BaseBoolParameter>(getDrumPid(ID::SB, ID::Sustain), "SB Sustain", false));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::Accent), "SB Accent", 0, 1.0f, 0.2f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::Freq), "SB Freq", 1, 5000.0f, 100));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::Tone), "SB Tone", 0, 1.0f, 0.6f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::Decay), "SB Decay", 0, 1.0f, 0.7f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::Gain), "SB Gain", 0, 10.0f, 1.5f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::SB_FMDecay), "SB FM Decay", 0, 1.0f, 0.3f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::SB_EnvFM), "SB FM Env", 0, 1.0f, 0.6f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SB, ID::SB_Dirt), "SB Dirt", 0, 1.0f, 0.7f));
        params.add(std::move(drum));
    }
    {
        auto drum = std::make_unique<AudioProcessorParameterGroup>(ID::AS, String(ID::AS), ID::separator);
        drum->addChild(std::make_unique<ssp::BaseBoolParameter>(getDrumPid(ID::AS, ID::Sustain), "AS Sustain", false));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AS, ID::Accent), "AS Accent", 0, 1.0f, 0.6f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AS, ID::Freq), "AS Freq", 1, 5000.0f, 200));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AS, ID::Tone), "AS Tone", 0, 1.0f, 0.5f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AS, ID::Decay), "AS Decay", 0, 1.0f, 0.3f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AS, ID::Gain), "AS Gain", 0, 10.0f, 1.0f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::AS, ID::AS_Snappy), "AS Snappy", 0, 1.0f, 0.7f));
        params.add(std::move(drum));
    }
    {
        auto drum = std::make_unique<AudioProcessorParameterGroup>(ID::SS, String(ID::SS), ID::separator);
        drum->addChild(std::make_unique<ssp::BaseBoolParameter>(getDrumPid(ID::SS, ID::Sustain), "SS Sustain", false));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::Accent), "SS Accent", 0, 1.0f, 0.6f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::Freq), "SS Freq", 1, 5000.0f, 200));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::Tone), "SS Tone", 0, 1.0f, 0.5f)); // not on SS
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::Decay), "SS Decay", 0, 1.0f, 0.3f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::Gain), "SS Gain", 0, 10.0f, 1.0f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::SS_Snappy), "SS Snappy", 0, 1.0f, 0.7f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::SS, ID::SS_FM), "SS FM", 0, 1.0f, 0.1f));
        params.add(std::move(drum));
    }

    {
        auto drum = std::make_unique<AudioProcessorParameterGroup>(ID::HH1, String(ID::HH1), ID::separator);
        drum->addChild(std::make_unique<ssp::BaseBoolParameter>(getDrumPid(ID::HH1, ID::Sustain), "HH1 Sustain", false));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH1, ID::Accent), "HH1 Accent", 0, 1.0f, 0.8f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH1, ID::Freq), "HH1 Freq", 1, 5000.0f, 3000));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH1, ID::Tone), "HH1 Tone", 0, 1.0f, 0.5f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH1, ID::Decay), "HH1 Decay", 0, 1.0f, 0.4f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH1, ID::Gain), "HH1 Gain", 0, 10.0f, 2.0f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH1, ID::HH1_Noise), "HH1 Noise", 0, 1.0f, 0.8f));
        params.add(std::move(drum));
    }

    {
        auto drum = std::make_unique<AudioProcessorParameterGroup>(ID::HH2, String(ID::HH2), ID::separator);
        drum->addChild(std::make_unique<ssp::BaseBoolParameter>(getDrumPid(ID::HH2, ID::Sustain), "HH2 Sustain", false));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH2, ID::Accent), "HH2 Accent", 0, 1.0f, 0.8f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH2, ID::Freq), "HH2 Freq", 1, 5000.0f, 3000));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH2, ID::Tone), "HH2 Tone", 0, 1.0f, 0.5f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH2, ID::Decay), "HH2 Decay", 0, 1.0f, 0.4f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH2, ID::Gain), "HH2 Gain", 0, 10.0f, 2.0f));
        drum->addChild(std::make_unique<ssp::BaseFloatParameter>(getDrumPid(ID::HH2, ID::HH2_Noise), "HH2 Noise", 0, 1.0f, 0.8f));
        params.add(std::move(drum));
    }

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "AB Trig",
        "AB Accent",
        "SB Trig",
        "SB Accent",
        "AS Trig",
        "AS Accent",
        "SS Trig",
        "SS Accent",
        "HH1 Trig",
        "HH1 Accent",
        "HH2 Trig",
        "HH2 Accent"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "A BD",
        "S BD",
        "A Snare",
        "S Snare",
        "Hi Hat 1",
        "Hi Hat 2"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    analogBassDrum_.Init(newSampleRate);
    syntheticBassDrum_.Init(newSampleRate);
    analogSnareDrum_.Init(newSampleRate);
    syntheticSnareDrum_.Init(newSampleRate);
    hiHat1_.Init(newSampleRate);
    hiHat2_.Init(newSampleRate);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();
    static constexpr float trigLevel = 0.2f;
    for (unsigned s = 0; s < sz; s++) {
        if (isInputEnabled(I_AB_TRIG && isOutputEnabled(O_AB))) {
            bool trigCv = buffer.getSample(I_AB_TRIG, s) > trigLevel;
            bool trig = trigCv && !trig_[O_AB];
            float gain = normValue(params_.ab_.Gain);
            gain = gain * gain;
            if (trig) {
                float freq = normValue(params_.ab_.Freq);
                float accent = constrain(params_.ab_.Accent.getValue() +
                                         buffer.getSample(I_AB_ACCENT, s),
                                         0.0f, 1.0f);
                bool sustain = params_.ab_.Sustain.getValue() > trigLevel;
                float decay = params_.ab_.Decay.getValue();
                float tone = params_.ab_.Tone.getValue();
                float attackfm = params_.ab_.AB_AttackFM.getValue();
                float selffm = params_.ab_.AB_SelfFM.getValue();

                analogBassDrum_.SetFreq(freq);
                analogBassDrum_.SetAccent(accent);
                analogBassDrum_.SetSustain(sustain);
                analogBassDrum_.SetDecay(decay);
                analogBassDrum_.SetTone(tone);
                analogBassDrum_.SetAttackFmAmount(attackfm);
                analogBassDrum_.SetSelfFmAmount(selffm);
            }
            buffer.setSample(O_AB, s, analogBassDrum_.Process(trig) * gain);
            trig_[O_AB] = trigCv;
        } else {
            buffer.setSample(O_AB, s, 0.0f);
        }

        if (isInputEnabled(I_SB_TRIG && isOutputEnabled(O_SB))) {
            bool trigCv = buffer.getSample(I_SB_TRIG, s) > trigLevel;
            bool trig = trigCv && !trig_[O_SB];
            float gain = normValue(params_.sb_.Gain);
            gain = gain * gain;
            if (trig) {
                float freq = normValue(params_.sb_.Freq);
                float accent = constrain(params_.sb_.Accent.getValue() +
                                         buffer.getSample(I_SB_ACCENT, s),
                                         0.0f, 1.0f);
                bool sustain = params_.sb_.Sustain.getValue() > trigLevel;
                float decay = params_.sb_.Decay.getValue();
//                float tone = params_.sb_.Tone.getValue();
                float envfm = params_.sb_.SB_EnvFM.getValue();
                float dirt = params_.sb_.SB_Dirt.getValue();
                float fmdecay = params_.sb_.SB_FMDecay.getValue();

                syntheticBassDrum_.SetFreq(freq);
                syntheticBassDrum_.SetAccent(accent);
                syntheticBassDrum_.SetSustain(sustain);
                syntheticBassDrum_.SetDecay(decay);
//                syntheticBassDrum_.SetTone(tone);
                syntheticBassDrum_.SetDirtiness(dirt);
                syntheticBassDrum_.SetFmEnvelopeAmount(envfm);
                syntheticBassDrum_.SetFmEnvelopeDecay(fmdecay);

            }
            buffer.setSample(O_SB, s, syntheticBassDrum_.Process(trig) * gain);
            trig_[O_SB] = trigCv;
        } else {
            buffer.setSample(O_SB, s, 0.0f);
        }

        if (isInputEnabled(I_AS_TRIG && isOutputEnabled(O_AS))) {
            bool trigCv = buffer.getSample(I_AS_TRIG, s) > trigLevel;
            bool trig = trigCv && !trig_[O_AS];
            float gain = normValue(params_.as_.Gain);
            gain = gain * gain;
            if (trig) {
                float freq = normValue(params_.as_.Freq);
                float accent = constrain(params_.as_.Accent.getValue() +
                                         buffer.getSample(I_AS_ACCENT, s),
                                         0.0f, 1.0f);
                bool sustain = params_.as_.Sustain.getValue() > trigLevel;
                float decay = params_.as_.Decay.getValue();
                float tone = params_.as_.Tone.getValue();
                float snappy = params_.as_.AS_Snappy.getValue();

                analogSnareDrum_.SetFreq(freq);
                analogSnareDrum_.SetAccent(accent);
                analogSnareDrum_.SetSustain(sustain);
                analogSnareDrum_.SetDecay(decay);
                analogSnareDrum_.SetTone(tone);
                analogSnareDrum_.SetSnappy(snappy);
            }
            buffer.setSample(O_AS, s, analogSnareDrum_.Process(trig) * gain);
            trig_[O_AS] = trigCv;
        } else {
            buffer.setSample(O_AS, s, 0.0f);
        }

        if (isInputEnabled(I_SS_TRIG && isOutputEnabled(O_SS))) {
            bool trigCv = buffer.getSample(I_SS_TRIG, s) > trigLevel;
            bool trig = trigCv && !trig_[O_SS];
            float gain = normValue(params_.ss_.Gain);
            gain = gain * gain;
            if (trig) {
                float freq = normValue(params_.ss_.Freq);
                float accent = constrain(params_.ss_.Accent.getValue() +
                                         buffer.getSample(I_SS_ACCENT, s),
                                         0.0f, 1.0f);
                bool sustain = params_.ss_.Sustain.getValue() > trigLevel;
                float decay = params_.ss_.Decay.getValue();
//                float tone = params_.ss_.Tone.getValue();
                float fm = params_.ss_.SS_FM.getValue();
                float snappy = params_.ss_.SS_Snappy.getValue();

                syntheticSnareDrum_.SetFreq(freq);
                syntheticSnareDrum_.SetAccent(accent);
                syntheticSnareDrum_.SetSustain(sustain);
                syntheticSnareDrum_.SetDecay(decay);
//                syntheticSnareDrum_.SetTone(tone);
                syntheticSnareDrum_.SetSnappy(snappy);
                syntheticSnareDrum_.SetFmAmount(fm);
            }
            buffer.setSample(O_SS, s, syntheticSnareDrum_.Process(trig) * gain);
            trig_[O_SS] = trigCv;
        } else {
            buffer.setSample(O_SS, s, 0.0f);
        }

        if (isInputEnabled(I_HH1_TRIG && isOutputEnabled(O_HH1))) {
            bool trigCv = buffer.getSample(I_HH1_TRIG, s) > trigLevel;
            bool trig = trigCv && !trig_[O_HH1];
            float gain = normValue(params_.hh1_.Gain);
            gain = gain * gain;
            if (trig) {
                float freq = normValue(params_.hh1_.Freq);
                float accent = constrain(params_.hh1_.Accent.getValue() +
                                         buffer.getSample(I_HH1_ACCENT, s),
                                         0.0f, 1.0f);
                bool sustain = params_.hh1_.Sustain.getValue() > trigLevel;
                float decay = params_.hh1_.Decay.getValue();
                float tone = params_.hh1_.Tone.getValue();
                float noise = params_.hh1_.HH1_Noise.getValue();

                hiHat1_.SetFreq(freq);
                hiHat1_.SetAccent(accent);
                hiHat1_.SetSustain(sustain);
                hiHat1_.SetDecay(decay);
                hiHat1_.SetTone(tone);
                hiHat1_.SetNoisiness(noise);
            }
            buffer.setSample(O_HH1, s, hiHat1_.Process(trig) * gain);
            trig_[O_HH1] = trigCv;
        } else {
            buffer.setSample(O_HH1, s, 0.0f);
        }

        if (isInputEnabled(I_HH2_TRIG && isOutputEnabled(O_HH2))) {
            bool trigCv = buffer.getSample(I_HH2_TRIG, s) > trigLevel;
            bool trig = trigCv && !trig_[O_HH2];
            float gain = normValue(params_.hh2_.Gain);
            gain = gain * gain;
            if (trig) {
                float freq = normValue(params_.hh2_.Freq);
                float accent = constrain(params_.hh2_.Accent.getValue() +
                                         buffer.getSample(I_HH2_ACCENT, s),
                                         0.0f, 1.0f);
                bool sustain = params_.hh2_.Sustain.getValue() > trigLevel;
                float decay = params_.hh2_.Decay.getValue();
                float tone = params_.hh2_.Tone.getValue();
                float noise = params_.hh2_.HH2_Noise.getValue();

                hiHat2_.SetFreq(freq);
                hiHat2_.SetAccent(accent);
                hiHat2_.SetSustain(sustain);
                hiHat2_.SetDecay(decay);
                hiHat2_.SetTone(tone);
                hiHat2_.SetNoisiness(noise);
            }
            buffer.setSample(O_HH2, s, hiHat2_.Process(trig) * gain);
            trig_[O_HH2] = trigCv;
        } else {
            buffer.setSample(O_HH2, s, 0.0f);
        }

    } // for sample
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
