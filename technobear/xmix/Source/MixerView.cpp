#include "MixerView.h"


MixerView::MixerView(PluginProcessor &p) : ssp::BaseView(&p, false), processor_(p) {
}

MixerView::~MixerView() {
}

void MixerView::drawView(Graphics &g) {
        int x = pluginWidth / 2;
        g.setColour(Colours::white);
        g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
        g.drawSingleLineText("MixerView", x - 40, getHeight() / 2 - 20);

}
