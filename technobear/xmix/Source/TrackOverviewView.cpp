
#include "TrackOverviewView.h"

TrackOverviewView::TrackOverviewView(PluginProcessor &p) : ssp::BaseView(&p, false), processor_(p) {

}

TrackOverviewView::~TrackOverviewView() {

}


void TrackOverviewView::drawView(Graphics &g) {
        int x = pluginWidth / 2;
        g.setColour(Colours::white);
        g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
        g.drawSingleLineText("TrackOverviewView", x - 40, getHeight() / 2 - 20);

}
