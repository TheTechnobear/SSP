
#include "TrackOverviewView.h"

TrackOverviewView::TrackOverviewView(PluginProcessor &p) : ssp::MiniBasicView(&p, nullptr), processor_(p) {
    addButton(0, std::make_shared<ValueButton>("Mod", [&](bool b) { onModBtn(0, b); }));
    addButton(1, std::make_shared<ValueButton>("Pre", [&](bool b) { onModBtn(1, b); }));
    addButton(2, std::make_shared<ValueButton>("Main", [&](bool b) { onModBtn(2, b); }));
    addButton(3, std::make_shared<ValueButton>("Post", [&](bool b) { onModBtn(3, b); }));
}

TrackOverviewView::~TrackOverviewView() {
}


void TrackOverviewView::drawView(Graphics &g) {
    MiniBasicView::drawView(g);
    int x = 30;
    int y = 30;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));

    g.drawSingleLineText("Track " + String(trackIdx_ + 1), x, y);

    g.setColour(Colours::white);
    for (int i = 0; i < Track::M_MAX; i++) {
        y += 30;
        String mod;
        switch (i) {
            case 0: mod = " (Mod) "; break;
            case 1: mod = " (Pre) "; break;
            case 2: mod = " (Main) "; break;
            case 3: mod = " (Post) "; break;
        }
        g.drawSingleLineText("Module " + mod, x, y);
        g.drawSingleLineText(String(" : ") + processor_.getLoadedPlugin(trackIdx_, i), x + 150, y);
    }
}
