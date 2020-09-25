#include "Algo.h"

#include "../JuceLibraryCode/JuceHeader.h"


//Algo
void Algo::paint (Graphics& g) {
    drawHelp(g);
}

void Algo::encoder(unsigned, int) {

}

void Algo::button(unsigned, bool) {

}

void Algo::encswitch(unsigned enc, bool state) {
    
}


void Algo::drawHelp(Graphics& g) {
    unsigned x = 900;
    unsigned y = 40;
    unsigned space = 30;
    unsigned yText = y + space;
    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 40, Font::plain));
    g.drawSingleLineText(name(), x, y);

    y = yText;
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 18, Font::plain));
    g.setColour(Colours::white);

    std::string delimiter = "\n";
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    std::string str=description();
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        g.drawSingleLineText(str.substr(prev, pos - prev), x, y);
        y += space;
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    g.drawSingleLineText(str.substr(prev), x, y);
    y += space;

    // x = x + 300;
    // y = yText;
    // g.drawSingleLineText("column 2", x, y);        y += space;
}


//AgFloatParam
void AgFloatParam::inc() {
    val_ = constrain(val_ + step_, min_, max_);
}

void AgFloatParam::dec() {
    val_ = constrain(val_ - step_, min_, max_);
}

void AgFloatParam::reset() {
    val_ = constrain(init_, min_, max_);
}
