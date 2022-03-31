#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/EditorHost.h"
#include "Scales.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())


static constexpr unsigned gateTime = 64; // samples //TODO: parameter?
static constexpr float trigLevel = 0.2f; // 1v
static constexpr float clockLevel = trigLevel; // 1v


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

inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}

void PluginProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) {
    unsigned sz = buffer.getNumSamples();

    static constexpr unsigned O_L_OFFSET = O_Y_CV - O_X_CV;
    static constexpr unsigned I_L_OFFSET = I_Y_CLK - I_X_CLK;

    for (int smp = 0; smp < sz; smp++) {
        for (int layer = 0; layer < PluginParams::MAX_LAYER; layer++) {
            bool enabled = outputEnabled[(layer * O_L_OFFSET) + O_X_CV] || outputEnabled[(layer * O_L_OFFSET) + O_X_GATE];

            auto &ld = layerData_[layer];
            auto &layerParam = params_.layers_[layer];

            bool op_trig = layerParam->fun_op_trig.getValue() > 0.5f;
            bool op_sleep = layerParam->fun_op_sleep.getValue() > 0.5f;
            unsigned modMode = normValue(layerParam->fun_mod_mode);
            unsigned cvMode = normValue(layerParam->fun_cv_mode);
            unsigned scale = normValue(layerParam->scale);
            unsigned root = normValue(layerParam->root);

            bool o_gate = false;
            float o_cv = 0.0f;


            if (layer < PluginParams::C) {
                float clkIn = buffer.getSample((layer * I_L_OFFSET) + I_X_CLK, smp);
                float modIn = buffer.getSample((layer * I_L_OFFSET) + I_X_MOD, smp);
                float cvIn = buffer.getSample((layer * I_L_OFFSET) + I_X_CV, smp);

                ld.clkTrig_ = clkIn > clockLevel && (ld.lastClkIn_ <= clockLevel);

                if (ld.clkTrig_) {
                    ld.lastClkSmp_ = ld.lastClkSmpCnt_;
                    ld.lastClkSmpCnt_ = 0;
                } else {
                    ld.lastClkSmpCnt_++;
                }

                ld.lastClkIn_ = clkIn;

                ld.reset_ = modMode == MOD_MODE_RESET && (modIn >= trigLevel && ld.lastModIn_ < trigLevel);
                ld.reverse_ = modMode == MOD_MODE_DIR && modIn < trigLevel;
                ld.run_ = modMode != MOD_MODE_RUNSTP || modIn > trigLevel;
                bool modClkTrig = modMode == MOD_MODE_CLK
                                  && (modIn >= trigLevel && ld.lastModIn_ < trigLevel)
                                  && clkIn < clockLevel;

                ld.clkTrig_ = ld.clkTrig_ || modClkTrig;

                if (ld.reset_) {
                    setLayerStep(0, ld, *layerParam, op_sleep);
                    if (ld.glide_) ld.glideTime_ = ld.lastClkSmp_;
                } else if (ld.run_ && ld.clkTrig_) {
                    advanceLayer(ld, *layerParam, op_sleep, ld.reverse_);
                    if (ld.glide_) ld.glideTime_ = ld.lastClkSmp_;
                }

                //TODO: posOffset, I assume should be live value, i.e. not just changing
                //when clkTrigs, so sounds like CV should be 'constantly' look up, not just on advance step
                if (cvMode == CV_MODE_LOC) {
                    ld.posOffset_ = cvIn * MAX_STEPS;
                }

                if (modClkTrig && clkIn < clockLevel) {
                    o_gate = ld.gateTime_ > 0;
                } else {
                    o_gate = op_trig ? ld.gateTime_ > 0 : ld.gate_ && (clkIn >= clockLevel);
                }
                o_cv = ld.cv_;

                if (cvMode == CV_MODE_ADD) {
                    ld.cvOffset_ = cvIn;
                } else if (cvMode == CV_MODE_SH) {
                    if (modIn >= trigLevel && (modIn >= trigLevel && ld.lastModIn_ < trigLevel)) {
                        ld.cvOffset_ = cvIn;
                    }
                } else {
                    ld.cvOffset_ = 0.0f;
                }
                o_cv += ld.cvOffset_;


                // TODO : glide time : for X/Y is based on clock rate?
                // so count samples?

                ld.lastCvIn_ = cvIn;
                ld.lastModIn_ = modIn;
            } else {
                // these are different from C... need to carefully read manual on these bits
                // TODO : op_trig behaviour for C
                // TODO : op_sleep behaviour for C
                auto &xld = layerData_[PluginParams::X];
                auto &yld = layerData_[PluginParams::Y];
                bool xTrig = xld.clkTrig_ && xld.run_;
                bool yTrig = yld.clkTrig_ && yld.run_;

                unsigned xPosOffset = xld.posOffset_;
                unsigned yPosOffset = yld.posOffset_;

                // TODO : glide time : for C is a constant rate.
                if (xld.reset_) {
                    setCartLayerX(ld, *layerParam, 0);
                    if (ld.glide_) ld.glideTime_ = getSampleRate() / 100; // 10ms
                    xTrig = false;
                }
                if (yld.reset_) {
                    setCartLayerY(ld, *layerParam, 0);
                    if (ld.glide_) ld.glideTime_ = getSampleRate() / 100; // 10ms
                    yTrig = false;
                }

                ld.clkTrig_ = xTrig || yTrig; // not used ;
                if (ld.clkTrig_) {
                    //TODO consider x/y pos offset
                    advanceCartLayer(ld, *layerParam, xTrig, yTrig);
                    if (ld.glide_)  {
                        ld.glideTime_ = getSampleRate() / 100; // 10ms
                    }
                }

                o_gate = op_trig ? ld.gateTime_ > 0 : ld.gate_;
                o_cv = ld.cv_;
            }

            // glide idea
            // when we move to step.... we know the new target cv value
            // we look up the clock, and calculate glideAmt (or incCV amount)  target - current
            // then each sample, we add incCV, until we hit target (or glideClk = 0?)

            if (scale > 0) {
                //TODO : presumably glide is AFTER quantization !
                constexpr float halfSemi = 0.5;
                constexpr bool roundUp = true;
                // cv2pitch, returns fractional semitones e.g 24.0 = C2
                float voct = cv2Pitch(o_cv) + 60.f + (roundUp ? halfSemi : 0.0f); // -5v = 0

                int oct = voct / 12;
                unsigned note = unsigned(voct) % MAX_TONICS;
                // Logger::writeToLog("float " + String(v) + " voct " + String(voct) + " oct " + String(oct) + " note " + String(note));

                quantizer_.quantize(root, scale, oct, note);

                float pv = float(oct * 12.0f) + float(note) - 60.0f;
                float o_cv = pitch2Cv(pv);
            }


            //TODO : glide, use lastCV , and glide towards it - time?
            buffer.setSample((layer * O_L_OFFSET) + O_X_CV, smp, o_cv);

            buffer.setSample((layer * O_L_OFFSET) + O_X_GATE, smp, o_gate);
            if (ld.gateTime_ > 0) ld.gateTime_--;
        }
    }
}

void PluginProcessor::setLayerStep(unsigned pos, LayerData &ld, Layer &params, bool sleep) {
    //TODO: this needs to skip to find ACCESS = ON (for reset)
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


void PluginProcessor::advanceLayer(LayerData &ld, Layer &params, bool sleep, bool rev) {
    unsigned pos = findNextStep(ld.pos_, params, sleep, rev);
    setLayerStep(pos, ld, params, sleep);
}


void PluginProcessor::setCartLayerX(LayerData &ld, Layer &params, unsigned pos) {
    unsigned cPos = ld.pos_;
    int xC = 0;
    int yC = cPos / 4;
    //TODO: this needs to skip to find ACCESS = ON (for reset)
    if (params.steps_[pos]->access.getValue() > 0.0f) {
        ld.pos_ = pos;
        ld.cv_ = (params.steps_[ld.pos_]->cv.getValue() * 2.0f) - 1.0f;
        ld.gate_ = params.steps_[ld.pos_]->gate.getValue() > 0.5f;
        ld.gateTime_ = (ld.gate_ ? gateTime : 0);
    }
}

void PluginProcessor::setCartLayerY(LayerData &ld, Layer &params, unsigned pos) {
    unsigned cPos = ld.pos_;
    int xC = cPos % 4;
    int yC = 0;

    //TODO: this needs to skip to find ACCESS = ON (for reset)
    if (params.steps_[pos]->access.getValue() > 0.0f) {
        ld.pos_ = pos;
        ld.cv_ = (params.steps_[ld.pos_]->cv.getValue() * 2.0f) - 1.0f;
        ld.gate_ = params.steps_[ld.pos_]->gate.getValue() > 0.5f;
        ld.gateTime_ = (ld.gate_ ? gateTime : 0);
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


unsigned PluginProcessor::findNextStep(unsigned cpos, Layer &params, bool sleep, bool reverse) {
    unsigned snake = GET_P_VAL(params.snake);
    jassert(snakes_.size() > 0);
    if (snake >= snakes_.size()) snake = 0;

    unsigned pos = cpos;

    unsigned tryCount = 16;
    while (tryCount > 0) {
        if (!reverse) {
            pos = snakes_.findNext(snake, pos);
        } else {
            pos = snakes_.findPrev(snake, pos);
        }

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