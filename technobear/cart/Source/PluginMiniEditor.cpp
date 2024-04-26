
#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "SeqCellComponent.h"
#include "SeqStepControl.h"
#include "UIStringFunc.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"


juce::Colour PluginMiniEditor::LAYER_COLOURS[L_CLRS] = { Colours::red, Colours::green, Colours::orange };

using pcontrol_type = ssp::BarParamControl;
using scontrol_type = SeqStepControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_LAYERS = PluginProcessor::MAX_LAYERS;
static constexpr unsigned MAX_STEPS = PluginProcessor::MAX_STEPS;
static const unsigned MAX_X = 4, MAX_Y = 4;


PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    static constexpr unsigned fh = 12 * COMPACT_UI_SCALE;

    for (unsigned layer = 0; layer < MAX_LAYERS; layer++) {
        auto &l = processor_.params_.layers_[layer % MAX_LAYERS];
        Colour clr = LAYER_COLOURS[layer % MAX_LAYERS];
        for (unsigned i = 0; i < MAX_STEPS / MAX_Y; i++) {
            int pi = i * MAX_X;
            auto &scale = processor_.params_.layers_[layer]->scale;

            addParamPage(std::make_shared<scontrol_type>(l->steps_[pi + 0]->cv, scale),
                         std::make_shared<scontrol_type>(l->steps_[pi + 1]->cv, scale),
                         std::make_shared<scontrol_type>(l->steps_[pi + 2]->cv, scale),
                         std::make_shared<scontrol_type>(l->steps_[pi + 3]->cv, scale), clr);

            addButtonPage(std::make_shared<bcontrol_type>(l->steps_[pi + 0]->gate, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 1]->gate, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 2]->gate, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 3]->gate, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 0]->access, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 1]->access, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 2]->access, fh, clr),
                          std::make_shared<bcontrol_type>(l->steps_[pi + 3]->access, fh, clr));

            // addButtonPage(std::make_shared<bcontrol_type>(l->steps_[pi + 0]->glide, fh, clr),
            //               std::make_shared<bcontrol_type>(l->steps_[pi + 1]->glide, fh, clr),
            //               std::make_shared<bcontrol_type>(l->steps_[pi + 2]->glide, fh, clr),
            //               std::make_shared<bcontrol_type>(l->steps_[pi + 3]->glide, fh, clr), nullptr, nullptr,
            //               nullptr, nullptr);

        }  // steps / v

        addParamPage(std::make_shared<pcontrol_type>(l->snake, 1), std::make_shared<pcontrol_type>(l->scale, 1),
                     std::make_shared<pcontrol_type>(l->root, 1), nullptr, clr);
        addButtonPage(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

        addParamPage(std::make_shared<pcontrol_type>(l->fun_op_trig, 1),
                     std::make_shared<pcontrol_type>(l->fun_op_sleep, 1),
                     std::make_shared<pcontrol_type>(l->fun_mod_mode, 1),
                     std::make_shared<pcontrol_type>(l->fun_cv_mode, 1), clr);
        addButtonPage(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }  // each layer

    // create cell UI
    {
        const int startX = canvasX(), startY = canvasY();
        const int cellX = 41 * COMPACT_UI_SCALE, cellY = cellX;
        const int sp = COMPACT_UI_SCALE;

        for (unsigned i = 0; i < MAX_STEPS; i++) {
            auto cell = std::make_shared<SeqCellComponent>(processor_.params_, i);
            cells_.push_back(cell);
            int xc = startX + ((i % 4) * (cellX + sp));
            int yc = startY + ((i / 4) * (cellY + sp));

            cell->setBounds(xc, yc, cellX, cellY);
            addAndMakeVisible(*cell);
        }
    }
}

void PluginMiniEditor::addParamPage(std::shared_ptr<ssp::BaseParamControl> c1,
                                    std::shared_ptr<ssp::BaseParamControl> c2,
                                    std::shared_ptr<ssp::BaseParamControl> c3,
                                    std::shared_ptr<ssp::BaseParamControl> c4, juce::Colour clr) {
    if (c1) c1->fg(clr);
    if (c2) c2->fg(clr);
    if (c3) c3->fg(clr);
    if (c4) c4->fg(clr);
    return base_type::addParamPage(c1, c2, c3, c4);
}


void PluginMiniEditor::resized() {
    base_type::resized();
}

void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    unsigned xP, yP, cP;
    float cv[3];
    bool gates[3];
    processor_.getActiveData(xP, yP, cP, cv[0], cv[1], cv[2], gates[0], gates[1], gates[2]);

    for (auto &cell : cells_) { cell->activeStep(xP, yP, cP); }

    const int tstartX = canvasX() + 200 * COMPACT_UI_SCALE;
    const int startX = tstartX + (15 * COMPACT_UI_SCALE), startY = (25 * COMPACT_UI_SCALE);
    const int sp = 25 * COMPACT_UI_SCALE;
    const int bw = 50 * COMPACT_UI_SCALE, gw = 15 * COMPACT_UI_SCALE;
    const int fh = 15 * COMPACT_UI_SCALE;

    g.setFont(10 * COMPACT_UI_SCALE);
    g.setColour(LAYER_COLOURS[0]);
    g.drawText("X", tstartX, startY, fh, sp, Justification::centredLeft);
    g.setColour(LAYER_COLOURS[1]);
    g.drawText("Y", tstartX, startY + sp, fh, sp, Justification::centredLeft);
    g.setColour(LAYER_COLOURS[2]);
    g.drawText("C", tstartX, startY + (2 * sp), fh, sp, Justification::centredLeft);

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
