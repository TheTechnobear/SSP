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


class ReneEnumSnake : public PluginProcessor::SnakeAlgo {
public:
    explicit ReneEnumSnake(const String &name, const unsigned *pattern) :
        name_(name), pattern_(pattern) {

        // nxtPattern - for a cpos find next
        for (int i = 0; i < 15; i++) {
            if (i == 0) {
                // 16 leads into position 1
                for (int x = 0; x < 16; x++) {
                    if (pattern_[x] == 15) {
                        nxtPattern_[i] = x;
                        break;
                    }
                }
            } else {
                for (int x = 0; x < 16; x++) {
                    if (pattern_[x] == i) {
                        nxtPattern_[i] = x;
                        break;
                    }
                }
            }
        }
    }

    const String &name() { return name_; }

    unsigned findNext(unsigned cpos) {
        return nxtPattern_[cpos];
    }

protected:
    String name_;
    const unsigned *pattern_;
    unsigned nxtPattern_[16];
};


class LinearAlgo : public PluginProcessor::SnakeAlgo {
public:
    explicit LinearAlgo() { ; }

    const String &name() { return name_; }

    unsigned findNext(unsigned cpos) {
        return (cpos + 1) % 16;
    }

    String name_ = "Linear";
};


void PluginProcessor::initSnakes() {
    static constexpr unsigned renepatterns[16][16] = {
        { // pattern 1
            0,  1,  2,  3,
            4,  5,  6,  7,
            8,  9,  10, 11,
            12, 14, 14, 15
        },

        { // pattern 2
            0,  1,  2,  3,
            7,  6,  5,  4,
            8,  9,  10, 11,
            15, 14, 13, 12
        },

        { // pattern 3
            3,  7,  11, 15,
            2,  6,  10, 14,
            1,  5,  9,  13,
            0,  4,  8,  12
        },

        { // pattern 4
            0,  4,  8,  12,
            12, 9,  5,  1,
            2,  6,  10, 14,
            15, 11, 7,  3

        },

        { // pattern 5
            0,  1,  2,  3,
            7,  11, 15, 14,
            13, 12, 8,  4,
            5,  6,  10, 9
        },
        { // pattern 6
            12, 13, 14, 15,
            11, 7,  3,  2,
            1,  0,  4,  8,
            9,  10, 6,  5
        },
        { // pattern 7
            0,  1,  4,  8,
            5,  2,  3,  6,
            9,  12, 13, 10,
            7,  11, 14, 14
        },
        { // pattern 8
            0,  5,  10, 15,
            14, 9,  4,  1,
            6,  11, 7,  2,
            8,  13, 12, 3
        },
        { // pattern 9
            0,  12, 13, 3,
            2,  14, 15, 1,
            7,  8,  9,  5,
            4,  10, 11, 6
        },
        { // pattern 10
            0,  7,  10, 14,
            4,  1,  8,  11,
            12, 5,  2,  9,
            15, 13, 6,  4
        },
        { // pattern 11
            0,  1,  8,  9,
            6,  7,  14, 15,
            4,  5,  12, 13,
            2,  3,  10, 11,
        },
        { // pattern 12
            0,  8,  1,  9,
            6,  14, 7,  15,
            4,  12, 5,  13,
            2,  10, 3,  11
        },
        { // pattern 13
            0,  1,  2,  3,
            4,  5,  6,  7,
            12, 14, 14, 15,
            8,  9,  10, 11
        },
        { // pattern 14
            0,  3,  11, 10,
            2,  5,  12, 12,
            4,  7,  15, 14,
            6,  1,  9,  8
        },
        { // pattern 15  - same as 14!
            0,  3,  11, 10,
            2,  5,  12, 12,
            4,  7,  15, 14,
            6,  1,  9,  8
        },

        { // pattern 16 - same as 14!
            0,  3,  11, 10,
            2,  5,  12, 12,
            4,  7,  15, 14,
            6,  1,  9,  8
        }

    };

//        snakes_.push_back(std::make_unique<LinearAlgo>());
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 1", renepatterns[0]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 2", renepatterns[1]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 3", renepatterns[2]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 4", renepatterns[3]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 5", renepatterns[4]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 6", renepatterns[5]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 7", renepatterns[6]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 8", renepatterns[7]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 9", renepatterns[8]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 10", renepatterns[9]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 11", renepatterns[10]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 12", renepatterns[11]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 13", renepatterns[12]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 14", renepatterns[13]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 15", renepatterns[14]));
    snakes_.push_back(std::make_unique<ReneEnumSnake>("Ptn 16", renepatterns[15]));
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