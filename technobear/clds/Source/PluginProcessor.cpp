
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ssp/EditorHost.h"

inline float constrainFloat(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

inline float TO_SHORTFRAME(float v) { return constrainFloat(v * 32767.0f, -32768.0f, 32767.0f); }

inline float FROM_SHORTFRAME(short v) { return (float(v) / 32768.0f); }

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
    granularProcessor_ = nullptr;
    ibuf_ = obuf_ = nullptr;
    block_mem_ = block_ccm_ = nullptr;
}

PluginProcessor::~PluginProcessor() {
    if (ibuf_) delete[] ibuf_;
    if (obuf_) delete[] obuf_;
    if (block_ccm_) delete[] block_ccm_;
    if (block_mem_) delete[] block_mem_;
    if (granularProcessor_) delete granularProcessor_;
}

PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) :
    position(*apvt.getParameter(ID::position)),
    size(*apvt.getParameter(ID::size)),
    density(*apvt.getParameter(ID::density)),
    texture(*apvt.getParameter(ID::texture)),

    mix(*apvt.getParameter(ID::mix)),
    spread(*apvt.getParameter(ID::spread)),
    feedback(*apvt.getParameter(ID::feedback)),
    reverb(*apvt.getParameter(ID::reverb)),

    pitch(*apvt.getParameter(ID::pitch)),
    mode(*apvt.getParameter(ID::mode)),
    in_gain(*apvt.getParameter(ID::in_gain)),
    freeze(*apvt.getParameter(ID::freeze)) {
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::position, "Position", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::size, "Size", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::density, "Density", -100.0f, 100.0f, -20.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::texture, "Texture", 0.0f, 100.0f, 50.0f));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::mix, "Mix", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::spread, "Spread", 0.0f, 100.0f, 50.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::feedback, "Feedback", 0.0f, 100.0f, 10.0f));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::reverb, "Reverb", 0.0f, 100.0f, 50.0f));

    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::pitch, "Pitch", -48.0f, +48.0f, 0.0f));
    StringArray modes;
    modes.add("Granular");
    modes.add("Stretch");
    modes.add("Delay");
    modes.add("Spectral");
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::mode, "Mode", modes, 0));
    params.add(std::make_unique<ssp::BaseFloatParameter>(ID::in_gain, "In Gain", 0.0f, 100.0f, 0.0f));
    params.add(std::make_unique<ssp::BaseBoolParameter>(ID::freeze, "Freeze", false));
    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "In L",
        "In R",
        "Trig",
        "VOct",
        "Pos",
        "Size",
        "Density",
        "Texture",
        "Freeze",
        "Mix",
        "Spread",
        "Feedback",
        "Reverb"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "Out L",
        "Out R"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    if (granularProcessor_ == nullptr) {
        granularProcessor_ = new clouds::GranularProcessor;
        auto &processor = *granularProcessor_;

        ibuf_ = new clouds::ShortFrame[IO_BUF_SZ];
        obuf_ = new clouds::ShortFrame[IO_BUF_SZ];
        block_mem_ = new uint8_t[BLOCK_MEM_SZ * 2];
        block_ccm_ = new uint8_t[BLOCK_CCM_SZ * 2];
        memset(block_mem_, 0, sizeof(uint8_t) * BLOCK_MEM_SZ * 2);
        memset(block_ccm_, 0, sizeof(uint8_t) * BLOCK_CCM_SZ * 2);
        memset(processor.mutable_parameters(), 0, sizeof(clouds::Parameters));
        processor.Init(
            block_mem_, BLOCK_MEM_SZ,
            block_ccm_, BLOCK_CCM_SZ);
    }


    auto &processor = *granularProcessor_;

    auto n = CloudsBlock;

    bool stereoIn = inputEnabled[I_RIGHT];
    bool stereoOut = outputEnabled[O_RIGHT];

    inRms_[0].process(buffer, I_LEFT);
    if (stereoIn) inRms_[1].process(buffer, I_RIGHT);

    // note: clouds is run at 32khz, we are running at 48khz!
    // Clouds usually has a blocks size of 16,(?)
    // SSP = 128 (@48k), so split up, so we read the control rate date every 16
    float p_in_gain = params_.in_gain.getValue();

    for (int bidx = 0; bidx < buffer.getNumSamples(); bidx += n) {

        bool trig = false;
        float gain = (p_in_gain * 4.0f);
        float in_gain = constrainFloat(1.0f + (gain * gain), 1.0f, 17.0f);

        for (int i = 0; i < n; i++) {

            ibuf_[i].l = TO_SHORTFRAME(buffer.getSample(I_LEFT, bidx + i) * in_gain);
            ibuf_[i].r = stereoIn ? TO_SHORTFRAME(buffer.getSample(I_RIGHT, bidx + i) * in_gain) : ibuf_[i].l;

            if (buffer.getSample(I_TRIG, bidx + i) > 0.5) {
                trig = true;
            }
        }

        // control rate
        int imode = params_.mode.convertFrom0to1(params_.mode.getValue());
        clouds::PlaybackMode mode = (clouds::PlaybackMode) (imode % clouds::PLAYBACK_MODE_LAST);

        processor.set_playback_mode(mode);
        processor.set_silence(false);
        processor.set_bypass(false);
        processor.set_num_channels(2);
        processor.set_low_fidelity(false);
        processor.Prepare();

        // processor.set_quality();
        // processor.set_bypass(data.f_bypass > 0.5f);
        // processor.set_silence(data.f_silence > 0.5f);
        // processor.set_num_channels(f_mono  < 0.5f ? 1 : 2 );
        // processor.set_low_fidelity(f_lo_fi > 0.5f);


        auto p = processor.mutable_parameters();

        float pitch = params_.pitch.convertFrom0to1(params_.pitch.getValue()) + cv2Pitch(buffer.getSample(I_VOCT, bidx));
        float position = params_.position.getValue() + buffer.getSample(I_POS, bidx);
        float size = params_.size.getValue() + buffer.getSample(I_SIZE, bidx);
        float density = params_.density.getValue() + buffer.getSample(I_DENSITY, bidx);
        float texture = params_.texture.getValue() + buffer.getSample(I_TEXT, bidx);
        bool freeze = params_.freeze.getValue() > 0.5f || (buffer.getSample(I_FREEZE, bidx) > 0.5f);

        float mix = params_.mix.getValue() + buffer.getSample(I_MIX, bidx);
        float spread = params_.spread.getValue() + buffer.getSample(I_SPREAD, bidx);
        float feedback = params_.feedback.getValue() + buffer.getSample(I_FEEDBACK, bidx);
        float reverb = params_.reverb.getValue() + buffer.getSample(I_REVERB, bidx);

        //restrict density to .2 to .8 for granular mode, outside this breaks up
        // density = constrain(density, 0.0f, 1.0f);
        // density = (mode == clouds::PLAYBACK_MODE_GRANULAR) ? (density * 0.6f) + 0.2f : density;

        p->freeze = freeze;

        p->gate = trig;
        p->trigger = p->gate;

        p->pitch = constrainFloat(pitch, -48.0f, 48.0f);
        p->position = constrainFloat(position, 0.0f, 1.0f);
        p->size = constrainFloat(size, 0.0f, 1.0f);
        p->texture = constrainFloat(texture, 0.0f, 1.0f);
        p->density = constrainFloat(density, 0.0f, 1.0f);

        p->dry_wet = constrainFloat(mix, 0.0f, 1.0f);
        p->stereo_spread = constrainFloat(spread, 0.0f, 1.0f);
        p->feedback = constrainFloat(feedback, 0.0f, 1.0f);
        p->reverb = constrainFloat(reverb, 0.0f, 1.0f);

        processor.Process(ibuf_, obuf_, n);

        if (stereoOut) {
            for (int i = 0; i < CloudsBlock; i++) {
                buffer.setSample(O_LEFT, bidx + i, FROM_SHORTFRAME(obuf_[i].l));
                buffer.setSample(O_RIGHT, bidx + i, FROM_SHORTFRAME(obuf_[i].r));
            }
        } else {
            for (int i = 0; i < CloudsBlock; i++) {
                buffer.setSample(O_LEFT, bidx + i, (FROM_SHORTFRAME(obuf_[i].l) + FROM_SHORTFRAME(obuf_[i].r)) / 2.0f);
            }
        }
    }
    outRms_[0].process(buffer, O_LEFT);
    if (stereoOut) outRms_[1].process(buffer, O_RIGHT);

}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this,new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}



