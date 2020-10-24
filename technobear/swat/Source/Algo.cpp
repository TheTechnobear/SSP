#include "Algo.h"

#include "../JuceLibraryCode/JuceHeader.h"

// Helper /////////////////////////////////////////////////////////////////////
void drawAB(Graphics& g, float A, float B) {
    unsigned space = 32;
    unsigned fh = 32;
    unsigned x = space;
    unsigned y = 100;
    g.setColour(Colours::white);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.drawSingleLineText("A : " + String(A), x, y);
    y += space * 2;
    g.drawSingleLineText("B : " + String(B), x, y);
}


//Algo

double Algo::sampleRate_ = 48000.0f;


void Algo::paint (Graphics& g) {
    drawHelp(g);
}

void Algo::encoder(unsigned enc, int dir) {
    if (enc < params_.size()) {
        if (dir > 0.0f) {
            params_[enc]->inc();
        } else if (dir < 0.0f) {
            params_[enc]->dec();
        }
    }
}

void Algo::button(unsigned btn, bool state) {

}

void Algo::encswitch(unsigned enc, bool state) {
    if (enc < params_.size()) {
        if (state < 0.5f) {
            params_[enc]->reset();
        }
    }
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

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    std::string str = description();
    while ((pos = str.find("\n", prev)) != std::string::npos)
    {
        g.drawSingleLineText(str.substr(prev, pos - prev), x, y);
        y += space;
        prev = pos + 1;
    }
    g.drawSingleLineText(str.substr(prev), x, y);
    y += space;

    x = x + 300;
    y = yText;

    unsigned enc = 1;
    for (auto p : params_) {
        String desc;
        if (p->desc().length() > 0 ) desc = " - " + p->desc();
        g.drawSingleLineText("Enc " + String(enc) + " : " + p->name() + desc, x, y); y += space;
        enc++;
    }
}


void Algo::writeToXml(juce::XmlElement& xml) {
    for (auto p : params_) {
        xml.setAttribute(p->name().c_str(), double(p->floatVal()));
    }
}

void Algo::readFromXml(juce::XmlElement& xml) {
    for (auto p : params_) {
        if (xml.hasAttribute(p->name().c_str())) {
            p->floatVal(xml.getDoubleAttribute(p->name().c_str(), 0.0f));
        } else {
            p->reset();
        }
    }
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


//AgIntParam
void AgIntParam::inc() {
    val_ = constrain(val_ + step_, min_, max_);
}

void AgIntParam::dec() {
    val_ = constrain(val_ - step_, min_, max_);
}

void AgIntParam::reset() {
    val_ = constrain(init_, min_, max_);
}
