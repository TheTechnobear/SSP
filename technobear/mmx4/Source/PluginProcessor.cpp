
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
    memset(lastVcaCV_, 0, sizeof(lastVcaCV_));
#if __APPLE__
    for(unsigned i=0;i<I_MAX;i++) inputEnabled[i]=false;
    for(unsigned i=0;i<O_MAX;i++) outputEnabled[i]=false;
#endif

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    vca{
        (*apvt.
            getParameter(ID::vca_1a)
        ),
        (*apvt.
            getParameter(ID::vca_1b)
        ),
        (*apvt.
            getParameter(ID::vca_1c)
        ),
        (*apvt.
            getParameter(ID::vca_1d)
        ),

        (*apvt.
            getParameter(ID::vca_2a)
        ),
        (*apvt.
            getParameter(ID::vca_2b)
        ),
        (*apvt.
            getParameter(ID::vca_2c)
        ),
        (*apvt.
            getParameter(ID::vca_2d)
        ),

        (*apvt.
            getParameter(ID::vca_3a)
        ),
        (*apvt.
            getParameter(ID::vca_3b)
        ),
        (*apvt.
            getParameter(ID::vca_3c)
        ),
        (*apvt.
            getParameter(ID::vca_3d)
        ),

        (*apvt.
            getParameter(ID::vca_4a)
        ),
        (*apvt.
            getParameter(ID::vca_4b)
        ),
        (*apvt.
            getParameter(ID::vca_4c)
        ),
        (*apvt.
            getParameter(ID::vca_4d)
        )
    } {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_1a, "VCA 1A", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_1b, "VCA 1B", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_1c, "VCA 1C", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_1d, "VCA 1D", -4.0f, 4.0f, 0.0f));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_2a, "VCA 2A", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_2b, "VCA 2B", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_2c, "VCA 2C", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_2d, "VCA 2D", -4.0f, 4.0f, 0.0f));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_3a, "VCA 3A", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_3b, "VCA 3B", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_3c, "VCA 3C", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_3d, "VCA 3D", -4.0f, 4.0f, 0.0f));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_4a, "VCA 4A", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_4b, "VCA 4B", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_4c, "VCA 4C", -4.0f, 4.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca_4d, "VCA 4D", -4.0f, 4.0f, 0.0f));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    if (channelIndex <= I_SIG_4R) {
        String n = "In ";
        unsigned idx = (channelIndex / 2) + 1;
        n = n + String(idx);
        if (channelIndex % 2 == 0) { n = n + String("L"); }
        else { n = n + String("R"); }
        return n;
    } else if (channelIndex <= I_VCA_4D) {
        unsigned idx = channelIndex - I_VCA_1A;
        String n = "Vca ";
        unsigned in = (idx / 4) + 1;
        unsigned out = idx % 4;
        char buf[2] = "A";
        buf[0] = 'A' + out;
        n = n + String(in) + String(buf);
        return n;
    }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    if (channelIndex <= O_SIG_DR) {
        String n = "Out ";
        unsigned idx = (channelIndex / 2);
        char buf[2] = "A";
        buf[0] = 'A' + idx;
        n = n + String(buf);
        if (channelIndex % 2 == 0) { n = n + String("L"); }
        else { n = n + String("R"); }
        return n;
    }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate,samplesPerBlock);
    outBufs_.setSize(MAX_SIG_OUT * 2, samplesPerBlock);
    workBuf_.setSize(1, samplesPerBlock);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned n = buffer.getNumSamples();

    for (unsigned i = 0; i < MAX_SIG_OUT * 2; i++) {
        //clear output buffer
        outBufs_.applyGain(i, 0, n, 0.0f);
    }

    for (unsigned out = 0; out < MAX_SIG_OUT; out++) {
        unsigned outL = O_SIG_AL + (out * 2);
        unsigned outR = O_SIG_AR + (out * 2);
        bool outEnabledL = outputEnabled[outL];
        bool outEnabledR = outputEnabled[outR];
        if (outEnabledL || outEnabledR) {

            for (unsigned in = 0; in < MAX_SIG_IN; in++) {
                unsigned inL = I_SIG_1L + (in * 2);
                unsigned inR = I_SIG_1R + (in * 2);
                bool inEnabledL = inputEnabled[inL];
                bool inEnabledR = inputEnabled[inR];

                unsigned vcaI = (in * 4) + out + I_VCA_1A;
                bool vcaEnabled = inputEnabled[vcaI];
                PluginParams::Parameter &p = params_.vca[in][out];

                float vca = p.convertFrom0to1(p.getValue());
                if (vcaEnabled) {
                    // vca cv enable
                    float *vcaflts = buffer.getWritePointer(vcaI);
                    lastVcaCV_[in][out] = vcaflts[0]; // without vca, we add this in UI
                    FloatVectorOperations::add(vcaflts, vca, n); // add param

                    if (inEnabledL) {
                        workBuf_.copyFrom(0, 0, buffer, inL, 0, n);
                        float *wflts = workBuf_.getWritePointer(0);
                        FloatVectorOperations::multiply(wflts, vcaflts, n);
                        outBufs_.addFrom(outL, 0, workBuf_, 0, 0, n);
                    }
                    if (inEnabledR) {
                        workBuf_.copyFrom(0, 0, buffer, inR, 0, n);
                        float *wflts = workBuf_.getWritePointer(0);
                        FloatVectorOperations::multiply(wflts, vcaflts, n);
                        outBufs_.addFrom(outR, 0, workBuf_, 0, 0, n);
                    }

                } else {
                    // vca cv not enabled
                    lastVcaCV_[in][out] = 0.0f;
                    if (inEnabledL) {
                        outBufs_.addFrom(outL, 0, buffer, inL, 0, n, vca);
                    }
                    if (inEnabledR) {
                        outBufs_.addFrom(outR, 0, buffer, inR, 0, n, vca);
                    }
                }
            }
        } else {
            for (unsigned in = 0; in < MAX_SIG_IN; in++) {
                lastVcaCV_[in][out] = 0.0f;
            }
        }
    }

    for (unsigned out = 0; out < MAX_SIG_OUT; out++) {
        unsigned outL = O_SIG_AL + out * 2;
        unsigned outR = O_SIG_AR + out * 2;
        bool outEnabledL = outputEnabled[outL];
        bool outEnabledR = outputEnabled[outR];
        if (outEnabledL) {
            buffer.copyFrom(outL, 0, outBufs_, outL, 0, n);
        } else {
            buffer.applyGain(outL, 0, n, 0.0f);
        }

        if (outEnabledR) {
            buffer.copyFrom(outR, 0, outBufs_, outR, 0, n);
        } else {
            buffer.applyGain(outR, 0, n, 0.0f);
        }
    }
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this,new PluginEditor(*this));
}


AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


