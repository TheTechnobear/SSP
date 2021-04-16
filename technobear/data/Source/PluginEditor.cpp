
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/BaseParamControl.h"
#include "ssp/ButtonControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ButtonControl;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p,
                String(JucePlugin_Name) + " : " + String(JucePlugin_Desc),
                JucePlugin_VersionString),
      processor_(p), clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow} {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.t_scale, 1),
        nullptr,
        nullptr,
        nullptr,
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
        std::make_shared<bcontrol_type>(processor_.params_.ab_xy, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.cd_xy, 24, Colours::lightskyblue),
        nullptr,
        nullptr,
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
        std::string title;
        scopes_[i].init(title, dataBuf_[i], MAX_DATA, clrs_[i], false);
        addAndMakeVisible(scopes_[i]);
    }
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


void PluginEditor::timerCallback() {
    PluginProcessor::DataMsg msg;
    while (processor_.messageQueue().try_dequeue(msg)) {
        for (int i = 0; i < MAX_SIG; i++) {
            dataBuf_[i][wrPos_] = msg.sample_[i];
        }
        wrPos_ = (wrPos_ + 1) % MAX_DATA;
    }

    for (int i = 0; i < MAX_SIG; i++) {
        auto &scope = scopes_[i];
        auto &sParam = *processor_.params_.sigparams_[i];

        bool vis = processor_.isInputEnabled(PluginProcessor::I_SIG_A + i) && sParam.show.getValue() > 0.5f;
        scope.setVisible(vis);

        float scale = sParam.y_scale.convertFrom0to1(sParam.y_scale.getValue());
        float offset = sParam.y_offset.convertFrom0to1(sParam.y_offset.getValue());
        scope.scaleOffset(scale, offset);
    }
    base_type::timerCallback();
}


void PluginEditor::paint(Graphics &g) {
    base_type::paint(g);
    drawGrid(g);
    drawValueDisplay(g);
}

void PluginEditor::resized() {
    static constexpr int x = 10, y = 50, w = 900 - 2 * x, h = 400 - 2 * y;

    scopes_[0].setBounds(x, y, w, h);
    scopes_[1].setBounds(x, y, w, h);
    scopes_[2].setBounds(x, y, w, h);
    scopes_[3].setBounds(x, y, w, h);
}


void PluginEditor::drawGrid(Graphics &g) {
    auto &scope = scopes_[0];
    int x = scope.getX() - 5;
    int y = scope.getY() - 5;
    int w = scope.getWidth() + 10;
    int h = scope.getHeight() + 10;

    g.setColour(Colours::darkgrey);
    unsigned div = 10;
    int sw = w / div;
    int sh = h / div;
    for (unsigned i = 0; i < 11; i++) {
        g.fillRect(x + (i * sw), y, 1, h); //vert
        g.fillRect(x, y + (i * sh), w, i != 5 ? 1 : 3); //horz
    }
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

String getNoteValue(float f) {
    float voct = cv2Pitch(f) + 60.0f; // -5v = 0
    int oct = voct / 12;
    unsigned note = unsigned(voct) % MAX_TONICS;
    unsigned cents = (voct - floorf(voct)) * 100;
    if (cents > 50) {
        cents -= 50;
        note = (note + 1) % MAX_TONICS;
    }
    return String(tonics[note] + String(oct - (note < 3))) + "." + String(cents);
}

void PluginEditor::drawValueDisplay(Graphics &g) {
    unsigned space = 32;
    unsigned fh = 24;
    int x = 950;
    int y = 50;
    unsigned fldw = 600 / 5;
    int yspace = fh + space;


    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(juce::Colours::white);
    g.drawText("Min", x, y, fldw, fh, Justification::left);
    g.drawText("Max", x + fldw, y, fldw, fh, Justification::left);
    g.drawText("Avg", x + fldw * 2, y, fldw, fh, Justification::left);
    g.drawText("Note", x + fldw * 3, y, fldw, fh, Justification::left);

    for (unsigned i = 0; i < MAX_SIG; i++) {

        if (processor_.isInputEnabled(PluginProcessor::I_SIG_A + i)) {

            float min = 0.0f, max = 0.0f, sum = 0.0f, avg = 0.0f;
            for (unsigned idx = 0; idx < MAX_DATA; idx++) {
                float d = dataBuf_[i][idx];
                sum += d;
                min = std::min(d, min);
                max = std::max(d, max);
            }
            avg = sum / MAX_DATA;

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
