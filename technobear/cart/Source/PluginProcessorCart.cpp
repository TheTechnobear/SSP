#include "PluginProcessor.h"


unsigned PluginProcessor::setCartLayerX(Steps &steps, unsigned x, unsigned xOffset, LayerData &ld) {
    int xC = x % 4;
    int yC = ld.seqStep_ / 4;

    if (!ld.p_.fun_op_sleep_) {
        for (int i = 0; i < 4; i++) {
            unsigned pos = (yC * 4) + ((xC + xOffset) % 4);
            if (steps[pos]->access.getValue() > 0.5f) {
                return xC % 4;
            }
            xC++;
        }
        // no access, so return current seqstep
        return ld.seqStep_ % 4;
    }
    return x % 4;
}

unsigned PluginProcessor::setCartLayerY(Steps &steps, unsigned y, unsigned yOffset, LayerData &ld) {
    int xC = ld.seqStep_ % 4;
    int yC = y % 4;

    if (!ld.p_.fun_op_sleep_) {
        for (int i = 0; i < 4; i++) {
            unsigned pos = (((yC + yOffset) % 4) * 4) + xC;
            if (steps[pos]->access.getValue() > 0.5f) {
                return yC % 4;
            }
            yC++;
        }
        // no access, so return current seqstep
        return ld.seqStep_ / 4;
    }
    return y % 4;
}

void PluginProcessor::prepCartLayer(Layer &layerParam, LayerData &ld) {
    // this is separate, so I don't populate stuff I don't use ...
    ld.p_.fun_op_trig_ = layerParam.fun_op_trig.getValue() > 0.5f;
    ld.p_.fun_op_sleep_ = layerParam.fun_op_sleep.getValue() > 0.5f;

    ld.p_.scale_ = normValue(layerParam.scale);
    ld.p_.root_ = normValue(layerParam.root);
}

void PluginProcessor::processCartLayer(Steps &steps, LayerData &ld, LayerData &xld, LayerData &yld, float &o_cv, bool &o_gate) {
    // functions working for C
    // fun.sleep, fun.trig
    // gate, access, glide, quant (scale, root)

    // functions affected indirectly by x/y params - not its own
    // fun.mod.reset, fun.mod.clk, fun.mod.runstop
    // fun.cv.loc

    // functions that have no effect... either directly or indirectly
    // fun.mod.dir
    // fun.cv.add, fun.cv.sh, fun.snake

    // these are different from C... need to carefully read manual on these bits
    // fun_op_trig
    // fun_op sleep - unimplemented for now, as manual is unclear ;)

    unsigned xOffset = 0;
    unsigned yOffset = 0;
    static constexpr unsigned MAX_C_STEP = 4;
    if (xld.p_.fun_cv_mode_ == CV_MODE_LOC) {
        xOffset = (int(xld.lastCvIn_ * MAX_C_STEP) + MAX_C_STEP) % MAX_C_STEP;
    }
    if (yld.p_.fun_cv_mode_ == CV_MODE_LOC) {
        yOffset = (int(yld.lastCvIn_ * MAX_C_STEP) + MAX_C_STEP) % MAX_C_STEP;
    }

    unsigned nextStep = ld.seqStep_;
    unsigned xStep = nextStep % 4;
    unsigned yStep = nextStep / 4;

    if (xld.reset_) {
        xStep = setCartLayerX(steps, 0, xOffset, ld);
    } else if (xld.run_ && xld.clkTrig_) {
        xStep = setCartLayerX(steps, xStep + 1, xOffset, ld);
    }
    if (yld.reset_) {
        yStep = setCartLayerY(steps, 0, yOffset, ld);
    } else if (yld.run_ && yld.clkTrig_) {
        yStep = setCartLayerY(steps, yStep + 1, yOffset, ld);
    }
    nextStep = (yStep * 4) + xStep;

    bool stepChg = ld.seqStep_ != nextStep;
    ld.clkTrig_ = stepChg && (xld.clkTrig_ || yld.clkTrig_);
    ld.seqStep_ = nextStep;
    ld.pos_ = (((yStep + yOffset) % MAX_C_STEP) * 4) + ((xStep + xOffset) % 4);

    auto &activeStep = *steps[ld.pos_];
    bool glide = activeStep.glide.getValue() > 0.5f;
    bool access = activeStep.access.getValue() > 0.5f;

    if (access) {
        ld.targetCv_ = (activeStep.cv.getValue() * 2.0f) - 1.0f;
        if (ld.p_.scale_ > 0.0f) ld.targetCv_ = quantizeCv(ld.p_.scale_, ld.p_.root_, ld.targetCv_);
    }

    if (glide) {
        float glideTime = getSampleRate() / 2.0f; // 120bpm = 2 ticks/sec
        o_cv = ld.cv_ + ((ld.targetCv_ - ld.cv_) * (glideRatio / glideTime));
    } else {
        o_cv = ld.targetCv_;
    }

    if (ld.p_.fun_op_trig_) {
        if (xld.clkTrig_ || yld.clkTrig_) {
            ld.gateTime_ = trigGateTime;
        }
        o_gate = ld.gateTime_ > 0;
        if (ld.gateTime_ > 0) ld.gateTime_--;
    } else {
        o_gate = ld.gate_ && (xld.gate_ || yld.gate_);
        o_gate = o_gate || (activeStep.gate.getValue() > 0.5f);
    }

    ld.cv_ = o_cv;
    ld.gate_ = o_gate;
}



