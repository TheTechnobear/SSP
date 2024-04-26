#pragma once

#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"

class SeqStepControl : public ssp::BarParamControl {
public:
    SeqStepControl(Parameter &p,  Parameter& scale);
    String getTextValue() override;
    void inc(bool fine) override;
    void dec(bool fine) override;

private:
    Parameter& scale_;
};