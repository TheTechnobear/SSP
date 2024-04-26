
#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p), processor_(p), clrs_{ Colours::green, Colours::blue, Colours::red, Colours::yellow } {
    memset(dataBuf_, 0, sizeof(dataBuf_));

    addParamPage(std::make_shared<pcontrol_type>(processor_.params_.t_scale, 1), nullptr,
                 std::make_shared<pcontrol_type>(processor_.params_.trig_src, 1),
                 std::make_shared<pcontrol_type>(processor_.params_.trig_lvl, 0.25), juce::Colours::lightskyblue);


    for (unsigned sig = 0; sig < MAX_SIG; sig++) {
        addParamPage(std::make_shared<pcontrol_type>(processor_.params_.sigparams_[sig]->y_scale, 0.25),
                     std::make_shared<pcontrol_type>(processor_.params_.sigparams_[sig]->y_offset, 0.25), nullptr,
                     nullptr, clrs_[sig]);
    }

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.freeze, 12 * COMPACT_UI_SCALE, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.ab_xy, 12 * COMPACT_UI_SCALE, clrs_[0]),
        std::make_shared<bcontrol_type>(processor_.params_.cd_xy, 12 * COMPACT_UI_SCALE, clrs_[2]), 
        nullptr,
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[0]->show, 12 * COMPACT_UI_SCALE, clrs_[0]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[1]->show, 12 * COMPACT_UI_SCALE, clrs_[1]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[2]->show, 12 * COMPACT_UI_SCALE, clrs_[2]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[3]->show, 12 * COMPACT_UI_SCALE, clrs_[3]));
        
    {
        // clear queue, when we create a new editor
        PluginProcessor::DataMsg msg;
        while (processor_.messageQueue().try_dequeue(msg)) { ; }
    }

    for (int i = 0; i < MAX_SIG; i++) {
        std::string title = std::string("In ") + std::to_string(i);
        mainScope_.initSignal(i, title, dataBuf_[i], MAX_DATA, MAX_DISP, clrs_[i]);

        miniScope_[i / 2].initSignal(i % 2, title, dataBuf_[i], MAX_DATA, MAX_DISP, clrs_[i]);
    }

    xyScope_[0].init("In A", dataBuf_[0], MAX_DATA, "In B", dataBuf_[1], MAX_DATA, MAX_DISP, clrs_[0]);
    xyScope_[1].init("In C", dataBuf_[2], MAX_DATA, "In D", dataBuf_[3], MAX_DATA, MAX_DISP, clrs_[2]);

    addChildComponent(mainScope_);
    addChildComponent(miniScope_[0]);
    addChildComponent(miniScope_[1]);
    addChildComponent(xyScope_[0]);
    addChildComponent(xyScope_[1]);

    for (int i = 0; i < MAX_SIG; i++) {
        auto &sParam = *processor_.params_.sigparams_[i];
        bool vis = processor_.isInputEnabled(PluginProcessor::I_SIG_A + i) && sParam.show.getValue() > 0.5f;
        mainScope_.signalVisible(i, vis);
    }


    bool abxy = processor_.params_.ab_xy.getValue() > 0.5f;
    bool cdxy = processor_.params_.cd_xy.getValue() > 0.5f;
    bool main = !abxy && !cdxy;

    miniScope_[0].setVisible(!main && !abxy);
    miniScope_[1].setVisible(!main && !cdxy);
    xyScope_[0].setVisible(!main && abxy);
    xyScope_[1].setVisible(!main && cdxy);
    mainScope_.setVisible(main);
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


void PluginMiniEditor::onSSPTimer() {
    base_type::onSSPTimer();

    PluginProcessor::DataMsg msg;
    while (processor_.messageQueue().try_dequeue(msg)) {
        for (int i = 0; i < MAX_SIG; i++) { dataBuf_[i][wrPos_] = msg.sample_[i]; }
        dataBuf_[MAX_SIG][wrPos_] = msg.trig_;
        wrPos_ = (wrPos_ + 1) % MAX_DATA;
    }

    syncPos_ = (int(wrPos_) - 1 - MAX_DISP + MAX_DATA) % MAX_DATA;

    unsigned trigSrc = processor_.params_.trig_src.convertFrom0to1(processor_.params_.trig_src.getValue());

    if (trigSrc != 0) {
        float trigLvl = processor_.params_.trig_lvl.convertFrom0to1(processor_.params_.trig_lvl.getValue());
        bool pos = false;
        for (unsigned i = 0; i < (MAX_DISP); i++) {
            unsigned idx = (syncPos_ - i + MAX_DATA) % MAX_DATA;
            if (pos && dataBuf_[trigSrc - 1][idx] < trigLvl) {
                syncPos_ = (idx + 1 + MAX_DATA) % MAX_DATA;
                break;
            }
            pos = dataBuf_[trigSrc - 1][idx] >= trigLvl;
        }
    }

    for (int i = 0; i < MAX_SIG; i++) {
        auto &sParam = *processor_.params_.sigparams_[i];

        bool vis = processor_.isInputEnabled(PluginProcessor::I_SIG_A + i) && sParam.show.getValue() > 0.5f;
        mainScope_.signalVisible(i, vis);

        float scale = sParam.y_scale.convertFrom0to1(sParam.y_scale.getValue());
        float offset = sParam.y_offset.convertFrom0to1(sParam.y_offset.getValue());
        mainScope_.scaleOffset(i, scale, offset);

        mainScope_.pos(i, syncPos_);

        miniScope_[i / 2].pos(i % 2, syncPos_);
        miniScope_[i / 2].pos(i % 2, syncPos_);

        //        miniScope_[i / 2].scaleOffset(i % 2, scale, offset);
        //        xyScope_[i / 2].scaleOffset(i % 2, scale, offset);
    }

    bool abxy = processor_.params_.ab_xy.getValue() > 0.5f;
    bool cdxy = processor_.params_.cd_xy.getValue() > 0.5f;
    bool main = !abxy && !cdxy;

    miniScope_[0].setVisible(!main && !abxy);
    miniScope_[1].setVisible(!main && !cdxy);
    xyScope_[0].setVisible(!main && abxy);
    xyScope_[1].setVisible(!main && cdxy);
    mainScope_.setVisible(main);

    xyScope_[0].pos(syncPos_);
    xyScope_[1].pos(syncPos_);
}


void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
}

void PluginMiniEditor::resized() {
    base_type::resized();
    static constexpr int gap = 5 * COMPACT_UI_SCALE;
    int x = canvasX();
    int y = canvasY() + gap;
    int w = canvasWidth();
    int h = canvasHeight() - 2 * gap;

    mainScope_.setBounds(x, y, w, h);

    // replace main scope
    // with miniscope or xyscope is displayed
    unsigned sp = 10 * COMPACT_UI_SCALE;
    unsigned xy_w = h;
    unsigned ms_w = w - xy_w - sp;

    miniScope_[0].setBounds(x, y, ms_w, h);
    miniScope_[1].setBounds(x + xy_w + sp, y, ms_w, h);

    xyScope_[0].setBounds(x, y, xy_w, h);
    xyScope_[1].setBounds(x + ms_w + sp, y, xy_w, h);
}
