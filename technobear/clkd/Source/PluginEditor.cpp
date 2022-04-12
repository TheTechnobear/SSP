
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/ParamControl.h"
#include "ssp/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p), processor_(p),
      runButton_("Run", nullptr, 32, Colours::green),
      resetButton_("Reset", nullptr, 32, Colours::yellow),
      useTrigsButton_("TrigSync", nullptr, 20, Colours::cyan) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.source, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.clkindiv, 1.0f, 0.25f),
        std::make_shared<pcontrol_type>(processor_.params_.bpm, 10.0f, 0.1f),
        std::make_shared<pcontrol_type>(processor_.params_.midippqn, 1.0f, 1.0f)
    );

    unsigned d = 0;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 0]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 1]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 2]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 3]->val, 1, 0.25)
    );
    d += 4;
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 0]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 1]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 2]->val, 1, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.divisions_[d + 3]->val, 1, 0.25)
    );

    // add some buttons
    setButtonBounds(runButton_, 0, 0);
    setButtonBounds(resetButton_, 0, 1);
    setButtonBounds(useTrigsButton_, 1, 0);

    addAndMakeVisible(runButton_);
    addAndMakeVisible(resetButton_);
    addAndMakeVisible(useTrigsButton_);

    setSize(1600, 480);
}

void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);

    static String runLabel = "Run";
    static String stopLabel = "Stop";
    bool runState = processor_.isRunning();
    bool clockStates[PluginProcessor::MAX_CLK_OUT];
    bool useTrigs = processor_.isUsingTrigs();
    processor_.getClockStates(clockStates);

    useTrigsButton_.value(useTrigs);

    if (runState) {
        runButton_.label(stopLabel);
    } else {
        runButton_.label(runLabel);
    }

    const int tstartX = 1250, lstartX = tstartX + 150, startX = lstartX + 30;
    const int startY = 80;
    const int sp = 10;
    const int gw = 25;

    g.setFont(30);
    static const char *running = "Running";
    static const char *stopped = "Stopped";
    const char *runTxt;
    if (runState) {
        runTxt = running;
        g.setColour(Colours::green);
    } else {
        runTxt = stopped;
        g.setColour(Colours::red);
    }
    g.drawText(runTxt, tstartX, startY - (sp / 2), 150, gw + sp, Justification::centredLeft);

    g.setFont(20);
    // draw current values
    for (int i = 0; i < PluginProcessor::MAX_CLK_OUT; i++) {
        bool gate = clockStates[i];
        int y = startY + ((sp + gw) * i);

        char lbl[3] = "G1";
        lbl[1] = '1' + i;
        g.drawText(lbl, lstartX, y, 30, gw + (sp / 2), Justification::centredLeft);
        if (gate) {
            g.fillRect(startX + 10, y, gw, gw);
        } else {
            g.drawRect(startX + 10, y, gw, gw);
        }
    }
}

void PluginEditor::onButton(unsigned btn, bool v) {
    base_type::onButton(btn, v);
    if (!v) { // on release
        switch (btn) {
            case BN_RUN: {
                processor_.toggleRunRequest();
                break;
            }
            case BN_RESET: {
                processor_.requestReset();
                break;
            }
            case BN_USETRIGS: {
                processor_.toggleUseTrigs();
                break;
            }
        }
    }
//    Logger::writeToLog(String::formatted("ClkD onButton %d -  %d ", btn,v));
}
