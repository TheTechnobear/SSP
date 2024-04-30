
#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p), processor_(p), clrs_{ Colours::green, Colours::blue, Colours::orange, Colours::yellow } {
    static constexpr float inc = 1.0f;
    static constexpr float finc = 0.01f;
    static constexpr unsigned fh = 12 * COMPACT_UI_SCALE;

    {  // recording parameters
        juce::Colour clr = Colours::red;
        auto &reclayer = processor_.params_.recParams_;

        addParamPage(std::make_shared<pcontrol_type>(reclayer->layer_, inc, inc),
                     std::make_shared<pcontrol_type>(reclayer->gain_, inc, finc),
                     std::make_shared<pcontrol_type>(reclayer->mon_, inc, finc), nullptr, clr);

        addButtonPage(std::make_shared<bcontrol_type>(reclayer->mode_, fh, Colours::red),
                      std::make_shared<bcontrol_type>(reclayer->loop_, fh, Colours::yellow), nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr);
    }


    for (int lidx = 0; lidx < MAX_LAYERS; lidx++) {
        juce::Colour clr = clrs_[lidx];
        auto &layer = processor_.params_.layers_[lidx];
        addParamPage(std::make_shared<pcontrol_type>(layer->rate_, inc, finc),
                     std::make_shared<pcontrol_type>(layer->gain_, inc, finc),
                     std::make_shared<pcontrol_type>(layer->begin_, 0.1f, 0.01),
                     std::make_shared<pcontrol_type>(layer->end_, 0.1, 0.01), clr);
        addButtonPage(std::make_shared<bcontrol_type>(layer->mode_, fh, Colours::red),
                      std::make_shared<bcontrol_type>(layer->loop_, fh, Colours::yellow), nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr);

        addParamPage(std::make_shared<pcontrol_type>(layer->xfade_, inc, finc),
                     std::make_shared<pcontrol_type>(layer->size_, 1.0, 1.0), nullptr, nullptr, clr);

        addButtonPage(std::make_shared<bcontrol_type>(layer->mode_, fh, Colours::red),
                      std::make_shared<bcontrol_type>(layer->loop_, fh, Colours::yellow), nullptr, nullptr, nullptr,
                      nullptr, nullptr, nullptr);
    }

    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        std::string title = std::string("Layer ") + std::to_string(i);
        scopes_[i].initSignal(0, title, layer.dataBuf_, DATA_POINTS, DATA_POINTS, clrs_[i]);
        addAndMakeVisible(scopes_[i]);
    }
}

void PluginMiniEditor::onSSPTimer() {
    base_type::onSSPTimer();
    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        processor_.fillLayerData(i, layer.dataBuf_, DATA_POINTS, layer.curPos_, layer.beginPos_, layer.endPos_,
                                 layer.isRec_, layer.recPos_);
        scopes_[i].setPosition(0, layer.curPos_, layer.beginPos_, layer.endPos_);
        scopes_[i].setRecPosition(0, layer.isRec_, layer.recPos_);
        scopes_[i].label(0, processor_.getLayerFile(i));
    }
}


void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    const juce::Colour bg(0xff111111);

    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        if (layer.isRec_) {
            const int h = 37 * COMPACT_UI_SCALE;
            const int w = DATA_POINTS;
            const int sp = 2 * COMPACT_UI_SCALE;
            int y = canvasY();
            int x = canvasX() + 10 * COMPACT_UI_SCALE;
            g.setColour(bg);
            g.fillRect(x, (i * (h + sp)) + y, w, h);
        }
    }
}

void PluginMiniEditor::resized() {
    base_type::resized();
    const unsigned h = 37 * COMPACT_UI_SCALE;
    const unsigned w = DATA_POINTS;
    const unsigned sp = 2 * COMPACT_UI_SCALE;
    unsigned y = canvasY();
    unsigned x = canvasX() + 10 * COMPACT_UI_SCALE;

    for (int i = 0; i < MAX_LAYERS; i++) {
        scopes_[i].setBounds(x, y, w, h);
        y += h + sp;
    }
}
