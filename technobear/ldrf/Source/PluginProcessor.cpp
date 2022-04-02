#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"

static constexpr float MAX_CUTOFF_FREQ = 15000.f;
static constexpr float MIN_CUTOFF_FREQ = 0.0f;

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
    for (unsigned i = 0; i < MAX_FILTERS; i++) {
        filters_.push_back(std::make_unique<daisysp::MoogLadder>());
    }
}

PluginProcessor::~PluginProcessor() {
}

String getFilterPid(unsigned id) {
    return String(ID::filters) + String(ID::separator) + String(id);
}

String getCutoffPid(unsigned fid) {
    return getFilterPid(fid) + String(ID::separator) + String(ID::cutoff);

}

String getResPid(unsigned fid) {
    return getFilterPid(fid) + String(ID::separator) + String(ID::res);

}

PluginProcessor::Filter::Filter(AudioProcessorValueTreeState &apvt, unsigned id) :
    id_(id), pid_(getFilterPid(id)),
    cutoff(*apvt.getParameter(getCutoffPid(id))),
    res(*apvt.getParameter(getResPid(id))) {
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) {
    for (unsigned id = 0; id < MAX_FILTERS; id++) {
        auto filter = std::make_unique<Filter>(apvt, id);
        filters_.push_back(std::move(filter));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    auto harmonics = std::make_unique<AudioProcessorParameterGroup>(ID::filters,
                                                                    String(ID::filters),
                                                                    ID::separator);
    for (unsigned id = 0; id < MAX_FILTERS; id++) {
        harmonics->addChild(std::make_unique<
            ssp::BaseFloatParameter>(getCutoffPid(id), "Cutoff " + String(id + 1), MIN_CUTOFF_FREQ, MAX_CUTOFF_FREQ, MAX_CUTOFF_FREQ));
        harmonics->addChild(
            std::make_unique<ssp::BaseFloatParameter>(getResPid(id), "Res " + String(id + 1), 0.0f, 1.0f, 0.0f));
    }
    params.add(std::move(harmonics));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In 1",
        "Cutoff 1",
        "Res 1",
        "In 2",
        "Cutoff 2",
        "Res 2",
        "In 3",
        "Cutoff 3",
        "Res 3",
        "In 4",
        "Cutoff 4",
        "Res 4"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out 1",
        "Out 2",
        "Out 2",
        "Out 4"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) {
    BaseProcessor::prepareToPlay(newSampleRate, estimatedSamplesPerBlock);
    for (auto &filter: filters_) {
        filter->Init(newSampleRate);
    }
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

//    static constexpr float baseNote = 60.0f;
//    float pitch =
//        normValue(params_.pitch)
//        + cv2Pitch(buffer.getSample(I_VOCT, 0))
//        + baseNote;
//    float freq = daisysp::mtof(pitch);
    static unsigned constexpr IN_MULT = I_IN_2 - I_IN_1;

    float cutoff[MAX_FILTERS];
    float res[MAX_FILTERS];

    for (unsigned f = 0; f < MAX_FILTERS; f++) {
        float cvCutoff = buffer.getSample((f * IN_MULT) + I_CUTOFF_1, 0) * MAX_CUTOFF_FREQ;
        float cvRes = buffer.getSample((f * IN_MULT) + I_RES_1, 0);

        float pCutoff = normValue(params_.filters_[f]->cutoff);
        float pRes = normValue(params_.filters_[f]->res);
        cutoff[f] = daisysp::fclamp(cvCutoff + pCutoff, MIN_CUTOFF_FREQ, MAX_CUTOFF_FREQ);
        res[f] = daisysp::fclamp(cvRes + pRes, 0.0f, 1.0f);

        if (isOutputEnabled(O_OUT_1 + f)) {
            filters_[f]->SetFreq(cutoff[f]);
            filters_[f]->SetRes(res[f]);
        }
    }


    for (unsigned f = 0; f < MAX_FILTERS; f++) {
        for (int s = 0; s < sz; s++) {
            unsigned och = O_OUT_1 + f;
            if (isOutputEnabled(och)) {
                unsigned ich = (f * IN_MULT) + I_IN_1;
                unsigned fch = ich;
                // cascading, think about this optionally... since we want to optimise when we dont use
                //            unsigned fch = (f == 0 || isInputEnabled(ich)) ? ich : och - 1;
                float in = buffer.getSample(fch, s);
                float compensation = 1 + (res[f]);
                buffer.setSample(och, s, filters_[f]->Process(in) * (compensation * compensation));
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
