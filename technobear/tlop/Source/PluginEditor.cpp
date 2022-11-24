#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p, unsigned maxviews)
    : base_type(&p, maxviews),
      processor_(p),
      clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow} {

    unsigned view = 0;
    float inc = 1.0f;
    float finc = 0.01f;

    leftBtn_.label("LYR-");
    rightBtn_.label("LYR+");


    auto &reclayer = processor_.params_.recParams_;
    addParamPage(
        std::make_shared<pcontrol_type>(reclayer->layer_, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(reclayer->gain_, inc, finc),
        std::make_shared<pcontrol_type>(reclayer->mon_, inc, finc),
        nullptr,
        view,
        Colours::cyan
    );


    addButtonPage(
        std::make_shared<bcontrol_type>(reclayer->mode_, 24, Colours::red),
        std::make_shared<bcontrol_type>(reclayer->loop_, 24, Colours::yellow),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        view
    );

    view++;

    for (int l = 0; l < PluginProcessor::MAX_LAYERS; l++) {
        auto &layer = processor_.params_.layers_[l];
        addParamPage(
            std::make_shared<pcontrol_type>(layer->rate_, inc, finc),
            std::make_shared<pcontrol_type>(layer->begin_, inc, finc),
            std::make_shared<pcontrol_type>(layer->end_, inc, finc),
            std::make_shared<pcontrol_type>(layer->gain_, inc, finc),
            view,
            clrs_[l]
        );
        addParamPage(
            std::make_shared<pcontrol_type>(layer->crossfade_, inc, finc),
            nullptr,
            nullptr,
            nullptr,
            view
        );
        addButtonPage(
            std::make_shared<bcontrol_type>(layer->mode_, 24, Colours::red),
            std::make_shared<bcontrol_type>(layer->loop_, 24, Colours::yellow),
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            view
        );

        view++;
    }



//    unsigned nParams = processor_.getNumRnboParameters();
//
//    int paramS = 0;
//    for (unsigned view = 0; view < maxviews; view++) {
//        for (unsigned row = 0; (row < 4) && (paramS < nParams); row++) {
//
//            std::shared_ptr<ssp::BaseParamControl> p[4];
//            for (unsigned i = 0; i < 4; i++) {
//                float inc = 1.0f;
//                float finc = 0.01f;
//                if (paramS < nParams) {
//                    const auto &param = processor_.params_.rnboParams_[paramS];
//                    if (param->info_.enumValues != nullptr) {
//                        finc = inc;
//                    } else if (param->info_.steps > 2) {
//                        inc = (param->info_.max - param->info_.min) / (param->info_.steps - 1);
//                        finc = inc;
//                    }
//                    p[i] = std::make_shared<pcontrol_type>(param->val_, inc, finc);
//                } else {
//                    p[i] = nullptr;
//                }
//                paramS++;
//            }
//            addParamPage(p[0], p[1], p[2], p[3], view, clrs_[view % 4]);
//        }
//    }


    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        std::string title = std::string("Layer ") + std::to_string(i);
        scopes_[i].initSignal(0, title, layer.dataBuf_, MAX_DATA, MAX_VIEWPOINTS, clrs_[i]);
        addAndMakeVisible(scopes_[i]);
    }


    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        processor_.fillLayerData(i, layer.dataBuf_, MAX_DATA,
                                 layer.curPos_, layer.beginPos_, layer.endPos_,
                                 layer.isRec_, layer.recPos_);
        scopes_[i].setPosition(0, layer.curPos_, layer.beginPos_, layer.endPos_);
        scopes_[i].setRecPosition(0, layer.isRec_, layer.recPos_);

        base_type::drawView(g);

        if (layer.isRec_) {
            const unsigned h = 75;
            const unsigned w = MAX_VIEWPOINTS;
            const unsigned sp = 5;
            unsigned y = 50;
            unsigned x = 910;
            g.setColour(Colours::darkgrey);
            g.drawRect(x - 2, (i * (h + sp)) + y - 2, w + 4, h + 4, 1);
        }
    }
}


void PluginEditor::resized() {
    base_type::resized();
    const unsigned h = 75;
    const unsigned w = MAX_VIEWPOINTS;
    const unsigned sp = 5;
    unsigned y = 50;
    unsigned x = 910;

    for (int i = 0; i < MAX_LAYERS; i++) {
        scopes_[i].setBounds(x, y, w, h);
        y += h + sp;
    }
}
