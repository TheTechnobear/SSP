#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "PluginMiniEditor.h"
#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

#define MAX_GAIN 3.0f
#define MIN_GAIN 0.0f


#include <cmath>

// from : https://www.kvraudio.com/forum/viewtopic.php?t=148865

// -3dB at center, sin/cos taper
// LeftOut = cos(pan*pi/2) * MonoIn;
// RightOut = sin(pan*pi/2) * MonoIn;

// -3db at center, sqrt taper
// LeftOut = sqrt(pan) * MonoIn;
// RightOut = sqrt(1-pan) * MonoIn;

// -6db at center, linear taper
// LeftOut = pan * MonoIn;
// RightOut = (1-pan) * MonoIn;

// to do: optimise, use a sin lookup table
inline float panGain(bool left, float p) {
    static constexpr float PIdiv2 = M_PI / 2.0f;
    float pan = (p + 1.0f) / 2.0f;
    if (left) {
        return cosf(pan * PIdiv2);
        // return std::cosf(pan * PIdiv2);
    }
    return sinf(pan * PIdiv2);
    // return std::sinf(pan * PIdiv2);
}


PluginProcessor::PluginProcessor() : PluginProcessor(getBusesProperties(), createParameterLayout()) {
}

PluginProcessor::PluginProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                                 AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
}

PluginProcessor::~PluginProcessor() {
}

String getVcaPid(unsigned vid) {
    return String(ID::vca) + String(ID::separator) + String(vid);
}

String getVcaParamId(unsigned vid, StringRef id) {
    return getVcaPid(vid) + String(ID::separator) + id;
}

PluginProcessor::VcaParams::VcaParams(AudioProcessorValueTreeState &apvt, unsigned id)
    : id_(id),
      pid_(getVcaPid(id)),
      gain(*apvt.getParameter(getVcaParamId(id, ID::gain))),
      pan(*apvt.getParameter(getVcaParamId(id, ID::pan))) {
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) {
    for (unsigned vid = 0; vid < MAX_VCA; vid++) {
        auto vca = std::make_unique<VcaParams>(apvt, vid);
        vcaParams_.push_back(std::move(vca));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);


    auto vcas = std::make_unique<AudioProcessorParameterGroup>(ID::vca, String(ID::vca), ID::separator);
    for (unsigned vid = 0; vid < MAX_VCA; vid++) {
        String desc = "VCA" + String(vid + 1) + " ";
        auto vca = std::make_unique<AudioProcessorParameterGroup>(getVcaPid(vid), getVcaPid(vid), ID::separator);
        vca->addChild(
            std::make_unique<ssp::BaseFloatParameter>(getVcaParamId(vid, ID::gain), desc + "Gain", 0.0f, 3.0f, 1.0f));
        vca->addChild(
            std::make_unique<ssp::BaseFloatParameter>(getVcaParamId(vid, ID::pan), desc + "Pan", -1.0f, 1.0f, 0.0f));
        vcas->addChild(std::move(vca));
    }
    params.add(std::move(vcas));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[MAX_I_VCA] = { "In L ", "In R ", "Gain", "Pan" };


    if (channelIndex >= I_MAX) return "ZZIn-" + String(channelIndex);

    unsigned vca = channelIndex / MAX_I_VCA;
    unsigned param = channelIndex % MAX_I_VCA;
    return String("VCA") + String(vca + 1) + " " + inBusName[param];
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    if (channelIndex >= O_MAX) return "ZZOut-" + String(channelIndex);

    String chName = "Main";
    if (channelIndex >= 2) { chName = "VCA " + String(channelIndex / 2); }

    if (channelIndex % 2) {
        chName += " R";
    } else {
        chName += " L";
    }
    return chName;
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();
    for (int vid = 0; vid < MAX_VCA; vid++) {
        // if (!isOutputEnabled(O_OUT_A + vid)) continue;

        auto &vcaparams = params_.vcaParams_[vid];
        auto &vca = vcas_[vid];
        float pGain = normValue(vcaparams->gain);
        float pPan = normValue(vcaparams->pan);

        auto inLCh = (vid * MAX_I_VCA) + I_VCA_1_IN_L;
        auto inRBuf = (vid * MAX_I_VCA) + I_VCA_1_IN_R;
        auto panCh = (vid * MAX_I_VCA) + I_VCA_1_PAN;
        auto gainCh = (vid * MAX_I_VCA) + I_VCA_1_GAIN;


        // raamp the paramters to prevent audio jumps
        float gainInc = (pGain - vca.gain_) / sz;
        float panInc = (pPan - vca.pan_) / sz;

        for (int i = 0; i < sz; i++) {
            float inL = buffer.getSample(inLCh, i);
            float inR = buffer.getSample(inRBuf, i);
            float inPan = buffer.getSample(panCh, i);
            float inGain= buffer.getSample(gainCh, i);

            vcas_[vid].gain_ += gainInc;
            vcas_[vid].pan_  += panInc;

            float panL = panGain(true, inPan + vcas_[vid].pan_);
            float panR = panGain(false, inPan + vcas_[vid].pan_);

            float gain = constrain(inGain + vcas_[vid].gain_, MIN_GAIN, MAX_GAIN);
            
            float outL = inL * gain * panL;
            float outR = inR * gain * panR;

            // note:
            // for the first 4 channels we have to be careful to not overrite the input
            // e.g. ch 0-4, are vca 1 inL/R,pan,gain, but also main out L/R/vca 1 L/R
            // so its 4 in, and 4 out... and the sum in is 0,1... so we cannot just use vca in lr
            // after than its 4 in and 2 out, so no danger of overwriting 

            if(vid==0) {
                buffer.setSample(O_SUM_L, i, outL);
                buffer.setSample(O_SUM_R, i, outR);
            } else {
                buffer.addSample(O_SUM_L, i, outL); 
                buffer.addSample(O_SUM_R, i, outR);
            }
            buffer.setSample(O_VCA_1_L + vid, i, outL);
            buffer.setSample(O_VCA_1_R + vid, i, outR);
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
