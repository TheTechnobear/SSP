
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p), clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow} {
    memset(dataBuf_,0,sizeof (dataBuf_));

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.t_scale, 1),
        nullptr,
        std::make_shared<pcontrol_type>(processor_.params_.trig_src, 1),
        std::make_shared<pcontrol_type>(processor_.params_.trig_lvl, 0.25),
        juce::Colours::lightskyblue
    );


    for (unsigned sig = 0; sig < MAX_SIG; sig++) {
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.sigparams_[sig]->y_scale, 0.25),
            std::make_shared<pcontrol_type>(processor_.params_.sigparams_[sig]->y_offset, 0.25),
            nullptr,
            nullptr,
            clrs_[sig]
        );
    }

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.freeze, 24, Colours::lightskyblue),
        nullptr,
        std::make_shared<bcontrol_type>(processor_.params_.ab_xy, 24, clrs_[0]),
        std::make_shared<bcontrol_type>(processor_.params_.cd_xy, 24, clrs_[2]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[0]->show, 24, clrs_[0]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[1]->show, 24, clrs_[1]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[2]->show, 24, clrs_[2]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[3]->show, 24, clrs_[3])
    );

    setSize(1600, 480);

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

ssp::BaseEditor::ControlPage PluginEditor::addParamPage(
    std::shared_ptr<ssp::BaseParamControl> c1,
    std::shared_ptr<ssp::BaseParamControl> c2,
    std::shared_ptr<ssp::BaseParamControl> c3,
    std::shared_ptr<ssp::BaseParamControl> c4,
    juce::Colour clr
) {
    if (c1) c1->fg(clr);
    if (c2) c2->fg(clr);
    if (c3) c3->fg(clr);
    if (c4) c4->fg(clr);
    return base_type::addParamPage(c1, c2, c3, c4);
}


void PluginEditor::onSSPTimer() {
    base_type::onSSPTimer();
    
    PluginProcessor::DataMsg msg;
    while (processor_.messageQueue().try_dequeue(msg)) {
        for (int i = 0; i < MAX_SIG; i++) {
            dataBuf_[i][wrPos_] = msg.sample_[i];
        }
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


void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    drawValueDisplay(g);
}

void PluginEditor::resized() {
    base_type::resized();

    static constexpr int x = 10, y = 50, w = 900 - 2 * x, h = 400 - 2 * y;
    mainScope_.setBounds(x, y, w, h);

    //replace main scope
    // with miniscope or xyscope is displayed
    unsigned sp = 20;
    unsigned xy_w = h;
    unsigned ms_w = w - xy_w - sp;

    miniScope_[0].setBounds(x, y, ms_w, h);
    miniScope_[1].setBounds(x + xy_w + sp, y, ms_w, h);

    xyScope_[0].setBounds(x, y, xy_w, h);
    xyScope_[1].setBounds(x + ms_w + sp, y, xy_w, h);
}

static constexpr unsigned MAX_TONICS = 12;

static const char tonics[MAX_TONICS][3] = {
    "C ",
    "C#",
    "D ",
    "D#",
    "E ",
    "F ",
    "F#",
    "G ",
    "G#",
    "A ",
    "A#",
    "B ",
};

String  PluginEditor::getNoteValue(float f) const {
    float voct = cv2Pitch(f) + 60.0f; // -5v = 0
    voct += 0.005f; // so we round up fractions of cent
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    int cents = ((voct - floorf(voct)) * 100.0f);
    if (cents > 50) {
        cents = 50-cents;
        note = (note + 1) % MAX_TONICS;
        oct += (note==0);
    }
    String cts=String::formatted("%+02d", cents);
    return String(tonics[note] + String(oct - (note < 3))) + " " + cts;
}

void PluginEditor::drawValueDisplay(Graphics &g) {
    unsigned space = 32;
    unsigned fh = 12 * COMPACT_UI_SCALE;
    int x = 1000;
    int y = 50;
    unsigned fldw = 600 / 5;
    int yspace = fh + space;


    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, Font::plain)));

    g.setColour(juce::Colours::white);
    g.drawText("Min", x, y, fldw, fh, Justification::left);
    g.drawText("Max", x + fldw, y, fldw, fh, Justification::left);
    g.drawText("Avg", x + fldw * 2, y, fldw, fh, Justification::left);
    g.drawText("Note", x + fldw * 3, y, fldw, fh, Justification::left);

    for (unsigned i = 0; i < MAX_SIG; i++) {

        if (processor_.isInputEnabled(PluginProcessor::I_SIG_A + i)) {

            float min = 0.0f, max = 0.0f, sum = 0.0f, avg = 0.0f;
            for (unsigned idx = 0; idx < MAX_DISP; idx++) {
                unsigned didx = (syncPos_ - idx + MAX_DATA) % MAX_DATA;
                float d = dataBuf_[i][didx];
                sum += d;
                min = std::min(d, min);
                max = std::max(d, max);
            }
            avg = sum / MAX_DISP;

            float mult = 1.0f;
            bool volt = true;
            if (volt) mult = 5.0f;

            y += yspace;

            g.setColour(clrs_[i]);
            g.drawText(String::formatted("%+5.3f", min * mult), x, y, fldw, fh, Justification::left);
            g.drawText(String::formatted("%+5.3f", max * mult), x + fldw, y, fldw, fh, Justification::left);
            g.drawText(String::formatted("%+5.3f", avg * mult), x + fldw * 2, y, fldw, fh, Justification::left);
            g.drawText(getNoteValue(avg), x + fldw * 3, y, fldw, fh, Justification::left);
        }
    }
}
