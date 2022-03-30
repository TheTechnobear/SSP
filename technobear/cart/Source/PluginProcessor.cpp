#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/EditorHost.h"
#include "Scales.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())


//TODO: parameter?
static constexpr unsigned gateTime = 64; // samples
static constexpr float clockLevel = 0.2f; // 1v


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
    modModes.add("Reset");
    modModes.add("Clk");
    modModes.add("RunStop");
    modModes.add("Dir");

    StringArray cvModes;
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
            lg->addChild(std::make_unique<ssp::BaseFloatParameter>(getLayerStepPID(ln, n) + ID::cv, "CV", -5.0f, 5.0f, 0.0f));
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

inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    unsigned sz = buffer.getNumSamples();

    bool xEnabled = outputEnabled[O_X_CV] || outputEnabled[O_X_GATE];
    bool yEnabled = outputEnabled[O_Y_CV] || outputEnabled[O_Y_GATE];
    bool cEnabled = outputEnabled[O_C_CV] || outputEnabled[O_C_GATE];

    for (int smp = 0; smp < sz; smp++) {
        bool xTrig = false, yTrig = false;


        //TODO... looks like most of this can be done as per layers as similar logic
        //though, likely C will need to override many things
        float xClk = buffer.getSample(I_X_CLK, smp);
        float yClk = buffer.getSample(I_Y_CLK, smp);

        xTrig = xClk > clockLevel && (x_.lastClk_ <= clockLevel);
        x_.lastClk_ = xClk;
        yTrig = yClk > clockLevel && (y_.lastClk_ <= clockLevel);
        y_.lastClk_ = yClk;

        bool trigX = params_.layers_[PluginParams::X]->fun_op_trig.getValue() > 0.5f;
        bool sleepX = params_.layers_[PluginParams::X]->fun_op_sleep.getValue() > 0.5f;
        unsigned modModeX = normValue(params_.layers_[PluginParams::X]->fun_mod_mode);
        unsigned cvModeX = normValue(params_.layers_[PluginParams::X]->fun_cv_mode);
        unsigned scaleX = normValue(params_.layers_[PluginParams::X]->scale);
        unsigned rootX = normValue(params_.layers_[PluginParams::X]->root);

        bool trigY = params_.layers_[PluginParams::Y]->fun_op_trig.getValue() > 0.5f;
        bool sleepY = params_.layers_[PluginParams::Y]->fun_op_sleep.getValue() > 0.5f;
        unsigned modModeY = normValue(params_.layers_[PluginParams::Y]->fun_mod_mode);
        unsigned cvModeY = normValue(params_.layers_[PluginParams::Y]->fun_cv_mode);
        unsigned scaleY = normValue(params_.layers_[PluginParams::Y]->scale);
        unsigned rootY = normValue(params_.layers_[PluginParams::Y]->root);

        bool trigC = params_.layers_[PluginParams::C]->fun_op_trig.getValue() > 0.5f;
        bool sleepC = params_.layers_[PluginParams::C]->fun_op_sleep.getValue() > 0.5f;
        unsigned modModeC = normValue(params_.layers_[PluginParams::C]->fun_mod_mode);
        unsigned cvModeC = normValue(params_.layers_[PluginParams::C]->fun_cv_mode);
        unsigned scaleC = normValue(params_.layers_[PluginParams::C]->scale);
        unsigned rootC = normValue(params_.layers_[PluginParams::C]->root);

        if (xTrig) {
            advanceLayer(x_, *params_.layers_[PluginParams::X], sleepX);
        }
        if (yTrig) {
            advanceLayer(y_, *params_.layers_[PluginParams::Y], sleepY);
        }
        if (xTrig || yTrig) {
            advanceCartLayer(c_, *params_.layers_[PluginParams::C], xTrig, yTrig);
        }

        float xMod = buffer.getSample(I_X_MOD, smp);
        float yMod = buffer.getSample(I_Y_MOD, smp);
        float xCV = buffer.getSample(I_X_CV, smp);
        float yCV = buffer.getSample(I_Y_CV, smp);



        //TODO : glide, use lastCV , and glide towards it

        //TODO : scale,root  - quantise or not (scale=0)

        //TODO : mod_reset, reset layer to start (0)
        //TODO : mod_clk, another clk trig
        //TODO : mod_runstop, ignore clock?
        //TODO : mod_dir, run forwards or backward (hmm, next pattern !?)

        //TODO : cv_add , add CV value
        //TODO : cv_loc, add to layers position value
        //TODO : cv_snake, add to snake value
        //TODO : cv_sh, same as add, but only on mod trig


        buffer.setSample(O_X_CV, smp, x_.cv_);
        buffer.setSample(O_X_GATE, smp, trigX ? x_.gateTime_ > 0 : x_.gate_ && (xClk > clockLevel));
        buffer.setSample(O_Y_CV, smp, y_.cv_);
        buffer.setSample(O_Y_GATE, smp, trigY ? y_.gateTime_ > 0 : y_.gate_ && (yClk > clockLevel));
        buffer.setSample(O_C_CV, smp, c_.cv_);
        buffer.setSample(O_C_GATE, smp, trigC ? c_.gateTime_ > 0 : c_.gate_);

        if (x_.gateTime_ > 0) x_.gateTime_--;
        if (y_.gateTime_ > 0) y_.gateTime_--;
        if (c_.gateTime_ > 0) c_.gateTime_--;
    }
}

void PluginProcessor::advanceLayer(LayerData &ld, Layer &params, bool sleep) {
    unsigned pos = findNextStep(ld.pos_, params, sleep);
    ld.pos_ = pos;
    jassert(ld.pos_ < MAX_STEPS);

    ld.gate_ = params.steps_[ld.pos_]->gate.getValue() > 0.5f;
    ld.glide_ = params.steps_[ld.pos_]->glide.getValue() > 0.5f;
    ld.gateTime_ = (ld.gate_ ? gateTime : 0);
    if (sleep || params.steps_[pos]->access.getValue() > 0.0f) {
        ld.cv_ = (params.steps_[ld.pos_]->cv.getValue() * 2.0f) - 1.0f;
        if (!ld.glide_) {
            ld.lastCV_ = ld.cv_;
        }
    }
}

void PluginProcessor::advanceCartLayer(LayerData &ld, Layer &params, bool xTrig, bool yTrig) {
    unsigned cPos = ld.pos_;
    int xC = cPos % 4;
    int yC = cPos / 4;

    unsigned pos = cPos;
    unsigned tryCount = 4;
    while (tryCount > 0) {
        xC = (xC + (xTrig ? 1 : 0)) % 4;
        yC = (yC + (yTrig ? 1 : 0)) % 4;
        pos = (yC * 4) + xC;
        if (params.steps_[pos]->access.getValue() > 0.0f) {
            tryCount = 0;
            break;
        }
        tryCount--;
    }

    jassert(pos < MAX_STEPS);

    if (params.steps_[pos]->access.getValue() > 0.0f) {
        ld.pos_ = pos;
        ld.cv_ = (params.steps_[ld.pos_]->cv.getValue() * 2.0f) - 1.0f;
        ld.gate_ = params.steps_[ld.pos_]->gate.getValue() > 0.5f;
        ld.gateTime_ = (ld.gate_ ? gateTime : 0);
    }
    // else, not allow at this position, so stay put.
}


unsigned PluginProcessor::findNextStep(unsigned cpos, Layer &params, bool sleep) {
    unsigned snake = GET_P_VAL(params.snake);
    jassert(snakes_.size() > 0);
    if (snake >= snakes_.size()) snake = 0;

    unsigned pos = cpos;

    unsigned tryCount = 16;
    while (tryCount > 0) {
        pos = snakes_.findNext(snake, pos);
//        jassert(pos < MAX_STEPS);

        bool access = params.steps_[pos]->access.getValue() > 0.5f;
        if (sleep || access) {
            return pos;
        }
        tryCount--;
    }
    return cpos;
}


void PluginProcessor::getActiveData(unsigned &xp, unsigned &yp, unsigned &cp,
                                    float &xCv, float &yCv, float &cCv,
                                    bool &xGate, bool &yGate, bool &cGate) const {
    xp = x_.pos_;
    yp = y_.pos_;
    cp = c_.pos_;
    xCv = x_.cv_;
    yCv = y_.cv_;
    cCv = c_.cv_;
    xGate = x_.gate_;
    yGate = y_.gate_;
    cGate = c_.gate_;
}


Percussa::SSP::PluginDescriptor *PluginProcessor::createDescriptor() {
    auto desc = new Percussa::SSP::PluginDescriptor;

    desc->name = JucePlugin_Name;
    desc->descriptiveName = JucePlugin_Desc;
    desc->manufacturerName = JucePlugin_Manufacturer;
    desc->version = JucePlugin_VersionString;
    desc->uid = (int) JucePlugin_VSTUniqueID;
    for (int i = 0; i < I_MAX; i++) {
        desc->inputChannelNames.push_back(getInputBusName(i).toStdString());
    }
    for (int i = 0; i < O_MAX; i++) {
        desc->outputChannelNames.push_back(getOutputBusName(i).toStdString());
    }
    return desc;
}