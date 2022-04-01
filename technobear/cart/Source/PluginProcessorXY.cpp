#include "PluginProcessor.h"


void PluginProcessor::prepLayer(Layer &layerParam, LayerData &ld) {
    ld.p_.snake_ = normValue(layerParam.snake);
    ld.p_.fun_op_trig_ = layerParam.fun_op_trig.getValue() > 0.5f;
    ld.p_.fun_op_sleep_ = layerParam.fun_op_sleep.getValue() > 0.5f;
    ld.p_.fun_mod_mode_ = ModMode(normValue(layerParam.fun_mod_mode));
    ld.p_.fun_cv_mode_ = CvMode(normValue(layerParam.fun_cv_mode));

    ld.p_.scale_ = normValue(layerParam.scale);
    ld.p_.root_ = normValue(layerParam.root);

    // reset data that can change
    ld.reset_ = false;
    ld.run_ = true;
    ld.reverse_ = false;
    ld.snakeOffset_ = 0;
    ld.seqOffset_ = 0;
    ld.cvOffset_ = 0.0f;
}


unsigned PluginProcessor::advanceLayer(Steps &steps, unsigned seqstep, LayerData &ld) {
    if (!ld.reverse_) {
        seqstep = (seqstep + 1) % MAX_STEPS;
    } else {
        seqstep = (seqstep + MAX_STEPS - 1) % MAX_STEPS;
    }
    return setLayerStep(steps, seqstep, ld);
}

unsigned PluginProcessor::setLayerStep(Steps &steps, unsigned seqstep, LayerData &ld) {
    // seqstep is where we 'want' to go, but access may stop us !

    unsigned step = seqstep;

    if (!ld.p_.fun_op_sleep_) {
        // sleep
        unsigned snake = (ld.p_.snake_ + ld.snakeOffset_) % snakes_.size();
        for (unsigned i = 0; i < MAX_STEPS; i++) {
            unsigned pos = snakes_.getPosition(snake, (ld.seqOffset_ + step) % MAX_STEPS);
            if (steps[pos]->access.getValue() > 0.5f) {
                return step;
            }
            if (!ld.reverse_) {
                step = (step + 1) % MAX_STEPS;
            } else {
                step = (step + MAX_STEPS - 1) % MAX_STEPS;
            }
        }
        // no access, so return current sequencer step
        return ld.seqStep_;
    }  // else, sleep enables us to go to any step
    return seqstep;
}


void PluginProcessor::processLayer(float clkIn, float modIn, float cvIn, Steps &steps, LayerData &ld, float &o_cv, bool &o_gate) {
    ld.clkTrig_ = clkIn > clockLevel && (ld.lastClkIn_ <= clockLevel);

    if (ld.clkTrig_) {
        ld.lastClkSmp_ = ld.lastClkSmpCnt_;
        ld.lastClkSmpCnt_ = 0;
    } else {
        ld.lastClkSmpCnt_++;
    }

    ld.reset_ = ld.p_.fun_mod_mode_ == MOD_MODE_RESET && (modIn >= trigLevel && ld.lastModIn_ < trigLevel);
    ld.reverse_ = ld.p_.fun_mod_mode_ == MOD_MODE_DIR && modIn < trigLevel;
    ld.run_ = ld.p_.fun_mod_mode_ != MOD_MODE_RUNSTP || modIn > trigLevel;
    bool modClkTrig = ld.p_.fun_mod_mode_ == MOD_MODE_CLK
                      && (modIn >= trigLevel && ld.lastModIn_ < trigLevel)
                      && clkIn < clockLevel;


    if (ld.p_.fun_cv_mode_ == CV_MODE_SNAKE) {
        unsigned sz = snakes_.size();
        ld.snakeOffset_ = (int(cvIn * sz) + sz) % sz;
    } else if (ld.p_.fun_cv_mode_ == CV_MODE_LOC) {
        ld.seqOffset_ = (int(cvIn * MAX_STEPS) + MAX_STEPS) % MAX_STEPS;
    }

    ld.clkTrig_ = ld.clkTrig_ || modClkTrig;

    unsigned nextStep = ld.seqStep_;

    if (ld.reset_) {
        nextStep = setLayerStep(steps, 0, ld);
    } else if (ld.run_ && ld.clkTrig_) {
        nextStep = advanceLayer(steps, ld.seqStep_, ld);
    }

    bool stepChg = ld.seqStep_ != nextStep;
    ld.seqStep_ = nextStep;

    ld.pos_ = snakes_.getPosition(
        (ld.p_.snake_ + ld.snakeOffset_) % snakes_.size(),
        (ld.seqStep_ + ld.seqOffset_) % MAX_STEPS
    );

    auto &activeStep = *steps[ld.pos_];

    bool glide = activeStep.glide.getValue() > 0.5f;
    bool access = activeStep.access.getValue() > 0.5f;
    bool gate = activeStep.gate.getValue() > 0.5f;

    if (access) {
        ld.targetCv_ = (activeStep.cv.getValue() * 2.0f) - 1.0f;
    }

    if (ld.p_.fun_cv_mode_ == CV_MODE_ADD) {
        ld.cvOffset_ = cvIn;
    } else if (ld.p_.fun_cv_mode_ == CV_MODE_SH) {
        if (modIn >= trigLevel && (modIn >= trigLevel && ld.lastModIn_ < trigLevel)) {
            ld.cvOffset_ = cvIn;
        }
    }

    if (ld.p_.scale_ > 0.0f) ld.targetCv_ = quantizeCv(ld.p_.scale_, ld.p_.root_, ld.cvOffset_ + ld.targetCv_);
    else ld.targetCv_ += ld.cvOffset_;

    if (glide) {
        o_cv = ld.cv_ + ((ld.targetCv_ - ld.cv_) * (glideRatio / float(ld.lastClkSmp_)));
    } else {
        o_cv = ld.targetCv_;
    }

    if (gate) {
        if (ld.clkTrig_) {
            if (modClkTrig || ld.p_.fun_op_trig_) {
                ld.gateTime_ = trigGateTime;
            }
        }

        if (modClkTrig && clkIn < clockLevel) {
            o_gate = ld.gateTime_ > 0;
        } else {
            o_gate = ld.p_.fun_op_trig_ ? ld.gateTime_ > 0 : clkIn >= clockLevel;
        }
    }

    ld.cv_ = o_cv;
    ld.gate_ = o_gate;
    if (ld.gateTime_ > 0) ld.gateTime_--;

    ld.lastClkIn_ = clkIn;
    ld.lastCvIn_ = cvIn;
    ld.lastModIn_ = modIn;
}
