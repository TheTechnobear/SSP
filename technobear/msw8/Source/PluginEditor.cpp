
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.inSel, 0.25),
        std::make_shared<pcontrol_type>(processor_.params_.outSel, 0.25),
        nullptr,
        nullptr
    );

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.active, 12 * COMPACT_UI_SCALE, Colours::lightskyblue),
        std::make_shared<bcontrol_type>(processor_.params_.soft, 12 * COMPACT_UI_SCALE, Colours::orange),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    setSize(1600, 480);
}

void PluginEditor::drawGrid(Graphics &g) {
//    int x = 40;
//    int y = 40;
    int x = 275;
    int y = 50;
    int lx = 100;
    int fh = 9 * COMPACT_UI_SCALE;
    int sz = fh;
    int sp = fh;

    unsigned insel, outsel;
    processor_.getLastSel(insel, outsel);
    Colour inclr = Colours::blue;
    Colour outclr = Colours::green;

    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), 12 * COMPACT_UI_SCALE, Font::plain)));
    g.setColour(outclr);
    g.drawText("Outputs", x + lx, y, (8 * sz) + (7 * sp), fh, Justification::centred);
    y += sp;
    g.setColour(inclr);
    g.drawText("Inputs", x, y, lx, fh, Justification::left);
    for (unsigned xi = 0; xi < PluginProcessor::MAX_SIG_OUT; xi++) {
        if (processor_.isOutputEnabled(xi + PluginProcessor::O_SIG_A)) {
            g.setColour(outclr);
        } else {
            g.setColour(Colours::grey);
        }
        char chout[2] = "A";
        chout[0] = 'A' + xi;
        g.drawText(String(chout), x + lx + xi * (sz + sp), y, sp + sz, fh, Justification::left);
    }
    y += sp;

    for (unsigned yi = 0; yi < PluginProcessor::MAX_SIG_IN; yi++) {
        if (processor_.isInputEnabled(yi + PluginProcessor::I_SIG_1)) {
            g.setColour(inclr);
        } else {
            g.setColour(Colours::grey);
        }
        g.drawText(String(yi + 1), x, y, lx, fh, Justification::left);
        for (unsigned xi = 0; xi < PluginProcessor::MAX_SIG_OUT; xi++) {
            g.setColour(Colours::white);

            if (xi == outsel && yi == insel) {
                g.fillRect(x + lx + xi * (sz + sp), y, sz, sz);
            } else {
                g.drawRect(x + lx + xi * (sz + sp), y, sz, sz, 1);
            }
        }
        y += sp + sz;
    }
}

void PluginEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    drawGrid(g);
}

