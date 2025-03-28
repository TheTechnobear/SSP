#include "OptionView.h"



OptionView::OptionView(PluginProcessor &p) : base_type(&p, nullptr), processor_(p) {
    addButton(0, std::make_shared<ssp::ValueButton>("Load Pr", [&](bool b) {}));
    addButton(1, std::make_shared<ssp::ValueButton>("Save Pr", [&](bool b) {}));
}

OptionView::~OptionView() {
}


void OptionView::drawView(Graphics &g) {
    base_type::drawView(g);
    int x = 30;
    int y = 30;

    g.setColour(Colours::yellow);
    static constexpr unsigned fh = 14 * COMPACT_UI_SCALE;
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain)));

    g.drawSingleLineText("Settings", x, y);
}
