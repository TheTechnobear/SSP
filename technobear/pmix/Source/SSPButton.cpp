
#include "SSPButton.h"

void SSPButton::paint(Graphics &g) {
    const int w = getWidth();
    const int h = getHeight();
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 36, Font::plain));


    if (!active_) {
        g.setColour(Colours::black);
        g.fillRect (0, 0 + 1, w - 2, h - 2);
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::red);
        g.fillRect (0, 0 + 1, w - 2, h - 2);
        g.setColour(Colours::black);
    }

    g.drawText(label_, 0, 0, w, h, Justification::centred);
}

