#include "SeqCellComponent.h"

juce::Colour SeqCellComponent::LAYER_COLOURS[L_CLRS] = { Colours::red, Colours::green, Colours::orange };

SeqCellComponent::SeqCellComponent(PluginProcessor::PluginParams &params, unsigned step)
    : params_(params), step_(step), activeX_(false), activeY_(false), activeC_(false) {
}

void SeqCellComponent::activeStep(unsigned xP, unsigned yP, unsigned cP) {
    activeX_ = step_ == xP;
    activeY_ = step_ == yP;
    activeC_ = step_ == cP;
}

void SeqCellComponent::paint(Graphics &g) {
    const int sp = 3, ls = 3;
    int sz = getHeight() - (2 * (sp * 2));
    int pos = 2 * sp;

    // draw active cells
    if (activeX_) {
        g.setColour(LAYER_COLOURS[0]);
        g.drawRect(pos, pos, sz, sz, ls);
        pos -= sp;
        sz += (sp * 2);
    }
    if (activeY_) {
        g.setColour(LAYER_COLOURS[1]);
        g.drawRect(pos, pos, sz, sz, ls);
        pos -= sp;
        sz += (sp * 2);
    }
    if (activeC_) {
        g.setColour(LAYER_COLOURS[2]);
        g.drawRect(pos, pos, sz, sz, ls);
    }

    int iw = getWidth() - (4 * (sp * 2));
    int isz = getHeight() - (4 * (sp * 2));
    int ih = isz / 3;
    int ix = 4 * sp;
    int iy = 4 * sp;
    int gw = 10;

    // draw cell values
    for (int i = 0; i < 3; i++) {
        auto &s = params_.layers_[i]->steps_[step_];
        float v = (s->cv.getValue() * 2.0f) - 1.0f;
        bool gate = s->gate.getValue() > 0.5;
        if (v >= 0) {
            g.setColour(Colours::green);
            g.fillRect(ix, iy + 2, int((iw - gw) * v), ih - 4);
        } else {
            g.setColour(Colours::red);
            g.fillRect(ix, iy + 2, int((iw - gw) * v * -1.0f), ih - 4);
        }
        Colour clr = LAYER_COLOURS[i];
        g.setColour(clr);
        if (gate) {
            g.fillRect(ix + (iw - gw) + 1, iy + 2, gw - 1, ih - 4);
        } else {
            g.drawRect(ix + (iw - gw) + 1, iy + 2, gw - 1, ih - 4);
        }
        iy += ih;
    }
}
