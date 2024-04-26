#include "SeqStepControl.h"

#include "UIStringFunc.h"

SeqStepControl::SeqStepControl(Parameter &p, Parameter& scale)
    : BarParamControl(p, 0.2f, 0.01f), scale_(scale) {
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())

String SeqStepControl::getTextValue() {
    bool useStepNotes = scale_.getValue() != 0.0f;

    if (useStepNotes) {
        // parameters are 0..1, v/oct cv is -1 to 1
        float v = (param_.getValue() * 2.0f) - 1.0f;
        String val = getNoteValue(v);
        return val;
    }

    float v = GET_P_VAL(param_);
    String val = String::formatted("%0.3f", v);
    return val;
}

void SeqStepControl::inc(bool fine) {
   bool  useStepNotes = scale_.getValue() != 0.0f;

    if (useStepNotes) {
        static constexpr float coarseInc = 1.0f / (10.0f * 12.0f);  // +/-5 oct
        static constexpr float fineInc = coarseInc / 100.0f;        // cents
        auto &p = param_;
        p.beginChangeGesture();
        float inc = fine ? fineInc : coarseInc;
        float v = p.getValue();
        float nv = v + (inc != 0 ? inc : 0.01f);
        nv = std::min(nv, 1.0f);
        p.setValueNotifyingHost(nv);
        p.endChangeGesture();
        return;
    }
    ssp::BarParamControl::inc(fine);
}

void SeqStepControl::dec(bool fine) {
   bool  useStepNotes = scale_.getValue() != 0.0f;

    if (useStepNotes) {
        static constexpr float coarseInc = 1.0f / (10.0f * 12.0f);  // +/-5 oct
        static constexpr float fineInc = coarseInc / 100.0f;        // cents
        auto &p = param_;
        p.beginChangeGesture();
        float inc = fine ? fineInc : coarseInc;
        float v = p.getValue();
        float nv = v - (inc != 0 ? inc : 0.01f);
        nv = std::max(nv, 0.0f);
        p.setValueNotifyingHost(nv);
        p.endChangeGesture();
        return;
    }
    ssp::BarParamControl::dec(fine);
}
