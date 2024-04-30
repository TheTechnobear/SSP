#include "MixerView.h"


MixerView::MixerView(PluginProcessor& p) : base_type(&p, nullptr), processor_(p) {
    for (int t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        auto& meter = vuMeters_[t];
        meter.init("Track " + String(t + 1), true);
        meter.enabled(!processor_.muteTrack(t));
        addAndMakeVisible(meter);
    }

    for (unsigned t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        auto tracksel = std::make_shared<ssp::ValueButton>("Track", [this, t](bool v) {});
        addButton(t, tracksel);

        auto mutebtn = std::make_shared<ssp::ValueButton>("Mute", [this, t](bool v) {
            processor_.muteTrack(t, v);
            bool muted = processor_.muteTrack(t);
            auto& meter = vuMeters_[t];
            meter.enabled(!muted);
        });
        mutebtn->value(processor_.muteTrack(t));
        mutebtn->setToggle(true);
        addButton(t + PluginProcessor::MAX_TRACKS, mutebtn);
    }
}

MixerView::~MixerView() {
}

void MixerView::resized() {
    base_type::resized();
    int gap = 2 * COMPACT_UI_SCALE;
    int h = canvasHeight() - 4 * gap;
    int gw = canvasWidth() / PluginProcessor::MAX_TRACKS;
    int w =  gw - 2* gap;

    int x = canvasX();
    int y = canvasY() + 2 * gap;

    for (int t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        vuMeters_[t].setBounds(x + (gw * t) + gap, y, w, h);
    }
}

void MixerView::editorShown() {
    base_type::editorShown();
    for (int t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        bool muted = processor_.muteTrack(t);
        auto btn = getButton(t);
        if (btn != nullptr) { btn->value(muted); }

        auto& meter = vuMeters_[t];
        meter.enabled(!muted);
    }
}

void MixerView::drawView(Graphics& g) {
    base_type::drawView(g);
    static constexpr unsigned fh = 14 * COMPACT_UI_SCALE;
    g.setColour(Colours::yellow);

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    g.drawSingleLineText("Trax", (SSP_COMPACT_WIDTH / 2) - (1.5f * float(fh)), COMPACT_UI_SCALE + fh);
}


void MixerView::onSSPTimer() {
    base_type::onSSPTimer();
    for (unsigned t = 0; t < PluginProcessor::MAX_TRACKS; t++) {
        float lLevel = 0.f, rLevel = 0.f;
        float gainLevel = processor_.gainTrack(t);
        processor_.rmsLevels(t, lLevel, rLevel);
        vuMeters_[t].level(lLevel, rLevel);
        vuMeters_[t].gainLevel(gainLevel, gainLevel);
        vuMeters_[t].repaint();
    }
}

void MixerView::onEncoder(unsigned enc, float diff) {
    unsigned trackIdx = enc;
    float diffGain = diff * (0.1f * (encDown_[enc] ? 0.1f : 1.0f));
    float newGain = processor_.gainTrack(trackIdx) + diffGain;
    newGain = std::max(std::min(2.0f, newGain), 0.0f);
    processor_.gainTrack(trackIdx, newGain);
    encFine_[enc] = encDown_[enc];
}

void MixerView::onEncoderSwitch(unsigned enc, bool v) {
    unsigned trackIdx = enc;
    float newGain = 1.0f;
    if (!v && !encFine_[enc]) processor_.gainTrack(trackIdx, newGain);
    encDown_[enc] = v;
    encFine_[enc] = false;
}
