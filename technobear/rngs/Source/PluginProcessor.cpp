#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMiniEditor.h"
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

    inLevel_ = 0.0f;

    io_buf_sz_ = RingsBlock;
    in_buf_ = new float[io_buf_sz_];
    out_buf_ = new float[io_buf_sz_];
    aux_buf_ = new float[io_buf_sz_];

    strummer_.Init(0.01f, 48000 / RingsBlock);
    string_synth_.Init(buffer);
    part_.Init(buffer);
    initPart(true);
}

PluginProcessor::~PluginProcessor() {
    delete[] in_buf_;
    delete[] out_buf_;
    delete[] aux_buf_;
    in_buf_ = nullptr;
    out_buf_ = nullptr;
    aux_buf_ = nullptr;
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    pitch(*apvt.getParameter(ID::pitch)),
    structure(*apvt.getParameter(ID::structure)),
    brightness(*apvt.getParameter(ID::brightness)),
    damping(*apvt.getParameter(ID::damping)),
    position(*apvt.getParameter(ID::position)),
    polyphony(*apvt.getParameter(ID::polyphony)),
    model(*apvt.getParameter(ID::model)),
    //bypass(*apvt.getParameter(ID::bypass)),
    //easter_egg(*apvt.getParameter(ID::easter_egg)),
    in_gain(*apvt.getParameter(ID::in_gain)),
    enableIn(*apvt.getParameter(ID::enable_in)),
    enableStrum(*apvt.getParameter(ID::enable_strum)),
    enableVoct(*apvt.getParameter(ID::enable_voct)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -36.0f, +36.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::structure, "Structure", 0.0f, 100.0f, 33.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::brightness, "Brightness", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::damping, "Damping", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::position, "Position", 0.0f, 100.0f, 50.0f));

    StringArray poly;
    poly.add("1");
    poly.add("2");
    poly.add("4");
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::polyphony, "Polyphony", poly, 0));

    StringArray models;
    models.add("Modal");
    models.add("Sym Str");
    models.add("Inh Str");
    models.add("FM");
    models.add("Syn Q Str");
    models.add("Rvb Str");
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::model, "Model", models, 0));
//    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::bypass, "bypass", false));
//    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::easter_egg, "easter_egg", false));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::in_gain, "In Gain", 0.0f, 100.0f, 0.0f));

    bool enableIO[3] = {false,false,false};
#ifdef TARGET_QSP
    enableIO[1] = true;
#endif
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::enable_in, "In", enableIO[0]));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::enable_strum, "Strum", enableIO[1]));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::enable_voct, "VOct", enableIO[2]));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In",
        "Strum",
        "VOct",
        "FM",
        "Struct",
        "Bright",
        "Damp",
        "Pos"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Odd",
        "Even"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);
    strummer_.Init(0.01f, sampleRate / RingsBlock);
    string_synth_.Init(buffer);
    part_.Init(buffer);

    initPart(true);
}

void PluginProcessor::initPart(bool force) {
    int polyphony = constrain(
        1 << int(params_.polyphony.convertFrom0to1(params_.polyphony.getValue())),
        1, rings::kMaxPolyphony);

    if (force || polyphony != part_.polyphony()) {
        part_.set_polyphony(polyphony);
        string_synth_.set_polyphony(polyphony);
    }

    int imodel = constrain(params_.model.convertFrom0to1(params_.model.getValue()),
                           0, rings::ResonatorModel::RESONATOR_MODEL_LAST - 1);
    rings::ResonatorModel model = static_cast<rings::ResonatorModel>(imodel);
    if (force || model != part_.model()) {
        part_.set_model(model);
        string_synth_.set_fx(static_cast<rings::FxType>(model));
    }
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    if(activityCount_==0) {
        for(int i=0;i<I_MAX;i++) {
            inActivity_[i]=buffer.getSample(i,0);
        }
    }


    auto n = RingsBlock;
    size_t size = n;

    inRms_.process(buffer, I_IN);
    bool stereoOut = outputEnabled[O_EVEN];

    float p_in_gain = params_.in_gain.getValue();

    // Rings usually has a blocks size of 16,
    // SSP = 128 (@48k), so split up, so we read the control rate date every 16
    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {


        float gain = (p_in_gain * 4.0f);
        float in_gain = constrain(1.0f + (gain * gain), 1.0f, 17.0f);

        bool strum = false;
        for (int i = 0; i < n; i++) {
            in_buf_[i] = out_buf_[i] = buffer.getSample(I_IN, bidx + i) * in_gain;
            bool trig = buffer.getSample(I_STRUM, bidx + i) > 0.5;
            if (trig != trig_ && trig) {
                strum = true;
            }
            trig_ = trig;
        }

        // control rate
        static constexpr float RngsPitchOffset = 30.f - 6.0f; // 30 is normal, but 6. makes it in turn at 12 oclock
        float transpose =
            params_.pitch.convertFrom0to1(params_.pitch.getValue())
            + RngsPitchOffset
            + (noteInput_ ? noteInputTranspose_ : 0.0f);

        float note = cv2Pitch(buffer.getSample(I_VOCT, bidx));
        float fm = cv2Pitch(buffer.getSample(I_FM, bidx));
        float damping = params_.damping.getValue() + buffer.getSample(I_DAMPING, bidx);
        float structure = params_.structure.getValue() + buffer.getSample(I_STRUCTURE, bidx);
        float brightness = params_.brightness.getValue() + buffer.getSample(I_BRIGHTNESS, bidx);
        float position = params_.position.getValue() + buffer.getSample(I_POSITION, bidx);
        float chord = patch_.structure * rings::kNumChords - 1;

        patch_.brightness = constrain(brightness, 0.0f, 1.0f);
        patch_.damping = constrain(damping, 0.0f, 1.0f);
        patch_.position = constrain(position, 0.0f, 1.0f);
        patch_.structure = constrain(structure, 0.0f, 0.9995f);

        performance_state_.fm = constrain(fm, -48.0f, 48.0f);
        performance_state_.note = note;
        performance_state_.tonic = 12.0f + transpose;

        performance_state_.internal_exciter = inputEnabled[I_IN] < 0.5f || params_.enableIn.getValue()  < 0.5;
        performance_state_.internal_strum = inputEnabled[I_STRUM] < 0.5f || params_.enableStrum.getValue()  < 0.5;;
        performance_state_.internal_note = inputEnabled[I_VOCT] < 0.5f || params_.enableVoct.getValue()  < 0.5;;
        performance_state_.chord = constrain(chord, 0, rings::kNumChords - 1);;

        if (!performance_state_.internal_note) {
            // performance_state.note = 0.0;
            // quantize if using v/oct input
            performance_state_.tonic = 12.0f + roundf(transpose);
        }

        performance_state_.strum = strum;

        initPart(false);

        bool bypass = false; //params_.bypass.getValue() > 0.5;
        part_.set_bypass(bypass);
        bool easter_egg = false; //params_.easter_egg.getValue() > 0.5;

        if (easter_egg) {
            strummer_.Process(NULL, size, &(performance_state_));
            string_synth_.Process(performance_state_, patch_, in_buf_, out_buf_, aux_buf_, size);
        } else {
            // Apply noise gate.
            for (size_t i = 0; i < size; ++i) {
                float in_sample = in_buf_[i];
                float error, gain;
                error = in_sample * in_sample - inLevel_;
                inLevel_ += error * (error > 0.0f ? 0.1f : 0.0001f);
                gain = inLevel_ <= kNoiseGateThreshold
                       ? (1.0f / kNoiseGateThreshold) * inLevel_ : 1.0f;
                in_buf_[i] = gain * in_sample;
            }
            strummer_.Process(in_buf_, size, &(performance_state_));
            part_.Process(performance_state_, patch_, in_buf_, out_buf_, aux_buf_, size);
        }

        if (stereoOut) {
            for (int i = 0; i < RingsBlock; i++) {
                buffer.setSample(O_ODD, bidx + i, out_buf_[i]);
                buffer.setSample(O_EVEN, bidx + i, aux_buf_[i]);
            }
        } else {
            for (int i = 0; i < RingsBlock; i++) {
                buffer.setSample(O_ODD, bidx + i, (out_buf_[i] + aux_buf_[i]) / 2.0f);
            }
        }
    }

    outRms_[0].process(buffer, O_ODD);
    if (stereoOut) outRms_[1].process(buffer, O_EVEN);

    if(activityCount_==0) {
        for(int i=0;i<O_MAX;i++) {
            outActivity_[i]=buffer.getSample(i,0);
        }
    }
    activityCount_ = (activityCount_ + 1 ) % ACTIVITY_PERIOD;
}

void PluginProcessor::setStateInformation(const void *data, int sizeInBytes) {
    ssp::BaseProcessor::setStateInformation(data, sizeInBytes);
    initPart(true);
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

