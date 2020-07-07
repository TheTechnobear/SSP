
#include "SSPParam.h"

#include <iostream>
void SSPParam::paint(Graphics &g) {
    unsigned h=getHeight();
    unsigned w=getWidth();

    String val = String::formatted(fmt_, v_);

    static constexpr unsigned fh = 36;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    if(active_) {
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::grey);
    }
    if(unit_.length()) { val = val + " " +unit_;}

    g.drawText(label_, 0 , 0 , w, fh, Justification::centred);
    g.drawText(val, 0, h/2, w, fh, Justification::centred);
}

