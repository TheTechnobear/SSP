#include "TrackView.h"

TrackView::TrackView(PluginProcessor &p) : base_type(&p, nullptr), processor_(p) {
    for (int i = Track::M_SLOT_1; i <= Track::M_SLOT_4; i++) {
        auto pbtn = std::make_shared<ssp::ValueButton>("Edit ", [&](bool b) {});
        addButton(i - Track::M_SLOT_1, pbtn);


        auto sbtn = std::make_shared<ssp::ValueButton>("View", [&](bool b) {});
        addButton(i + Track::M_SLOT_4 - 1, sbtn);
    }

    for (int i = 0; i < Track::MAX_MODULES; i++) {
        auto module = std::make_shared<ModuleComponent>(processor_, 0, i);
        modules_.push_back(module);
        addAndMakeVisible(module.get());
    }
}

TrackView::~TrackView() {
}


void TrackView::resized() {
    base_type::resized();
    int gap = 10 * COMPACT_UI_SCALE;
    int h = canvasHeight() - gap;
    int mh = (h / 3) - gap;
    int x = canvasX();

    for (int i = 0; i < Track::MAX_MODULES; i++) {
        auto module = modules_[i];
        switch (i) {
            case 0: {
                int y = canvasY() + gap;
                int w = canvasWidth();
                module->setBounds(x, y, w, mh);
                break;
            }
            case Track::MAX_MODULES - 1: {
                int y = canvasY() + h  - mh ;
                int w = canvasWidth();
                module->setBounds(x, y, w, mh);
                break;
            }
            default: {
                int y = canvasY() + mh + 2 * gap;
                int fw = canvasWidth() - (PluginProcessor::MAX_TRACKS - 1) * gap;
                int w = fw / PluginProcessor::MAX_TRACKS;
                module->setBounds(x + ((gap + w) * (i - 1)),  y , w, mh);
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
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Track " + String(trackIdx_ + 1), x, y);
}


