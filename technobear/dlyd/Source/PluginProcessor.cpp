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

String getTapPid(unsigned tid) {
    return String(ID::taps) + String(ID::separator) + String(tid);
}

String getTapParamId(unsigned tid, StringRef id) {
    return getTapPid(tid) + String(ID::separator) + id;
}


PluginProcessor::Tap::Tap(AudioProcessorValueTreeState &apvt, unsigned id) :
    id_(id), pid_(getTapPid(id)),
    time(*apvt.getParameter(getTapParamId(id, ID::time))),
    repeats(*apvt.getParameter(getTapParamId(id, ID::repeats))),
    pan(*apvt.getParameter(getTapParamId(id, ID::pan))),
    level(*apvt.getParameter(getTapParamId(id, ID::level))),
    feedback(*apvt.getParameter(getTapParamId(id, ID::feedback))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    size(*apvt.getParameter(ID::size)),
    mix(*apvt.getParameter(ID::mix)),
    in_level(*apvt.getParameter(ID::in_level)),
    out_level(*apvt.getParameter(ID::out_level)) {
    for (unsigned tid = 0; tid < MAX_TAPS; tid++) {
        auto tap = std::make_unique<Tap>(apvt, tid);
        taps_.push_back(std::move(tap));
    }
}

AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::size, "Size", 0.0f, 100.0f, 10.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::mix, "Mix", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::in_level, "In Lvl", 0.0f, 200.0f, 100.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::out_level, "Out Lvl", 0.0f, 200.0f, 100.0f));

    auto taps = std::make_unique<AudioProcessorParameterGroup>(ID::taps, String(ID::taps), ID::separator);
    for (unsigned tid = 0; tid < MAX_TAPS; tid++) {
        char la[2];
        la[0] = 'A' + tid;
        la[1] = 0;
        String desc = "Tap " + String(la) + " ";
        auto tap = std::make_unique<AudioProcessorParameterGroup>(getTapPid(tid), getTapPid(tid), ID::separator);
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::time), desc + "Time", 0.0f, 100.0f, (tid + 1) * 5.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::repeats), desc + "Repeats", 0.0f, 100.0f, 5.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::pan), desc + "Pan", -100.0f, 100.0f, 0.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::level), desc + "Level", 0.0f, 100.0f, 50.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::feedback), desc + "Feedback", 0, 100.0f, 0.0f));
        taps->addChild(std::move(tap));
    }
    params.add(std::move(taps));


    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In 1",
        "In 2"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out 1",
        "Out 2",
        "Tap A 1",
        "Tap B 1",
        "Tap C 1",
        "Tap D 1",
        "Tap A 2",
        "Tap B 2",
        "Tap C 2",
        "Tap D 2"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    for (int line = 0; line < N_DLY_LINES; line++) {
        auto &dlyline = delayLine_[line];
        dlyline.Init();
    }
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    // for testing
    jassert(N_DLY_LINES == I_MAX);

    float size = params_.size.getValue();
    float mix = params_.mix.getValue();
    float inlvl = normValue(params_.in_level) / 100.0f; // lvl can be > 100
    float outlvl = normValue(params_.out_level) / 100.0f;

    float maxtime = size * float(MAX_DELAY);

    for (int s = 0; s < sz; s++) {
        for (int line = 0; line < N_DLY_LINES; line++) {
            if (!isOutputEnabled(line)) continue; //?

            auto &dlyline = delayLine_[line];
//            dlyline.SetDelay(size * float(MAX_DELAY));

            float in = buffer.getSample(line, s);
            in *= inlvl;
            float wet = 0.0f;
            float fbk = 0.0f;
            for (int t = 0; t < MAX_TAPS; t++) {
                auto &tap = params_.taps_[t];
                float time = tap->time.getValue();

                float level = tap->level.getValue(); // out level?
                float feedback = tap->feedback.getValue(); // feedback level
                float repeats = tap->repeats.getValue();
                float pan = tap->pan.getValue(); // pan on main out?

                float v = dlyline.Read(maxtime * time);
                wet += v * level;
                fbk += v * feedback;
                // todo : pan (we can use line to determine if left or right)
                // todo : repeats?
                unsigned tsig = (line * MAX_TAPS) + t;
                buffer.setSample(O_OUT_TA_1 + tsig, s, v);
            }

            dlyline.Write(in + fbk);

            float out = (in * (1.0f - mix) + wet) * outlvl;
            buffer.setSample(line, s, out);
        }
    }
}


AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
