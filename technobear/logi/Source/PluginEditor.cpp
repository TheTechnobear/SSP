
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
        nullptr,
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

    bool inputs[PluginProcessor::I_MAX];
    bool outputs[PluginProcessor::O_MAX];
    processor_.getValues(inputs, outputs);

    const int tstartX = 1225, startX = tstartX + 30, startY = 50;
    const int sp = 40;
    const int gw = 25;

    g.setFont(20);

    for (unsigned i = 0; i < PluginProcessor::I_MAX; i++) {
        int y = startY + (sp * i);
        bool gate = inputs[i];
        g.setColour(Colours::white);
        {
            int xb = startX + 10;
            int yb = y + 10;
            if (gate) {
                g.fillRect(xb, yb, gw - 1, sp - 20);
            } else {
                g.drawRect(xb, yb, gw - 1, sp - 20);
            }
        }

        if (i + 1 < PluginProcessor::O_MAX) {
            g.setColour(Colours::green);
            gate = outputs[i + 1];
            int xb = startX + 10 + gw + sp;
            int yb = y + 10 + (sp / 2) + (i * sp);
            if (gate) {
                g.fillRect(xb, yb, gw - 1, sp - 20);
            } else {
                g.drawRect(xb, yb, gw - 1, sp - 20);
            }
        }
    }

    {
        g.setColour(Colours::yellow);
        bool gate = outputs[PluginProcessor::O_OUT_ALL];
        int xb = startX + 10 + (gw + sp) * 2;
        int yb = startY + (sp * 4) - (sp / 2) + 10;
        if (gate) {
            g.fillRect(xb, yb, gw - 1, sp - 20);
        } else {
            g.drawRect(xb, yb, gw - 1, sp - 20);
        }
    }

}