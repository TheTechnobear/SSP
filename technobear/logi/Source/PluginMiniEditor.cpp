
#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    addParamPage(std::make_shared<pcontrol_type>(processor_.params_.oper, 1.0f, 1.0f),
                 std::make_shared<pcontrol_type>(processor_.params_.triglevel, 0.1, 0.01f), nullptr, nullptr);

    static constexpr unsigned fh = 12 * COMPACT_UI_SCALE;
    addButtonPage(std::make_shared<bcontrol_type>(processor_.params_.gateparams_[0]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[1]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[2]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[3]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[4]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[5]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[6]->inv, fh, Colours::lightskyblue),
                  std::make_shared<bcontrol_type>(processor_.params_.gateparams_[7]->inv, fh, Colours::lightskyblue));
}


void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);

    float inputs[PluginProcessor::I_MAX];
    bool outputs[PluginProcessor::O_MAX];
    processor_.getValues(inputs, outputs);


    const int sp = 20 * COMPACT_UI_SCALE;
    const int hsp = sp / 2;
    const int qsp = sp / 4;

    const int gw = 12 * COMPACT_UI_SCALE;
    const int bw = 50 * COMPACT_UI_SCALE;
    const int tstartX = canvasX(), startX = tstartX + 15 * COMPACT_UI_SCALE, startY = canvasY();

    for (unsigned i = 0; i < PluginProcessor::I_MAX; i++) {
        int y = startY + (sp * i);
        bool gate = inputs[i];
        g.setColour(Colours::white);
        {
            int xb = startX + qsp;
            int yb = y + qsp;

            float v = inputs[i];
            int w;
            g.setColour(Colours::grey);
            g.drawRect(xb, yb, bw, sp - hsp);
            if (v >= 0) {
                w = std::min(bw, int(v * (bw - 2)));
                g.setColour(Colours::green);
            } else {
                w = std::min(bw, int(v * -1.0f * (bw - 2)));
                g.setColour(Colours::red);
            }
            g.fillRect(xb + 1, yb + 1, w, sp - hsp - 2);
        }  // inputs


        if (i + 1 < PluginProcessor::O_MAX) {
            g.setColour(Colours::green);
            gate = outputs[i + 1];
            int xb = startX + qsp + bw + sp;
            int yb = y + qsp + (sp / 2) + (i * sp);
            if (gate) {
                g.fillRect(xb, yb, gw - 1, sp - hsp);
            } else {
                g.drawRect(xb, yb, gw - 1, sp - hsp);
            }
        }  // outputs
    }

    {
        g.setColour(Colours::yellow);
        bool gate = outputs[PluginProcessor::O_OUT_ALL];
        int xb = startX + qsp + bw + sp + gw + sp;
        int yb = startY + (sp * 4) - (sp / 2) + qsp;
        if (gate) {
            g.fillRect(xb, yb, gw - 1, sp - hsp);
        } else {
            g.drawRect(xb, yb, gw - 1, sp - hsp);
        }
    }  // main out
}
