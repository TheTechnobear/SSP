
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
      processor_(p) {

    static juce::Colour clrs[4] = {Colours::green, Colours::blue, Colours::red, Colours::yellow};


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
            clrs[sig]
        );
    }

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.ab_xy, 24, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.cd_xy, 24, Colours::lightskyblue),
        nullptr,
        nullptr,
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[0]->show, 24, clrs[0]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[1]->show, 24, clrs[1]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[2]->show, 24, clrs[2]),
        std::make_shared<bcontrol_type>(processor_.params_.sigparams_[3]->show, 24, clrs[3])
    );

    setSize(1600, 480);

    {
        // clear queue, when we create a new editor
        PluginProcessor::DataMsg msg;
        while (processor_.messageQueue().try_dequeue(msg)) { ; }
    }

    for (int i = 0; i < MAX_SIG; i++) {
        std::string title;
        scopes_[i].init(title, dataBuf_[i], MAX_DATA, clrs[i], false);
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
    g.setColour(Colours::grey);
    auto &scope = scopes_[0];
    int x = scope.getX();
    int y = scope.getY();
    int w = scope.getWidth();
    int h = scope.getHeight();

    base_type::paint(g);

    g.setColour(Colours::grey);
    g.drawRect(x - 5, y - 5, w + 10, h + 10);
}

void PluginEditor::resized() {
    static constexpr int x = 10, y = 40, w = 900 - 2 * x, h = 400 - 2 * y;

    scopes_[0].setBounds(x, y, w, h);
    scopes_[1].setBounds(x, y, w, h);
    scopes_[2].setBounds(x, y, w, h);
    scopes_[3].setBounds(x, y, w, h);
}