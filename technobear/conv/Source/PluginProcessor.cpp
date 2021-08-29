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


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    mix(*apvt.getParameter(ID::mix)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::mix, "Mix", 0.0f, 100.0f, 50.0f));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "InL",
        "InR",
        "Mix"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "OutL",
        "OutR"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);
    workBuf_.setSize(2, samplesPerBlock);

//    const float TEST_SIZE = sampleRate;
    const float TEST_SIZE = 2048 * 8;
    IRBuf_.setSize(2, TEST_SIZE);

    float *tL = IRBuf_.getWritePointer(0);
    float *tR = IRBuf_.getWritePointer(1);

    for (size_t i = 0; i < TEST_SIZE; i++) {
        float multiply = 1.0f - (float) i / (float) TEST_SIZE;
        multiply = multiply * multiply;
        tL[i] = (rand() % 10000) * 0.0001f * multiply;
        tR[i] = (rand() % 10000) * 0.0001f * multiply;
    }


    juce::dsp::ProcessSpec specs;
    specs.maximumBlockSize = samplesPerBlock;
    specs.numChannels = 2;
    specs.sampleRate = sampleRate;
    convolution_.prepare(specs);
    convolution_.loadImpulseResponse(AudioBuffer<float>(IRBuf_),
                                     sampleRate,
                                     dsp::Convolution::Stereo::yes,
                                     dsp::Convolution::Trim::no,
                                     dsp::Convolution::Normalise::no);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned n = buffer.getNumSamples();
    workBuf_.copyFrom(0, 0, buffer, I_INL, 0, n);
    workBuf_.copyFrom(1, 0, buffer, I_INR, 0, n);

    bool stereoIn = inputEnabled[I_INR];
    inRms_[0].process(buffer, I_INL);
    if (stereoIn) inRms_[1].process(buffer, I_INL);

    juce::dsp::AudioBlock<float> block(workBuf_);
    juce::dsp::ProcessContextReplacing<float> context(block);
    convolution_.process(context);

    buffer.copyFrom(O_OUTL, 0, workBuf_, 0, 0, n);
    buffer.copyFrom(O_OUTR, 0, workBuf_, 1, 0, n);

    bool stereoOut = outputEnabled[O_OUTR];
    outRms_[0].process(buffer, O_OUTL);
    if (stereoOut) outRms_[1].process(buffer, O_OUTR);
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    ssp::BaseProcessor::setStateInformation(data, sizeInBytes);
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

