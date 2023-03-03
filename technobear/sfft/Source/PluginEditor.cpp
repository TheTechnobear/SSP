#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p, unsigned maxviews)
    : base_type(&p, maxviews),
      processor_(p),
      clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow} {
    unsigned nParams = processor_.getNumRnboParameters();

    unsigned paramS = 0;
    for (unsigned view = 0; view < maxviews; view++) {
        for (unsigned row = 0; (row < 4) && (paramS < nParams); row++) {

            std::shared_ptr<ssp::BaseParamControl> p[4];
            for (unsigned i = 0; i < 4; i++) {
                float inc = 1.0f;
                float finc = 0.01f;

                if (paramS < nParams) {
                    const auto &param = processor_.params_.rnboParams_[paramS];
                    if (param->info_.enumValues != nullptr) {
                        finc = inc;
                    } else if (param->info_.steps > 2) {
                        inc = (param->info_.max - param->info_.min) / (param->info_.steps - 1);
                        finc = inc;
                    }
                    p[i] = std::make_shared<pcontrol_type>(param->val_, inc, finc);
                } else {
                    p[i] = nullptr;
                }
                paramS++;
            }
            addParamPage(p[0], p[1], p[2], p[3], view, clrs_[view % 4]);
        }
    }

    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}


void PluginEditor::resized() {
    base_type::resized();
    const unsigned h = 130;
    const unsigned sp = 10;
    unsigned y = 50;
    unsigned x = 0;
}
