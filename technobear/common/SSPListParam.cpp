
#include "SSPListParam.h"

#include <iostream>

static std::string nullString= "undefined";

void SSPListParam::paint(Graphics &g) {
    unsigned h = getHeight();
    unsigned w = getWidth();

    const std::string&  val=fmtValue();

    static constexpr unsigned fh = 36;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    if (active_) {
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::grey);
    }

    g.drawText(label_, 0 , 0 , w, fh, Justification::centred);
    g.drawText(val.c_str(), 0, h / 2, w, fh, Justification::centred);
}


const std::string& SSPListParam::fmtValue() {
    std::string&  val=nullString;
    if(v_ < list_.size()) val=list_[v_];
    return val;
}


