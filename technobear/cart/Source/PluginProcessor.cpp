#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/EditorHost.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())

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

    StringArray snakeTypes;
    for( unsigned s=0;s<snakes_.size();s++) {
        auto n=snakes_.name(s);
        snakeTypes.add(n);
    }

    auto lgs = std::make_unique<AudioProcessorParameterGroup>(ID::layer, "Layers", ID::separator);
    for (unsigned ln = 0; ln < 3; ln++) {
        auto lg = std::make_unique<AudioProcessorParameterGroup>(layerID(ln), layerID(ln), ID::separator);
        lg->addChild(std::make_unique<ssp::BaseBoolParameter>(getLayerPID(ln) + ID::glide, "glide", false));
        lg->addChild(std::make_unique<ssp::BaseChoiceParameter>(getLayerPID(ln) + ID::snake, "snake", snakeTypes,0));
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


void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {

    struct fun_op {
        struct _op {
            bool sleep_ = false; // on = stay  on pos, but dont fire gate
            bool trig_ = false;  // short trig
            bool scan_ = false; // scan knobs.. NA?
        } op_;
        struct _mod { // how to use mod
            bool reset_ = false; // reset to first location
            bool clk_ = false; //  combine clk and mod , thus allow retrig
            bool runstp_ = false; // run = high
            bool dir_ = false;  // high = fwd, low backwards
        } mod_;
        struct _cv { // how to use cv
            bool add_ = false; // add to cv
            bool loc_ = false; // add to internal location, presumably wrap?
            bool snake_ = false; // control which snake pattern
            bool s_and_h_ = false; // same as add, except only on rising edge of mod
        } cv_;
    } fun_[3];


    unsigned sz = buffer.getNumSamples();

    bool xEnabled = outputEnabled[O_X_CV] || outputEnabled[O_X_GATE];
    bool yEnabled = outputEnabled[O_Y_CV] || outputEnabled[O_Y_GATE];
    bool cEnabled = outputEnabled[O_C_CV] || outputEnabled[O_C_GATE];

    for (int smp = 0; smp < sz; smp++) {
        bool xTrig = false, yTrig = false;
        bool xClk = buffer.getSample(I_X_CLK, smp);
        bool yClk = buffer.getSample(I_Y_CLK, smp);

        xTrig = xClk && (x_.lastClk_ != xClk);
        x_.lastClk_ = xClk;
        yTrig = yClk && (y_.lastClk_ != yClk);
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
        buffer.setSample(O_X_GATE, smp, fun_[0].op_.trig_ ? x_.gateTime_ > 0 : x_.gate_ && xClk);
        buffer.setSample(O_Y_CV, smp, y_.cv_);
        buffer.setSample(O_Y_GATE, smp, y_.gateTime_ > 0);
        buffer.setSample(O_Y_GATE, smp, fun_[0].op_.trig_ ? y_.gateTime_ > 0 : y_.gate_ && yClk);
        buffer.setSample(O_C_CV, smp, c_.cv_);
        buffer.setSample(O_C_GATE, smp, fun_[0].op_.trig_ ? c_.gateTime_ > 0 : c_.gate_);

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
    ld.gate_ = params.steps_[ld.pos_]->gate.getValue() > 0.5f;
    ld.gateTime_ = (ld.gate_ ? gateTime : 0);
}

void PluginProcessor::advanceCartLayer(LayerData &ld, Layer &params, unsigned xPos, unsigned yPos) {
    // cart layer does not have its own pos, instead it is inferred x/y
    // this means we can ignore snake/access etc
    unsigned x = xPos % 4;
    unsigned y = yPos / 4;
    ld.pos_ = (y * 4) + x;
    ld.cv_ = params.steps_[ld.pos_]->cv.getValue();
    ld.gate_ = params.steps_[ld.pos_]->gate.getValue() > 0.5f;
    ld.gateTime_ = (ld.gate_ ? gateTime : 0);
}


unsigned PluginProcessor::findNextStep(unsigned cpos, Layer &params) {
    unsigned snake = GET_P_VAL(params.snake);
    jassert(snakes_.size()>0);
    if (snake >= snakes_.size()) snake = 0;

    unsigned pos = cpos;
    bool found = false;

    //TODO is this the best approach?
    //another possibility is to send in a 'mask' to snake algo
    unsigned tryCount = 16;

    while (!found && tryCount > 0) {
        pos = snakes_.findNext(snake,pos);

        bool access = params.steps_[pos]->access.getValue() > 0.5f;
        found = access;
        if (!found) {
            tryCount--;
        }
    }

    if (tryCount == 0) pos = cpos;

    return pos;
}

Percussa::SSP::PluginDescriptor* PluginProcessor::createDescriptor() {
    auto desc = new Percussa::SSP::PluginDescriptor;

    desc->name = JucePlugin_Name;
    desc->descriptiveName = JucePlugin_Desc;
    desc->manufacturerName = JucePlugin_Manufacturer;
    desc->version = JucePlugin_VersionString;
    desc->uid = (int)JucePlugin_VSTUniqueID;
    for(int i=0;i<I_MAX;i++) {
        desc->inputChannelNames.push_back(getInputBusName(i).toStdString());
    }
    for(int i=0;i<O_MAX;i++) {
        desc->outputChannelNames.push_back(getOutputBusName(i).toStdString());
    }
    return desc;
}