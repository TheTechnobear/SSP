#include "PluginProcessor.h"
#include "PluginEditor.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {

#if __APPLE__
    // so that we get some sound, without patching
    inputEnabled[I_TRIG]=false;
    inputEnabled[I_LEVEL]=false;
#endif

}


PluginProcessor::~PluginProcessor() {
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
    vca(*apvt.getParameter(ID::vca)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -30.0f, +30.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::harmonics, "Harmonics", 0.0f, 100.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::timbre, "Timbre", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::morph, "Morph", 0.0f, 100.0f, 50.0f));


    StringArray models;
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
    stmlib::BufferAllocator allocator(shared_buffer, sizeof(shared_buffer));
    voice.Init(&allocator);
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    auto n = PltsBlock;

    bool auxOut = outputEnabled[O_AUX] > 0.5f;

    // bool voctEn  = inputEnabled[ I_VOCT ] > 0.5f;
    bool fmEn = inputEnabled[I_FM] > 0.5f;
    bool timbreEn = inputEnabled[I_TIMBRE] > 0.5f;
    bool morphEn = inputEnabled[I_MORPH] > 0.5f;
    bool trigEn = inputEnabled[I_TRIG] > 0.5f;
    bool levelEn = inputEnabled[I_LEVEL] > 0.5f;

    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {

        bool trig = false; //todo check
        for (int i = 0; i < n; i++) {
            bool t = buffer.getSample(I_TRIG, bidx + i) > 0.5;
            if (t != trig_ && t) {
                trig = true;
            }
            trig_ = t;
        }

        // static constexpr float PltsPitchOffset = 60.0f - 3.044f;
        static constexpr float PltsPitchOffset = 60.0f;
        float pitch = params_.pitch.convertFrom0to1(params_.pitch.getValue()) + PltsPitchOffset;

        patch.engine = constrain(params_.model.convertFrom0to1(params_.model.getValue()),
                                 0.0f, PltsMaxEngine); //check

        patch.note = pitch;
        //patch.note = 60.f + pitch * 12.f;
        patch.harmonics = params_.harmonics.getValue();
        patch.timbre = params_.timbre.getValue();
        patch.morph = params_.morph.getValue();
        patch.lpg_colour = params_.lpg.getValue();
        patch.decay = params_.vca.getValue();

        patch.frequency_modulation_amount = (params_.freq_mod.getValue() * 2.0f) - 1.0f;
        patch.timbre_modulation_amount = (params_.timbre_mod.getValue() * 2.0f) - 1.0f;
        patch.morph_modulation_amount = (params_.morph_mod.getValue() * 2.0f) - 1.0f;

        // Construct modulations
        plaits::Modulations modulations;
        modulations.engine = buffer.getSample(I_MODEL, bidx);
        modulations.note = cv2Pitch(buffer.getSample(I_VOCT, bidx));
        modulations.frequency = cv2Pitch(buffer.getSample(I_FM, bidx));
        modulations.harmonics = buffer.getSample(I_HARMONICS, bidx);
        modulations.timbre = buffer.getSample(I_TIMBRE, bidx) * 0.625f;
        modulations.morph = buffer.getSample(I_MORPH, bidx) * 0.625f;

        modulations.trigger = trig_; //TODO  trig || trig_ ?
        modulations.level = buffer.getSample(I_LEVEL, bidx) * 0.625f;

        // modulations.frequency_patched = voctEn;
        modulations.frequency_patched = fmEn;
        modulations.timbre_patched = timbreEn;
        modulations.morph_patched = morphEn;
        modulations.trigger_patched = trigEn;
        modulations.level_patched = levelEn;

        // Render frames
        plaits::Voice::Frame output[PltsBlock];
        voice.Render(patch, modulations, output, PltsBlock);

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
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


