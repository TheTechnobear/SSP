
#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "SeqCellComponent.h"
#include "SeqStepControl.h"
#include "UIStringFunc.h"

#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"


juce::Colour PluginEditor::LAYER_COLOURS[L_CLRS] = { Colours::red, Colours::green, Colours::orange };

using pcontrol_type = ssp::BarParamControl;
using scontrol_type = SeqStepControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;
static constexpr unsigned MAX_STEPS = PluginProcessor::MAX_STEPS;
static constexpr unsigned MAX_VIEW = MAX_LAYERS * 2;
static const unsigned MAX_X = 4, MAX_Y = 4;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p, MAX_VIEW), processor_(p), currentLayer_(0), cvButtonMode_(B_GATEACCESS), encoderMode_(E_CV) {
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
            auto &scale = processor_.params_.layers_[layer]->scale;

            addParamPage(std::make_shared<scontrol_type>(l->steps_[pi + 0]->cv, scale),
                         std::make_shared<scontrol_type>(l->steps_[pi + 1]->cv, scale),
                         std::make_shared<scontrol_type>(l->steps_[pi + 2]->cv, scale),
                         std::make_shared<scontrol_type>(l->steps_[pi + 3]->cv, scale), encView, clr);

            addButtonPage(std::make_shared<bcontrol_type>(l->steps_[pi + 0]->gate, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 1]->gate, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 2]->gate, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 3]->gate, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 0]->access, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 1]->access, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 2]->access, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 3]->access, 24, clr), encView);
        }
        for (unsigned i = 0; i < MAX_STEPS / MAX_Y; i++) {
            int pi = i * MAX_X;
            addButtonPage(std::make_shared<bcontrol_type>(l->steps_[pi + 0]->glide, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 1]->glide, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 2]->glide, 24, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 3]->glide, 24, clr), nullptr, nullptr, nullptr,
                          nullptr, encView);
        }
        encView++;
    }


    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[layer % MAX_LAYERS];
        Colour clr = LAYER_COLOURS[layer % MAX_LAYERS];
        addParamPage(std::make_shared<pcontrol_type>(l->snake, 1), std::make_shared<pcontrol_type>(l->scale, 1),
                     std::make_shared<pcontrol_type>(l->root, 1), nullptr, encView, clr);
        addParamPage(std::make_shared<pcontrol_type>(l->fun_op_trig, 1),
                     std::make_shared<pcontrol_type>(l->fun_op_sleep, 1),
                     std::make_shared<pcontrol_type>(l->fun_mod_mode, 1),
                     std::make_shared<pcontrol_type>(l->fun_cv_mode, 1), encView, clr);
        encView++;
    }


    // create cell UI
    {
        const int startX = 950, startY = 40;
        const int cellX = 82, cellY = 82;
        const int sp = 2;

        for (unsigned i = 0; i < MAX_STEPS; i++) {
            auto cell = std::make_shared<SeqCellComponent>(processor_.params_, i);
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

void PluginEditor::eventLeftShift(bool v) {
    if (!v) {
        switch (encoderMode_) {
            case E_CV: {
                switch (cvButtonMode_) {
                    case B_GATEACCESS:
                        cvButtonMode_ = B_GLIDE;
                        setButtonPage(buttonPage_ + MAX_Y);
                        break;
                    case B_GLIDE:
                        cvButtonMode_ = B_GATEACCESS;
                        setButtonPage(buttonPage_ - MAX_Y);
                        break;
                    default: break;
                }  // but mode
                break;
            }
            default: break;
        }  // enc mode
    }
}


void PluginEditor::eventRightShift(bool v) {
    if (!v) {
        encoderMode_ = (EncoderMode)((encoderMode_ + 1) % MAX_ENCODER_MODE);
        setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
    }
}


void PluginEditor::eventUp(bool v) {
    if (!v) {
        chgParamPage(-1);
        unsigned nextP = buttonPage_;
        unsigned minP = encoderMode_ == E_CV && cvButtonMode_ == B_GLIDE ? MAX_Y : 0;
        if (nextP > minP) nextP--;
        if (nextP != buttonPage_) setButtonPage(nextP);
    }
}

void PluginEditor::eventDown(bool v) {
    if (!v) {
        chgParamPage(1);
        unsigned nextP = buttonPage_;
        unsigned maxP =
            encoderMode_ == E_CV && cvButtonMode_ == B_GATEACCESS ? MAX_Y : (views_[view_].buttonPages_.size());
        if (nextP < (maxP - 1)) nextP++;
        if (nextP != buttonPage_) setButtonPage(nextP);
    }
}

void PluginEditor::eventLeft(bool v) {
    if (!v) {
        if (currentLayer_ > 0) {
            currentLayer_--;
            setView(currentLayer_ + (encoderMode_ == E_CV ? 0 : MAX_LAYERS));
        }
    }
}

void PluginEditor::eventRight(bool v) {
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

    for (auto &cell : cells_) { cell->activeStep(xP, yP, cP); }

    const int tstartX = 1325, startX = tstartX + 30, startY = 50;
    const int sp = 50;
    const int bw = 100, gw = 30;

    g.setFont(10 * COMPACT_UI_SCALE);
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
