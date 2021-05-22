#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())

PluginProcessor::PluginProcessor()
    : PluginProcessor(getBusesProperties(), createParameterLayout()) {}

PluginProcessor::PluginProcessor(
    const AudioProcessor::BusesProperties &ioLayouts,
    AudioProcessorValueTreeState::ParameterLayout layout)
    : BaseProcessor(ioLayouts, std::move(layout)), params_(vts()) {
    init();
    initSnakes();
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
    gate(*apvt.getParameter(getLayerStepPID(ln, sn) + ID::gate)) {
    ;
}


PluginProcessor::Layer::Layer(AudioProcessorValueTreeState &apvt, unsigned ln) :
    glide(*apvt.getParameter(getLayerPID(ln) + ID::glide)),
    snake(*apvt.getParameter(getLayerPID(ln) + ID::snake)),
    fun(*apvt.getParameter(getLayerPID(ln) + ID::fun)),
    quant(*apvt.getParameter(getLayerPID(ln) + ID::quant)) {
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

    auto lgs = std::make_unique<AudioProcessorParameterGroup>(ID::layer, "Layers", ID::separator);
    for (unsigned ln = 0; ln < 3; ln++) {
        auto lg = std::make_unique<AudioProcessorParameterGroup>(layerID(ln), layerID(ln), ID::separator);
        lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerPID(ln) + ID::glide, "glide", false));
        lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerPID(ln) + ID::fun, "fun", false));
        lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerPID(ln) + ID::quant, "quant", true));


        for (unsigned n = 0; n < 16; n++) {
            lg->addChild(std::make_unique<ssp::BaseFloatParameter>(getLayerStepPID(ln, n) + ID::cv, "CV", -5.0f, 5.0f, 0.0f));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerStepPID(ln, n) + ID::access, "access", true));
            lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerStepPID(ln, n) + ID::gate, "gate", false));
        }
        lgs->addChild(std::move(lg));
    }
    params.add(std::move(lgs));

    return params;
}

enum {
    I_X_CLK,
    I_X_MOD,
    I_X_CV,
    I_Y_CLK,
    I_Y_MOD,
    I_Y_CV,
    I_Z_MOD,
    I_Z_CV,
    I_MAX
};
enum {
    O_X_CV,
    O_X_GATE,
    O_Y_CV,
    O_Y_GATE,
    O_C_CV,
    O_C_GATE,
    O_MAX
};


const String PluginProcessor::getInputBusName(int channelIndex) {
    static String inBusName[I_MAX] = {
        "X CLK",
        "X MOD",
        "X CV",
        "Y CLK",
        "Y MOD",
        "Y CV",
        "Z MOD",
        "Z CV"
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


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    bool xEnabled = outputEnabled[O_X_CV] || outputEnabled[O_X_GATE];
    bool yEnabled = outputEnabled[O_Y_CV] || outputEnabled[O_Y_GATE];
    bool cEnabled = outputEnabled[O_C_CV] || outputEnabled[O_C_GATE];

    for (int smp = 0; smp < sz; smp++) {
        bool xTrig = false, yTrig = false;
        bool xClk = buffer.getSample(I_X_CLK, smp);
        bool yClk = buffer.getSample(I_Y_CLK, smp);

        xTrig = xClk && x_.lastClk_ != xClk;
        x_.lastClk_ = xClk;
        yTrig = yClk && y_.lastClk_ != yClk;
        y_.lastClk_ = yClk;

        if (xTrig) {
            advanceLayer(x_, *params_.layers_[PluginParams::X]);
        }
        if (yTrig) {
            advanceLayer(y_, *params_.layers_[PluginParams::Y]);
        }
        if (xTrig || yTrig) {
            advanceCartLayer(c_, *params_.layers_[PluginParams::C], x_.pos_, y_.pos_);
        }

        //TODO : glide
        buffer.setSample(O_X_CV, smp, x_.cv_);
        buffer.setSample(O_X_GATE, smp, x_.gateTime_ > 0);
        buffer.setSample(O_Y_CV, smp, y_.cv_);
        buffer.setSample(O_Y_GATE, smp, y_.gateTime_ > 0);
        buffer.setSample(O_C_CV, smp, c_.cv_);
        buffer.setSample(O_C_GATE, smp, c_.gateTime_ > 0);

        if (x_.gateTime_ > 0) x_.gateTime_--;
        if (y_.gateTime_ > 0) y_.gateTime_--;
        if (c_.gateTime_ > 0) c_.gateTime_--;
    }
}

// TODO:  implement layer functionality
//layer :: glide
//layer :: snake
//layer :: fun //?0
//layer :: quant

// TODO:  implement step
// step cv - glide, quant
// step access - next pos
// step gate - quant?

//TODO: parameter?
static constexpr unsigned gateTime = 64; // samples


void PluginProcessor::advanceLayer(LayerData &ld, Layer &params) {

    // todo x/y/z mod and cv?
    unsigned npos = findNextStep(ld.pos_, params);
    ld.pos_ = npos;
    ld.cv_ = params.steps_[ld.pos_]->cv.getValue();
    ld.gateTime_ = (params.steps_[ld.pos_]->gate.getValue() > 0.5f ? gateTime : 0);
}

void PluginProcessor::advanceCartLayer(LayerData &ld, Layer &params, unsigned xPos, unsigned yPos) {
    // cart layer does not have its own pos, instead it is inferred x/y
    // this means we can ignore snake/access etc
    unsigned x = xPos % 4;
    unsigned y = yPos / 4;
    ld.pos_ = (y * 4) + x;

    ld.cv_ = params.steps_[ld.pos_]->cv.getValue();
    ld.gateTime_ = (params.steps_[ld.pos_]->gate.getValue() > 0.5f ? gateTime : 0);
}


unsigned PluginProcessor::findNextStep(unsigned cpos, Layer &params) {
    unsigned snake = GET_P_VAL(params.snake);
    jassert(!snakes_.empty());
    if (snake >= snakes_.size()) snake = 0;
    auto &algo = *snakes_[snake];

    unsigned pos = cpos;
    bool found = false;

    //TODO is this the best approach?
    //another possibility is to send in a 'mask' to snake algo
    unsigned tryCount = 16;

    while (!found && tryCount > 0) {
        pos = algo.findNext(pos);

        bool access = params.steps_[pos]->access.getValue() > 0.5f;
        found = access;
        if (!found) {
            tryCount--;
        }
    }

    if (tryCount == 0) pos = cpos;

    return pos;
}

class LinearAlgo : public PluginProcessor::SnakeAlgo {
public:
    explicit LinearAlgo() { ; }

    String name() { return "Linear"; }

    unsigned findNext(unsigned cpos) {
        return (cpos + 1) % 16;
    }
};


void PluginProcessor::initSnakes() {
    snakes_.push_back(std::make_unique<LinearAlgo>());
}

