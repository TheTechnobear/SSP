#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}

PluginEditor::PluginEditor(PluginProcessor &p, unsigned maxviews)
    : base_type(&p, maxviews),
      processor_(p),
      clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow} {

    unsigned view = 0;
    //fixme: sort out course and fine tune
    float inc = 1.0f;
    float finc = 0.01f;

    leftBtn_.label("LYR-");
    rightBtn_.label("LYR+");
    rightShiftBtn_.label("REC");
    addAndMakeVisible(rightShiftBtn_);


    auto &reclayer = processor_.params_.recParams_;


    // layer pages
    for (int l = 0; l < PluginProcessor::MAX_LAYERS; l++) {
        auto &layer = processor_.params_.layers_[l];
        addParamPage(
            std::make_shared<pcontrol_type>(layer->rate_, inc, finc),
            std::make_shared<pcontrol_type>(layer->gain_, inc, finc),
            std::make_shared<pcontrol_type>(layer->begin_, inc, finc),
            std::make_shared<pcontrol_type>(layer->end_, inc, finc),
            view,
            clrs_[l]
        );
        addParamPage(
            std::make_shared<pcontrol_type>(layer->xfade_, inc, finc),
            nullptr,
            nullptr,
            nullptr,
            view,
            clrs_[l]
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

    // recording page
    addParamPage(
        std::make_shared<pcontrol_type>(reclayer->gain_, inc, finc),
        std::make_shared<pcontrol_type>(reclayer->mon_, inc, finc),
        nullptr,
        nullptr,
        view,
        Colours::cyan
    );
    addParamPage(
        nullptr, //std::make_shared<pcontrol_type>(reclayer->begin_, inc, finc),
        nullptr, // std::make_shared<pcontrol_type>(reclayer->end_, inc, finc),
        nullptr,
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


    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        std::string title = std::string("Layer ") + std::to_string(i);
        scopes_[i].initSignal(0, title, layer.dataBuf_, MAX_DATA, MAX_VIEWPOINTS, clrs_[i]);
        addAndMakeVisible(scopes_[i]);
    }

    unsigned layer = normValue(processor_.params_.recParams_->layer_);
    setView(layer);
    viewMode_ = M_LAYER;

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


void PluginEditor::onRightShiftButton(bool v) {
    rightShiftBtn_.value(v);
    if (!v) {
        viewMode_ = (viewMode_ + 1) % M_MAX;
        switch (viewMode_) {
            case M_LAYER : {
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                setView(layer);
                return;
            }
            case M_REC : {
                setView(MAX_LAYERS);
                return;
            }
        }
    }
}


void PluginEditor::onLeftButton(bool v) {
    leftBtn_.value(v);
    if (!v) {
        if (viewMode_ == M_LAYER) {
            int newView = int(view_) - 1;
            if (newView >= 0) {
                setView(newView);
                auto& p = processor_.params_.recParams_->layer_;
                p.beginChangeGesture();
                p.setValueNotifyingHost(p.convertTo0to1(newView));
                p.endChangeGesture();
            }
        }
    }
}

void PluginEditor::onRightButton(bool v) {
    rightBtn_.value(v);
    if (!v) {
        if (viewMode_ == M_LAYER) {
            int newView = int(view_) + 1;
            if (newView < MAX_LAYERS) {
                setView(newView);
                auto& p = processor_.params_.recParams_->layer_;
                p.beginChangeGesture();
                p.setValueNotifyingHost(p.convertTo0to1(newView));
                p.endChangeGesture();
            }
        }
    }
}

