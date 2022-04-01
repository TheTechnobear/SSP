
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"


class SeqStepControl : public ssp::BarParamControl {
public:
    SeqStepControl(PluginEditor& editor, unsigned layer, Parameter &p);
    String getTextValue() override;
    void inc(bool fine) override;
    void dec(bool fine) override;
private:
    PluginEditor& editor_;
    unsigned layer_;
};

using pcontrol_type = ssp::BarParamControl;
using scontrol_type = SeqStepControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;
static constexpr unsigned MAX_STEPS = PluginProcessor::MAX_STEPS;
static constexpr unsigned MAX_VIEW = MAX_LAYERS * 2;
static const unsigned MAX_X = 4, MAX_Y = 4;

static Colour LAYER_COLOURS[3] = {Colours::red, Colours::green, Colours::orange};

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}


static const char tonics[MAX_TONICS][3] = {
    "C ",
    "C#",
    "D ",
    "D#",
    "E ",
    "F ",
    "F#",
    "G ",
    "G#",
    "A ",
    "A#",
    "B ",
};


String getNoteValue(float f) {
    float voct = cv2Pitch(f) + 60.0f; // -5v = 0
    voct += 0.005f; // so we round up fractions of cent
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    int cents = ((voct - floorf(voct)) * 100.0f);
    if (cents > 50) {
        cents = 50 - cents;
        note = (note + 1) % MAX_TONICS;
        oct += (note == 0);
    }
    String cts = String::formatted("%+02d", cents);
    return String(tonics[note] + String(oct - (note < 3))) + " " + cts;
}


SeqStepControl::SeqStepControl(
    PluginEditor& editor, unsigned layer,Parameter &p)
 : BarParamControl(p,0.2f,0.01f), editor_(editor), layer_(layer) {
}

#define GET_P_VAL(x) x.convertFrom0to1(x.getValue())

String SeqStepControl::getTextValue() {
    if( editor_.useNotesForSteps(layer_)) {
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
    if( editor_.useNotesForSteps(layer_)) {
        static constexpr float coarseInc = 1.0f / ( 10.0f * 12.0f ); // +/-5 oct
        static constexpr float fineInc = coarseInc / 100.0f; //cents
        auto& p = param_;
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
    if( editor_.useNotesForSteps(layer_)) {
        static constexpr float coarseInc = 1.0f / ( 10.0f * 12.0f ); // +/-5 oct
        static constexpr float fineInc = coarseInc / 100.0f; //cents
        auto& p = param_;
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

bool PluginEditor::useNotesForSteps(unsigned int layer) {
    auto &l = processor_.params_.layers_[layer];
    return l->scale.getValue() != 0.0f;
}


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW),
      processor_(p),
      currentLayer_(0),
      cvButtonMode_(B_GATEACCESS), encoderMode_(E_CV) {

    leftShiftBtn_.label("GT/GL");
    addAndMakeVisible(leftShiftBtn_);
    rightShiftBtn_.label("FUN");
    addAndMakeVisible(rightShiftBtn_);
    leftBtn_.label("LYR-");
    rightBtn_.label("LYR+");

    unsigned encView = 0;
    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[layer % MAX_LAYERS];
        Colour clr = LAYER_COLOURS[layer % MAX_LAYERS];
        for (unsigned i = 0; i < MAX_STEPS / MAX_Y; i++) {
            int pi = i * MAX_X;
            addParamPage(
                std::make_shared<scontrol_type>(*this, layer, l->steps_[pi + 0]->cv),
                std::make_shared<scontrol_type>(*this, layer, l->steps_[pi + 1]->cv),
                std::make_shared<scontrol_type>(*this, layer, l->steps_[pi + 2]->cv),
                std::make_shared<scontrol_type>(*this, layer, l->steps_[pi + 3]->cv),
                encView,
                clr
            );

            addButtonPage(
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->gate, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->access, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->access, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->access, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->access, 24, clr),
                encView
            );
        }
        for (unsigned i = 0; i < MAX_STEPS / MAX_Y; i++) {
            int pi = i * MAX_X;
            addButtonPage(
                std::make_shared<bcontrol_type>(l->steps_[pi + 0]->glide, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 1]->glide, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 2]->glide, 24, clr),
                std::make_shared<bcontrol_type>(l->steps_[pi + 3]->glide, 24, clr),
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                encView
            );
        }
        encView++;
    }


    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[layer % MAX_LAYERS];
        Colour clr = LAYER_COLOURS[layer % MAX_LAYERS];
        addParamPage(
            std::make_shared<pcontrol_type>(l->snake, 1),
            std::make_shared<pcontrol_type>(l->scale, 1),
            std::make_shared<pcontrol_type>(l->root, 1),
            nullptr,
            encView,
            clr
        );
        addParamPage(
            std::make_shared<pcontrol_type>(l->fun_op_trig, 1),
            std::make_shared<pcontrol_type>(l->fun_op_sleep, 1),
            std::make_shared<pcontrol_type>(l->fun_mod_mode, 1),
            std::make_shared<pcontrol_type>(l->fun_cv_mode, 1),
            encView,
            clr
        );
        encView++;
    }


    // create cell UI
    {
        const int startX = 950, startY = 40;
        const int cellX = 82, cellY = 82;
        const int sp = 2;

        for (unsigned i = 0; i < MAX_STEPS; i++) {

            auto cell = std::make_shared<SeqCell>(processor_.params_, i);
            cells_.push_back(cell);
            int xc = startX + ((i % 4) * (cellX + sp));
            int yc = startY + ((i / 4) * (cellY + sp));

            cell->setBounds(xc, yc, cellX, cellY);
            addAndMakeVisible(*cell);
        }
    }

    setSize(1600, 480);
}


void PluginEditor::resized() {
}

void PluginEditor::onLeftShiftButton(bool v) {
    leftShiftBtn_.value(v);
    if (!v) {
        switch (encoderMode_) {
            case E_CV : {
                switch (cvButtonMode_) {
                    case B_GATEACCESS :
                        cvButtonMode_ = B_GLIDE;
                        setButtonPage(buttonPage_ + MAX_Y);
                        break;
                    case B_GLIDE :
                        cvButtonMode_ = B_GATEACCESS;
                        setButtonPage(buttonPage_ - MAX_Y);
                        break;
                    default:
                        break;
                } // but mode
                break;
            }
            default:
                break;
        } // enc mode
    }
}

void PluginEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.value(v);
    if (!v) {
        encoderMode_ = (EncoderMode) ((encoderMode_ + 1) % MAX_ENCODER_MODE);
        setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
    }
}


void PluginEditor::onUpButton(bool v) {
    upBtn_.value(v);
    if (!v) {
        chgParamPage(-1);
        unsigned nextP = buttonPage_;
        unsigned minP = encoderMode_ == E_CV && cvButtonMode_ == B_GLIDE ? MAX_Y : 0;
        if (nextP > minP) nextP--;
        if (nextP != buttonPage_) setButtonPage(nextP);
    }
}

void PluginEditor::onDownButton(bool v) {
    downBtn_.value(v);
    if (!v) {
        chgParamPage(1);
        unsigned nextP = buttonPage_;
        unsigned maxP = encoderMode_ == E_CV && cvButtonMode_ == B_GATEACCESS ?
                        MAX_Y : (views_[view_].buttonPages_.size());
        if (nextP < (maxP - 1)) nextP++;
        if (nextP != buttonPage_) setButtonPage(nextP);
    }
}

void PluginEditor::onLeftButton(bool v) {
    leftBtn_.value(v);
    if (!v) {
        if (currentLayer_ > 0) {
            currentLayer_--;
            setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
        }
    }
}

void PluginEditor::onRightButton(bool v) {
    rightBtn_.value(v);
    if (!v) {
        if (currentLayer_ < (MAX_LAYERS - 1)) {
            currentLayer_++;
            setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
        }
    }
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    unsigned xP, yP, cP;
    float cv[3];
    bool gates[3];
    processor_.getActiveData(xP, yP, cP, cv[0], cv[1], cv[2], gates[0], gates[1], gates[2]);

    for (auto &cell: cells_) {
        cell->activeStep(xP, yP, cP);
    }

    const int tstartX = 1325, startX = tstartX + 30, startY = 50;
    const int sp = 50;
    const int bw = 100, gw = 30;

    g.setFont(20);
    g.setColour(LAYER_COLOURS[0]);
    g.drawText("X", tstartX, startY, 30, sp, Justification::centredLeft);
    g.setColour(LAYER_COLOURS[1]);
    g.drawText("Y", tstartX, startY + sp, 30, sp, Justification::centredLeft);
    g.setColour(LAYER_COLOURS[2]);
    g.drawText("C", tstartX, startY + (2 * sp), 30, sp, Justification::centredLeft);

    // draw current values
    for (int i = 0; i < 3; i++) {
        float v = cv[i];
        bool gate = gates[i];
        int y = startY + (sp * i);

        auto &l = processor_.params_.layers_[i % MAX_LAYERS];
        if (l->scale.getValue() == 0.0f) {
            if (v >= 0) {
                g.setColour(Colours::green);
                g.fillRect(startX, y + 10, int(bw * v), sp - 20);
            } else {
                g.setColour(Colours::red);
                g.fillRect(startX, y + 10, int(bw * v * -1), sp - 20);
            }
        } else {
            g.setColour(LAYER_COLOURS[i]);
            g.drawText(getNoteValue(v), startX, y, bw, sp, Justification::centredLeft);

        }
        g.setColour(LAYER_COLOURS[i]);
        if (gate) {
            g.fillRect(startX + bw + 10, y + 10, gw - 1, sp - 20);
        } else {
            g.drawRect(startX + bw + 10, y + 10, gw - 1, sp - 20);
        }
    }
}

PluginEditor::SeqCell::SeqCell(PluginProcessor::PluginParams &params, unsigned step) :
    params_(params), step_(step),
    activeX_(false), activeY_(false), activeC_(false) {

}

void PluginEditor::SeqCell::activeStep(unsigned xP, unsigned yP, unsigned cP) {
    activeX_ = step_ == xP;
    activeY_ = step_ == yP;
    activeC_ = step_ == cP;
}

void PluginEditor::SeqCell::paint(Graphics &g) {
    const int sp = 3, ls = 3;
    int sz = getHeight() - (2 * (sp * 2));
    int pos = 2 * sp;

    // draw active cells
    if (activeX_) {
        g.setColour(LAYER_COLOURS[0]);
        g.drawRect(pos, pos, sz, sz, ls);
        pos -= sp;
        sz += (sp * 2);
    }
    if (activeY_) {
        g.setColour(LAYER_COLOURS[1]);
        g.drawRect(pos, pos, sz, sz, ls);
        pos -= sp;
        sz += (sp * 2);
    }
    if (activeC_) {
        g.setColour(LAYER_COLOURS[2]);
        g.drawRect(pos, pos, sz, sz, ls);
    }

    int iw = getWidth() - (4 * (sp * 2));
    int isz = getHeight() - (4 * (sp * 2));
    int ih = isz / 3;
    int ix = 4 * sp;
    int iy = 4 * sp;
    int gw = 10;

    // draw cell values
    for (int i = 0; i < 3; i++) {
        auto &s = params_.layers_[i]->steps_[step_];
        float v = (s->cv.getValue() * 2.0f) - 1.0f;
        bool gate = s->gate.getValue() > 0.5;
        if (v >= 0) {
            g.setColour(Colours::green);
            g.fillRect(ix, iy + 2, int((iw - gw) * v), ih - 4);
        } else {
            g.setColour(Colours::red);
            g.fillRect(ix, iy + 2, int((iw - gw) * v * -1.0f), ih - 4);
        }
        Colour clr = LAYER_COLOURS[i];
        g.setColour(clr);
        if (gate) {
            g.fillRect(ix + (iw - gw) + 1, iy + 2, gw - 1, ih - 4);
        } else {
            g.drawRect(ix + (iw - gw) + 1, iy + 2, gw - 1, ih - 4);
        }
        iy += ih;
    }


}




