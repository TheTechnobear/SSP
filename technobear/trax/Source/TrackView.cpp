#include "TrackView.h"

TrackView::TrackView(PluginProcessor &p) : base_type(&p, nullptr), processor_(p) {
    for (int i = Track::M_IN + 1; i < Track::M_OUT; i++) {
        auto sbtn = std::make_shared<ssp::ValueButton>(
            "Mod " + std::to_string(i), [&](bool b) {}, 16 * COMPACT_UI_SCALE, Colours::yellow);
        addButton(i - Track::M_SLOT_1, sbtn);
    }

    for (int i = 0; i < Track::MAX_MODULES; i++) {
        auto clr = i == Track::M_IN || i == Track::M_OUT ? Colours::white : Colours::yellow;
        auto module = std::make_shared<ModuleComponent>(processor_, 0, i, clr);
        modules_.push_back(module);
        addAndMakeVisible(module.get());
    }
}

TrackView::~TrackView() {
}

void TrackView::trackIdx(unsigned idx) {
    trackIdx_ = idx;
    for (int i = 0; i < Track::MAX_MODULES; i++) { modules_[i]->trackIdx(idx); }
}

void TrackView::resized() {
    base_type::resized();
    static constexpr int ModPerRow = 4;
    int gap = 10 * COMPACT_UI_SCALE;
    int h = canvasHeight() - gap;

    int mrows = (Track::MAX_USER_MODULES + ModPerRow - 1) / ModPerRow;
    int mh = (h / (mrows + 2)) - gap;

    for (int i = 0; i < Track::MAX_MODULES; i++) {
        auto module = modules_[i];
        switch (i) {
            case 0: {
                int x = canvasX();
                int y = canvasY() + gap;
                int w = canvasWidth();
                module->setBounds(x, y, w, mh);
                break;
            }
            case Track::MAX_MODULES - 1: {
                int x = canvasX();
                int y = canvasY() + h - mh;
                int w = canvasWidth();
                module->setBounds(x, y, w, mh);
                break;
            }
            default: {
                int fw = canvasWidth() - (ModPerRow - 1) * gap;
                int w = fw / ModPerRow;
                int midx = i - Track::M_SLOT_1;
                int mr = midx / ModPerRow;
                int mc = midx % ModPerRow;

                int x = canvasX() + ((gap + w) * mc);
                int y = canvasY() + ((mh + gap) * (mr + 1)) + gap;
                module->setBounds(x, y, w, mh);
            }
        }
    }
}


void TrackView::drawView(Graphics &g) {
    base_type::drawView(g);
    int fh = 16 * COMPACT_UI_SCALE;
    int space = 40 * COMPACT_UI_SCALE;
    int x = 15 * COMPACT_UI_SCALE;
    int y = 15 * COMPACT_UI_SCALE;

    g.setColour(Colours::yellow);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain)));

    g.drawSingleLineText("Track " + String(trackIdx_ + 1), x, y);
}
