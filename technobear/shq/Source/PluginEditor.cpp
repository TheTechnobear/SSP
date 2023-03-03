#include "PluginProcessor.h"
#include "PluginEditor.h"


#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.root, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.scale, 1.0f, 1.0f),
        nullptr,
        nullptr
    );

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.quant, 24, Colours::lightskyblue),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    // only one page of parameters, so up/down not required
    upBtn_.setVisible(false);
    downBtn_.setVisible(false);
    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    if (processor_.params_.quant.getValue() > 0) {
        unsigned fh = 48;
        g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
        g.setColour(Colours::red);
        String root = processor_.params_.root.getCurrentValueAsText();
        String scale = processor_.params_.scale.getCurrentValueAsText();
        g.drawText(root + " " + scale, 20, 64, 900, 34, Justification::left);
    }
}

