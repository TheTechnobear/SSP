#include "TrackView.h"

TrackView::TrackView(PluginProcessor &p) : base_type(&p, nullptr), processor_(p) {
    for (int i = Track::M_SLOT_1; i <= Track::M_SLOT_4; i++) {
        auto pbtn = std::make_shared<ssp::ValueButton>("Load ", [&](bool b) {});
        addButton(i - Track::M_SLOT_1, pbtn);

        auto sbtn = std::make_shared<ssp::ValueButton>("Route", [&](bool b) {});
        addButton(i + Track::M_SLOT_4 - 1, sbtn);
    }

}

TrackView::~TrackView() {
}


void TrackView::drawView(Graphics &g) {
    base_type::drawView(g);
    int space = 40 * COMPACT_UI_SCALE;
    int fh = 10 * COMPACT_UI_SCALE;
    int x = 15 * COMPACT_UI_SCALE;
    int y = 15 * COMPACT_UI_SCALE;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("Track " + String(trackIdx_ + 1), x, y);

    g.setColour(Colours::white);
    for (int i = Track::M_SLOT_1; i <= Track::M_SLOT_4; i++) {
        y += 30;
        String mod = String("Slot ") + String(i);
        g.drawSingleLineText(mod, x, y);
        g.drawSingleLineText(String(" : ") + processor_.getLoadedPlugin(trackIdx_, i), x + space, y);
    }
}
