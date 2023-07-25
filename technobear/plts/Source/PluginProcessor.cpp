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

    memset(shared_buffer_, 0, sizeof(shared_buffer_));
#if __APPLE__
    // so that we get some sound, without patching
    inputEnabled[I_TRIG] = false;
    inputEnabled[I_LEVEL] = false;
#endif

}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    pitch(*apvt.getParameter(ID::pitch)),
    harmonics(*apvt.getParameter(ID::harmonics)),
    timbre(*apvt.getParameter(ID::timbre)),
    morph(*apvt.getParameter(ID::morph)),
    model(*apvt.getParameter(ID::model)),
    freq_mod(*apvt.getParameter(ID::freq_mod)),
    timbre_mod(*apvt.getParameter(ID::timbre_mod)),
    morph_mod(*apvt.getParameter(ID::morph_mod)),
    lpg(*apvt.getParameter(ID::lpg)),
    vca(*apvt.getParameter(ID::vca)),
    enable_trig(*apvt.getParameter(ID::enable_trig)),
    enable_level(*apvt.getParameter(ID::enable_level)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -48.0f, +48.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::harmonics, "Harmonics", 0.0f, 100.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::timbre, "Timbre", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::morph, "Morph", 0.0f, 100.0f, 50.0f));


    StringArray models;
    models.add("AnalogVCF");
    models.add("PhaseDist");
    models.add("SixOp1");
    models.add("SixOp2");
    models.add("SixOp3");
    models.add("WTerrain");
    models.add("StringM");
    models.add("ChipTune");


    models.add("Analog");
    models.add("Waveshape");
    models.add("FM");
    models.add("Granular");
    models.add("Harmonic");
    models.add("Wavetable");
    models.add("Chords");
    models.add("Speech");

    models.add("Swarm");
    models.add("Flt Noise");
    models.add("Particle");
    models.add("Inh String");
    models.add("Modal");
    models.add("Bass Drum");
    models.add("Snare");
    models.add("HiHat");

    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::model, "Model", models, 0));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::freq_mod, "Freq Mod", -100.0f, 100.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::timbre_mod, "Timbre Mod", -100.0f, 100.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::morph_mod, "Morph Mod", -100.0f, 100.0f, 0.0f));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::lpg, "LPG", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::vca, "VCA", 0.0f, 100.0f, 50.0f));

    bool enableIO[2] = {false,false};
#ifdef TARGET_QSP
    enableIO[0] = true;
#endif
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::enable_trig, "Trig", enableIO[0]));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::enable_level, "Level", enableIO[1]));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "VOct",
        "Trig",
        "Level",
        "Harm",
        "Timbre",
        "Morph",
        "FM",
        "Model"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out",
        "Aux"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    BaseProcessor::prepareToPlay(sampleRate, samplesPerBlock);
    stmlib::BufferAllocator allocator(shared_buffer_, sizeof(shared_buffer_));
    voice_.Init(&allocator);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    auto n = PltsBlock;

    bool auxOut = outputEnabled[O_AUX];

    // bool voctEn  = inputEnabled[ I_VOCT ] > 0.5f;
    bool fmEn = inputEnabled[I_FM];
    bool timbreEn = inputEnabled[I_TIMBRE];
    bool morphEn = inputEnabled[I_MORPH];

    bool trigEn = inputEnabled[I_TRIG] || params_.enable_trig.getValue() > 0.5;
    bool levelEn = inputEnabled[I_LEVEL] || params_.enable_level.getValue() > 0.5;;

#ifdef TARGET_QSP
    // these all have attenuators so can be enabled with functional changes?!
    fmEn=true;
    timbeEn=true;
    morphEn=true;
#endif

    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {
        bool trig = false;
        for (int i = 0; i < n; i++) {
            bool t = buffer.getSample(I_TRIG, bidx + i) > 0.5;
            if (t != trig_ && t) {
                trig = true;
            }
            trig_ = t;
        }

        // static constexpr float PltsPitchOffset = 60.0f - 3.044f;
        static constexpr float PltsPitchOffset = 60.0f;
        float pitch =
            params_.pitch.convertFrom0to1(params_.pitch.getValue())
            + (noteInput_ ? noteInputTranspose_ : 0.0f);

        patch_.engine = (int) constrain(params_.model.convertFrom0to1(params_.model.getValue()),
                                        0.0f, PltsMaxEngine);

        patch_.note = PltsPitchOffset + pitch;
//        patch_.note = 60.f + pitch * 12.f;
        patch_.harmonics = params_.harmonics.getValue();
        patch_.timbre = params_.timbre.getValue();
        patch_.morph = params_.morph.getValue();
        patch_.lpg_colour = params_.lpg.getValue();
        patch_.decay = params_.vca.getValue();

        patch_.frequency_modulation_amount = (params_.freq_mod.getValue() * 2.0f) - 1.0f;
        patch_.timbre_modulation_amount = (params_.timbre_mod.getValue() * 2.0f) - 1.0f;
        patch_.morph_modulation_amount = (params_.morph_mod.getValue() * 2.0f) - 1.0f;

        // Construct modulations
        plaits::Modulations modulations{};
        modulations.engine = buffer.getSample(I_MODEL, bidx);
        modulations.note = cv2Pitch(buffer.getSample(I_VOCT, bidx));
        modulations.frequency = cv2Pitch(buffer.getSample(I_FM, bidx));
        modulations.harmonics = buffer.getSample(I_HARMONICS, bidx);
        modulations.timbre = buffer.getSample(I_TIMBRE, bidx) * 0.625f;
        modulations.morph = buffer.getSample(I_MORPH, bidx) * 0.625f;

        modulations.trigger = trig;
        modulations.level = buffer.getSample(I_LEVEL, bidx) * 0.625f;

        // modulations.frequency_patched = voctEn;
        modulations.frequency_patched = fmEn;
        modulations.timbre_patched = timbreEn;
        modulations.morph_patched = morphEn;
        modulations.trigger_patched = trigEn;
        modulations.level_patched = levelEn;

        // Render frames
        plaits::Voice::Frame output[PltsBlock];
        voice_.Render(patch_, modulations, output, PltsBlock);

        if (auxOut) {
            for (int i = 0; i < PltsBlock; i++) {
                buffer.setSample(O_OUT, bidx + i, output[i].out / 32768.f);
                buffer.setSample(O_AUX, bidx + i, output[i].aux / 32768.f);
            }
        } else {
            for (int i = 0; i < PltsBlock; i++) {
                buffer.setSample(O_OUT, bidx + i, output[i].out / 32768.f);
            }
            buffer.clear(O_AUX, 0, n);
        }
    }

    outRms_[0].process(buffer, O_OUT);
    if (auxOut) outRms_[1].process(buffer, O_AUX);

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
    if(useCompactUI()) {
        return new ssp::EditorHost(this, new PluginMiniEditor(*this),useCompactUI());

    } else {
        return new ssp::EditorHost(this, new PluginEditor(*this),useCompactUI());
    }
#endif
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


