
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    upBtn_.setVisible(false);
    downBtn_.setVisible(false);

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.oper, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.triglevel, 0.1, 0.01f),
        nullptr,
        nullptr
    );

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[0]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[1]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[2]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[3]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[4]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[5]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[6]->inv, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.gateparams_[7]->inv, 24, Colours::lightskyblue)
    );

    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);

    float inputs[PluginProcessor::I_MAX];
    bool outputs[PluginProcessor::O_MAX];
    processor_.getValues(inputs, outputs);

    const int tstartX = 1225, startX = tstartX + 30, startY = 50;
    const int sp = 40;
    const int gw = 25, bw = 100;

    g.setFont(20);

    for (unsigned i = 0; i < PluginProcessor::I_MAX; i++) {
        int y = startY + (sp * i);
        bool gate = inputs[i];
        g.setColour(Colours::white);
        {
            int xb = startX + 10;
            int yb = y + 10;

            float v = inputs[i];
            int w;
            g.setColour(Colours::grey);
            g.drawRect(xb, yb, bw, sp - 20);
            if (v >= 0) {
                w = std::min(bw, int(v * (bw - 2)));
                g.setColour(Colours::green);
            } else {
                w = std::min(bw, int(v * -1.0f * (bw - 2)));
                g.setColour(Colours::red);
            }
            g.fillRect(xb + 1, yb + 1, w, sp - 20 - 2);
        } // inputs


        if (i + 1 < PluginProcessor::O_MAX) {
            g.setColour(Colours::green);
            gate = outputs[i + 1];
            int xb = startX + 10 + bw + sp;
            int yb = y + 10 + (sp / 2) + (i * sp);
            if (gate) {
                g.fillRect(xb, yb, gw - 1, sp - 20);
            } else {
                g.drawRect(xb, yb, gw - 1, sp - 20);
            }
        } // outputs
    }

    {
        g.setColour(Colours::yellow);
        bool gate = outputs[PluginProcessor::O_OUT_ALL];
        int xb = startX + 10 + bw + sp + gw + sp;
        int yb = startY + (sp * 4) - (sp / 2) + 10;
        if (gate) {
            g.fillRect(xb, yb, gw - 1, sp - 20);
        } else {
            g.drawRect(xb, yb, gw - 1, sp - 20);
        }
    } // main out
}
