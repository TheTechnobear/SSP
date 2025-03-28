
#include "PluginProcessor.h"
#include "PluginMiniEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


PluginMiniEditor::PluginMiniEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p) {

    for (unsigned p = 0; p < PluginProcessor::MAX_SIG_OUT; p++) {
        addParamPage(
            std::make_shared<pcontrol_type>(processor_.params_.vca[p][0],0.1f),
            std::make_shared<pcontrol_type>(processor_.params_.vca[p][1],0.1f),
            std::make_shared<pcontrol_type>(processor_.params_.vca[p][2],0.1f),
            std::make_shared<pcontrol_type>(processor_.params_.vca[p][3],0.1f)
        );
    }
}

void PluginMiniEditor::drawGrid(Graphics &g) {
    int startX = canvasX() + 40 * COMPACT_UI_SCALE;
    int y = canvasY(); 
    int fh = 9 * COMPACT_UI_SCALE;
    int lsz = 35 * COMPACT_UI_SCALE;

    int szx = fh * 3;
    int szy = szx;
    int sp = fh;

    int x = startX;

    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, Font::plain)));
    g.setColour(Colours::red);

    x += sp + (szx * 2.5f);
    g.drawText("Outputs", x, y, lsz, fh, Justification::left);

    x = startX;
    y += sp;


    // draw header
    g.drawText("Inputs", x, y, lsz, fh, Justification::left);
    x += lsz + sp;
    for (unsigned xi = 0; xi < PluginProcessor::MAX_SIG_OUT; xi++) {
        if (processor_.isOutputEnabled(xi * 2) + processor_.isOutputEnabled(xi * 2 + 1)) {
            g.setColour(Colours::red);
        } else {
            g.setColour(Colours::grey);
        }
        char chout[2] = "A";
        chout[0] = 'A' + xi;
        g.drawText(String(chout), x, y, szx, fh, Justification::centred);
        x += szx;
    }


    y += szy;
    // draw each input line
    for (unsigned yi = 0; yi < PluginProcessor::MAX_SIG_IN; yi++) {
        x = startX;

        if (processor_.isInputEnabled(yi * 2) || processor_.isInputEnabled(yi * 2 + 1)) {
            g.setColour(Colours::red);
        } else {
            g.setColour(Colours::grey);
        }
        g.drawText(String(yi + 1), x, (y - (fh / 2)), lsz, fh, Justification::left);
        x += lsz + sp;

        for (unsigned xi = 0; xi < PluginProcessor::MAX_SIG_OUT; xi++) {
            float vca = processor_.getVCA(yi, xi) + processor_.getVCACV(yi, xi);
            vca = constrain(vca / 1.2f, -1.0f, 1.0f);

            if (vca == 0.0f) {
                unsigned d = 5;
                unsigned o = ((szy - d) / 2);
                unsigned cy = y - (szy / 2) + o;
                unsigned cx = x + o;
                g.setColour(Colours::white);
                g.fillEllipse(cx, cy, d, d);
                // g.drawEllipse(cx, cy, d, d, 1);
            } else {
                unsigned d = szy;
                if (vca > 0.0f) {
                    d = vca * szy;
                    g.setColour(Colours::green);

                } else {
                    d = vca * -szy;
                    g.setColour(Colours::red);
                }
                unsigned o = ((szy - d) / 2);
                unsigned cy = y - (szy / 2) + o;
                unsigned cx = x + o;
                g.fillEllipse(cx, cy, d, d);
            }
            x += szx;

            // String v = "-";
            // if (vca <= -0.01f || vca >= 0.01f) v = String::formatted("%4.2f", vca);
            // g.drawText(v, x + lx + xi * (szx + sp), y , sp + szx, fh, Justification::left);
        }
        y += szy;
    }
}


void PluginMiniEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    drawGrid(g);
}


void PluginMiniEditor::resized() {
    base_type::resized();
}


