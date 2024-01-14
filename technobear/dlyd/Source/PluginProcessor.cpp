#include "PluginProcessor.h"
#include "PluginMiniEditor.h"
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

inline float panGain(bool left, float p) {
    static constexpr float PIdiv2 = M_PI / 2.0f;
    float pan = (p + 1.0f) / 2.0f;
    if (left) {
        return cosf(pan * PIdiv2);
        //return std::cosf(pan * PIdiv2);
    }
    return sinf(pan * PIdiv2);
    //return std::sinf(pan * PIdiv2);
}

PluginProcessor::Tap::Tap(AudioProcessorValueTreeState &apvt, unsigned id) :
    id_(id), pid_(getTapPid(id)),
    time(*apvt.getParameter(getTapParamId(id, ID::time))),
    hpf(*apvt.getParameter(getTapParamId(id, ID::hpf))),
    lpf(*apvt.getParameter(getTapParamId(id, ID::lpf))),
    noise(*apvt.getParameter(getTapParamId(id, ID::noise))),
    pan(*apvt.getParameter(getTapParamId(id, ID::pan))),
    level(*apvt.getParameter(getTapParamId(id, ID::level))),
    feedback(*apvt.getParameter(getTapParamId(id, ID::feedback))) {

}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    size(*apvt.getParameter(ID::size)),
    mix(*apvt.getParameter(ID::mix)),
    in_level(*apvt.getParameter(ID::in_level)),
    out_level(*apvt.getParameter(ID::out_level)),
    freeze(*apvt.getParameter(ID::freeze)) {
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
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::freeze, "Freeze", false));

    auto taps = std::make_unique<AudioProcessorParameterGroup>(ID::taps, String(ID::taps), ID::separator);
    for (unsigned tid = 0; tid < MAX_TAPS; tid++) {
        char la[2];
        la[0] = 'A' + tid;
        la[1] = 0;
        String desc = "Tap " + String(la) + " ";
        auto tap = std::make_unique<AudioProcessorParameterGroup>(getTapPid(tid), getTapPid(tid), ID::separator);
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::time), desc + "Time", 0.0f, 100.0f, (tid + 1) * 5.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::hpf), desc + "HPF", 0.0f, 20000.0f, 10.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::lpf), desc + "LPF", 0.0f, 20000.0f, 20000.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::noise), desc + "Dirt", 0.0f, 100.0f, 0.0f));
        tap->addChild(std::make_unique<ssp::BaseFloatParameter>(getTapParamId(tid, ID::pan), desc + "Pan", -1.0f, 1.0f, 0.0f));
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
        "In 2",
        "Time 1 A",
        "Time 1 B",
        "Time 1 C",
        "Time 1 D",
        "Time 2 A",
        "Time 2 B",
        "Time 2 C",
        "Time 2 D"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out 1",
        "Out 2",
        "Tap 1 A",
        "Tap 1 B",
        "Tap 1 C",
        "Tap 1 D",
        "Tap 2 A",
        "Tap 2 B",
        "Tap 2 C",
        "Tap 2 D"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    for (int line = 0; line < N_DLY_LINES; line++) {
        auto &dline = delayLines_[line];
        dline.line_.Init();
        for (int t = 0; t < MAX_TAPS; t++) {
            dline.taps_[t].hpf_.Init(newSampleRate);
            dline.taps_[t].lpf_.Init(newSampleRate);
            dline.taps_[t].noise_.Init();
        }
    }
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if(activityCount_==0) {
        for(int i=0;i<I_MAX;i++) {
            inActivity_[i]=buffer.getSample(i,0);
        }
    }
    unsigned sz = buffer.getNumSamples();

    float size = params_.size.getValue();
    float mix = params_.mix.getValue();
    bool freeze = params_.freeze.getValue() > 0.5f;
    float inlvl = freeze ? 0.0f : normValue(params_.in_level) / 100.0f; // lvl can be > 100
    float outlvl = normValue(params_.out_level) / 100.0f;
    float maxtime = size * float(MAX_DELAY);

    inRms_[0].process(buffer, I_IN_1);
    inRms_[1].process(buffer, I_IN_2);

    struct {
        float time_;
        float level_;
        float feedback_;
        float noiseamt_;
        float panamt_;
    } tapvalues[N_DLY_LINES][MAX_TAPS];

    // control rate handling
    for (int line = 0; line < N_DLY_LINES; line++) {
        for (int t = 0; t < MAX_TAPS; t++) {
            auto &v = tapvalues[line][t];
            auto &tap = delayLines_[line].taps_[t];
            auto &tap_params = params_.taps_[t];

            v.time_ = tap_params->time.getValue();
            v.level_ = tap_params->level.getValue();
            v.feedback_ = tap_params->feedback.getValue();
            v.noiseamt_ = tap_params->noise.getValue();
            float pan = normValue(tap_params->pan);
            v.panamt_ = panGain(line == 0, pan);

            float hpf = normValue(tap_params->hpf);
            float lpf = normValue(tap_params->lpf);
            if (tap.lpf_.GetFreq() != lpf) tap.lpf_.SetFreq(lpf);
            if (tap.hpf_.GetFreq() != hpf) tap.hpf_.SetFreq(hpf);
        }
    }

    // sample rate processing
    for (int s = 0; s < sz; s++) {
        for (int line = 0; line < N_DLY_LINES; line++) {

            auto &dlyline = delayLines_[line];
            float in = buffer.getSample(line, s) * inlvl;
            float wet = 0.0f;
            float fbk = 0.0f;
            for (int t = 0; t < MAX_TAPS; t++) {
                auto &tv = tapvalues[line][t];
                unsigned sigoffset = (line * MAX_TAPS) + t;
                float cvtime = buffer.getSample(I_TIME_1_TA + sigoffset, s);
                auto &tap = delayLines_[line].taps_[t];
                float noisesig = tap.noise_.Process() * tv.noiseamt_;
                float time = constrain(tv.time_ + cvtime, 0.0f, 1.0f) * maxtime;
                float v = dlyline.line_.Read(time) + noisesig;
                v = tap.lpf_.Process(v);
                v = tap.hpf_.Process(v);
                wet += v * tv.level_ * tv.panamt_ * mix;
                fbk += v * tv.feedback_;
                buffer.setSample(O_OUT_1_TA + sigoffset, s, v);
            }
            dlyline.line_.Write(in + fbk);
            float out = (in * (1.0f - mix) + wet) * outlvl;
            buffer.setSample(line, s, out);
        }
    }

    outRms_[0].process(buffer, O_OUT_1);
    outRms_[1].process(buffer, O_OUT_2);

    if(activityCount_==0) {
        for(int i=0;i<O_MAX;i++) {
            outActivity_[i]=buffer.getSample(i,0);
        }
    }
    activityCount_ = (activityCount_ + 1 ) % ACTIVITY_PERIOD;
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
