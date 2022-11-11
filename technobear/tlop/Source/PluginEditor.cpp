#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p),
      clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow} {
    unsigned nParams = processor_.getNumRnboParameters();

    for (unsigned page = 0; page <= (nParams / 4); page++) {
        int paramS = page * 4;

        std::shared_ptr<ssp::BaseParamControl> p[4];
        for (unsigned i = 0; i < 4; i++) {
            float inc = 1.0f;
            float finc = 0.01f;

            if ((paramS + i) < nParams) {
                const auto &param = processor_.params_.rnboParams_[paramS + i];
                if (param->info_.enumValues != nullptr) {
                    finc = inc;
                } else if (param->info_.steps > 2) {
                    inc = (param->info_.max - param->info_.min) / (param->info_.steps - 1);
                    finc = inc;
                }
                p[i] = std::make_shared<pcontrol_type>(param->val_, inc, finc);
            } else p[i] = nullptr;
        }

        addParamPage(p[0], p[1], p[2], p[3]);
    }


    for (int i = 0; i < MAX_LAYER; i++) {
        std::string title = std::string("Layer ") + std::to_string(i);
        scopes_[i].initSignal(0, title, dataBuf_[i], MAX_DATA, MAX_DATA, clrs_[i]);
        addAndMakeVisible(scopes_[i]);
    }


    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}


void PluginEditor::resized() {
    base_type::resized();
    const unsigned h = 75;
    const unsigned w = 600;
    const unsigned sp = 5;
    unsigned y = 50;
    unsigned x = 900;

    for (int i = 0; i < MAX_LAYER; i++) {
        scopes_[i].setBounds(x,y,w,h);
        y+= h + sp;
    }
}
