#include "MixerView.h"


MixerView::MixerView(PluginProcessor &p) : ssp::MiniBasicView(&p, nullptr), processor_(p) {
    for (int t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        vuMeters_[t].init("Track " + String(t + 1), false);
        addAndMakeVisible(vuMeters_[t]);
    }
}

MixerView::~MixerView() {
}

void MixerView::resized() {
    // 320 x 240
    int x = 10 * COMPACT_UI_SCALE;
    int y = (10 * COMPACT_UI_SCALE) * 2;

    int h = 150 * COMPACT_UI_SCALE;
    int w = 65 * COMPACT_UI_SCALE;
    int offset = 5 * COMPACT_UI_SCALE;

    for (int t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        vuMeters_[t].setBounds(x + (offset + w + offset) * t, y, w, h);
    }
}

void MixerView::drawView(Graphics &g) {
    ssp::MiniBasicView::drawView(g);
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
    g.drawSingleLineText("Trax", 10 * COMPACT_UI_SCALE, 10 * COMPACT_UI_SCALE);
}


void MixerView::onSSPTimer() {
    for (int t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        float lLevel = 0.f, rLevel = 0.f;
        processor_.rmsLevels(t, lLevel, rLevel);
        vuMeters_[t].level(lLevel, rLevel);
        vuMeters_[t].repaint();
    }
}