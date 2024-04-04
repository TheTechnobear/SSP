#include "TrackView.h"

TrackView::TrackView(PluginProcessor &p) : ssp::MiniBasicView(&p, nullptr), processor_(p) {
    addButton(0, std::make_shared<ssp::ValueButton>("Mod", [&](bool b) {}));
    addButton(1, std::make_shared<ssp::ValueButton>("User 1", [&](bool b) {}));
    addButton(2, std::make_shared<ssp::ValueButton>("User 2", [&](bool b) {}));
    addButton(3, std::make_shared<ssp::ValueButton>("User 3", [&](bool b) {}));
    addButton(4, std::make_shared<ssp::ValueButton>("Matrix", [&](bool b) {}));
    addButton(5, std::make_shared<ssp::ValueButton>("Add Route", [&](bool b) {}));
}

TrackView::~TrackView() {
}


void TrackView::drawView(Graphics &g) {
    MiniBasicView::drawView(g);
    int x = 30;
    int y = 30;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));

    g.drawSingleLineText("Track " + String(trackIdx_ + 1), x, y);

    g.setColour(Colours::white);
    for (int i = Track::M_MOD; i <= Track::M_USER_3; i++) {
        y += 30;
        String mod;
        switch (i) {
            case Track::M_MOD: mod = "Mod"; break;
            case Track::M_USER_1: mod = "User 1"; break;
            case Track::M_USER_2: mod = "User 2"; break;
            case Track::M_USER_3: mod = "User 3"; break;
        }
        g.drawSingleLineText(mod, x, y);
        g.drawSingleLineText(String(" : ") + processor_.getLoadedPlugin(trackIdx_, i), x + 150, y);
    }
}
