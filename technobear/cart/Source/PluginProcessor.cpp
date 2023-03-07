#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}


Snakes PluginProcessor::snakes_;

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

String layerID(unsigned ln) {
    static const char *layerChar[] = {
        "x", "y", "c"
    };
    return layerChar[ln];

}

String getLayerPID(unsigned ln) {
    String pid = String(ID::layer) + String(ID::separator) + layerID(ln) + String(ID::separator);
    return pid;
}


String getLayerStepPID(unsigned ln, unsigned n) {
    String layerPid = getLayerPID(ln);
    String pid = layerPid + String(n) + String(ID::separator);
    return pid;
}


PluginProcessor::LayerStep::LayerStep(AudioProcessorValueTreeState &apvt, unsigned ln, unsigned sn) :
    cv(*apvt.getParameter(getLayerStepPID(ln, sn) + ID::cv)),
    access(*apvt.getParameter(getLayerStepPID(ln, sn) + ID::access)),
    gate(*apvt.getParameter(getLayerStepPID(ln, sn) + ID::gate)),
    glide(*apvt.getParameter(getLayerStepPID(ln, sn) + ID::glide)) {
    ;
}

PluginProcessor::Layer::Layer(AudioProcessorValueTreeState &apvt, unsigned ln) :
    snake(*apvt.getParameter(getLayerPID(ln) + ID::snake)),
    scale(*apvt.getParameter(getLayerPID(ln) + ID::scale)),
    root(*apvt.getParameter(getLayerPID(ln) + ID::root)),
    fun_op_trig(*apvt.getParameter(getLayerPID(ln) + ID::fun_op_trig)),
    fun_op_sleep(*apvt.getParameter(getLayerPID(ln) + ID::fun_op_sleep)),
    fun_mod_mode(*apvt.getParameter(getLayerPID(ln) + ID::fun_mod_mode)),
    fun_cv_mode(*apvt.getParameter(getLayerPID(ln) + ID::fun_cv_mode)) {
    ;
}


PluginProcessor::PluginParams::PluginParams(AudioProcessorValueTreeState &apvt) {
    for (unsigned i = 0; i < MAX_LAYER; i++) {
        auto layer = std::make_unique<Layer>(apvt, i);
        for (unsigned n = 0; n < 16; n++) {
            layer->steps_.push_back(std::make_unique<LayerStep>(apvt, i, n));
        }
        layers_.push_back(std::move(layer));
    }
}


AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout params;
    BaseProcessor::addBaseParameters(params);

    StringArray snakeTypes;
    for (unsigned s = 0; s < snakes_.size(); s++) {
        auto n = snakes_.name(s);
        snakeTypes.add(n);
    }

    StringArray modModes;
    modModes.add("None");
    modModes.add("Reset");
    modModes.add("Clk");
    modModes.add("RunStop");
    modModes.add("Dir");

    StringArray cvModes;
    cvModes.add("None");
    cvModes.add("Add");
    cvModes.add("Loc");
    cvModes.add("Snake");
    cvModes.add("S&H");

    auto &q = quantizer_;
    StringArray tonics;
    for (auto i = 0; i < MAX_TONICS; i++) {
        tonics.add(q.getTonicName(i));
    }
    params.add(std::make_unique<ssp::BaseChoiceParameter>(ID::root, "Root", tonics, 0));

    StringArray scales;
    for (auto i = 0; i < MAX_SCALES; i++) {
        scales.add(q.getScaleName(i));
    }

    auto lgs = std::make_unique<AudioProcessorParameterGroup>(ID::layer, "Layers", ID::separator);
    for (unsigned ln = 0; ln < MAX_LAYERS; ln++) {
        auto lg = std::make_unique<AudioProcessorParameterGroup>(layerID(ln), layerID(ln), ID::separator);
        lg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getLayerPID(ln) + ID::snake, "snake", snakeTypes, 0));
        lg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getLayerPID(ln) + ID::scale, "scale", scales, 0));
        lg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getLayerPID(ln) + ID::root, "root", tonics, 0));
        lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerPID(ln) + ID::fun_op_trig, "trig", false));
        lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerPID(ln) + ID::fun_op_sleep, "sleep", false));
        lg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getLayerPID(ln) + ID::fun_mod_mode, "mod mode", modModes, 0));
        lg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getLayerPID(ln) + ID::fun_cv_mode, "CV mode", cvModes, 0));


        for (unsigned n = 0; n < MAX_STEPS; n++) {
            lg->addChild(std::make_unique<ssp::BaseFloatParameter>(getLayerStepPID(ln, n) + ID::cv, "CV", -5.0f, 5.0f, 0.0f, 0.0f));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerStepPID(ln, n) + ID::access, "access", true));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerStepPID(ln, n) + ID::gate, "gate", false));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerStepPID(ln, n) + ID::glide, "glide", false));
        }
        lgs->addChild(std::move(lg));
    }
    params.add(std::move(lgs));

    return params;
}


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "X CLK",
        "X MOD",
        "X CV",
        "Y CLK",
        "Y MOD",
        "Y CV"
//        "Z MOD",
//        "Z CV"
    };
    if (channelIndex < I_MAX) { return inBusName[channelIndex]; }
    return "ZZIn-" + String(channelIndex);
}


const String PluginProcessor::getOutputBusName(int channelIndex) {
    static String outBusName[O_MAX] = {
        "X CV",
        "X GATE",
        "Y CV",
        "Y GATE",
        "C CV",
        "C GATE"
    };
    if (channelIndex < O_MAX) { return outBusName[channelIndex]; }
    return "ZZOut-" + String(channelIndex);
}

AudioProcessorEditor *PluginProcessor::createEditor() {
    return new ssp::EditorHost(this, new PluginEditor(*this));
}

AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}


float PluginProcessor::quantizeCv(unsigned scale, unsigned root, float voctIn) {
    constexpr float halfSemi = 0.5;
    constexpr bool roundUp = true;
    // cv2pitch, returns fractional semitones e.g 24.0 = C2
    float voct = cv2Pitch(voctIn) + 60.f + (roundUp ? halfSemi : 0.0f); // -5v = 0

    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    quantizer_.quantize(root, scale, oct, note);
    float pv = float(oct * 12.0f) + float(note) - 60.0f;
    float o_cv = pitch2Cv(pv);
    return o_cv;
}


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    static constexpr unsigned O_L_OFFSET = O_Y_CV - O_X_CV;
    static constexpr unsigned I_L_OFFSET = I_Y_CLK - I_X_CLK;

    prepLayer(*params_.layers_[PluginParams::X], layerData_[PluginParams::X]);
    prepLayer(*params_.layers_[PluginParams::Y], layerData_[PluginParams::Y]);
    prepCartLayer(*params_.layers_[PluginParams::C], layerData_[PluginParams::C]);

    for (int smp = 0; smp < sz; smp++) {
        for (int layer = 0; layer < PluginParams::MAX_LAYER; layer++) {
//            bool enabled = outputEnabled[(layer * O_L_OFFSET) + O_X_CV] || outputEnabled[(layer * O_L_OFFSET) + O_X_GATE];

            auto &ld = layerData_[layer];
            auto &layerParam = *params_.layers_[layer];
            float o_cv = 0.0f;
            bool o_gate = false;

            if (layer < PluginParams::C) {
                float clkIn = buffer.getSample((layer * I_L_OFFSET) + I_X_CLK, smp);
                float modIn = buffer.getSample((layer * I_L_OFFSET) + I_X_MOD, smp);
                float cvIn = buffer.getSample((layer * I_L_OFFSET) + I_X_CV, smp);

                processLayer(clkIn, modIn, cvIn, layerParam.steps_, ld, o_cv, o_gate);

            } else {
                auto &xld = layerData_[PluginParams::X];
                auto &yld = layerData_[PluginParams::Y];
                processCartLayer(layerParam.steps_, ld, xld, yld, o_cv, o_gate);
            }

            buffer.setSample((layer * O_L_OFFSET) + O_X_CV, smp, o_cv);
            buffer.setSample((layer * O_L_OFFSET) + O_X_GATE, smp, o_gate);
            if (ld.gateTime_ > 0) ld.gateTime_--;
        }
    }
}


void PluginProcessor::getActiveData(unsigned &xp, unsigned &yp, unsigned &cp,
                                    float &xCv, float &yCv, float &cCv,
                                    bool &xGate, bool &yGate, bool &cGate) const {
    xp = layerData_[PluginParams::X].pos_;
    xGate = layerData_[PluginParams::X].gate_;
    xCv = layerData_[PluginParams::X].cv_;

    yp = layerData_[PluginParams::Y].pos_;
    yCv = layerData_[PluginParams::Y].cv_;
    yGate = layerData_[PluginParams::Y].gate_;

    cp = layerData_[PluginParams::C].pos_;
    cCv = layerData_[PluginParams::C].cv_;
    cGate = layerData_[PluginParams::C].gate_;
}
