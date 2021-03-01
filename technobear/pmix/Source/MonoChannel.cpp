
#include "SSPMonoChannel.h"

SSPMonoChannel::SSPMonoChannel() {
    addAndMakeVisible(channel_);
}

void SSPMonoChannel::paint (Graphics& g)  {
    static constexpr int fh = 16;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    if (enabled()) {
        g.setColour(Colours::red);
    } else {
        g.setColour(Colours::lightgrey);
    }
    g.drawText(label_, 0 , 0 , w, fh, Justification::centred);

    int tbh = h - (fh * 10);
    int barbase = tbh + (2 * fh);
    int bw = w / 2;
    int bx = w / 4;


    bool  mute = data_->mute_;
    bool  solo = data_->solo_;
    float pan = data_->pan_;
    float lvl = data_->lvl_;

    // channel_.paint(g);


    int y = barbase;
    y += 5;

    g.setColour(Colours::lightgrey);
    float dbS = lvl > 0.0f ? std::log10(lvl) * 20.0f : -200.f;
    if (dbS > -100.0f) {
        String val = String::formatted("%4.2f", dbS);
        g.drawText(val, 0 , y  , w, fh, Justification::centred);
    } else {
        g.drawText("Inf", 0 , y  , w, fh, Justification::centred);
    }

    y += 2 * fh;

    // pan
    g.setColour(Colours::lightgrey);
    g.drawLine(bx, y, bx + bw, y, 2);
    unsigned poff = unsigned( float(bw) * ((pan + 1.0) / 2.0));
    g.setColour(Colours::white);
    g.fillRect(bx + poff - 5, y - 5, 10, 10);

    // solo
    y += 2 * fh;
    g.setColour(Colours::lightgrey);
    g.drawRect(bx - 1 , y - 1 , bw + 2 , fh + 2);
    if (solo) {
        g.setColour(Colours::yellow);
        g.fillRect(bx , y  , bw , fh );
        g.setColour(Colours::black);
    }
    g.drawText("S", bx , y , bw, fh, Justification::centred);

    // mute
    y += 2 * fh;
    g.setColour(Colours::lightgrey);
    g.drawRect(bx - 1 , y - 1 , bw + 2 , fh + 2);
    if (mute) {
        g.setColour(Colours::red);
        g.fillRect(bx , y , bw , fh );
        g.setColour(Colours::black);
    }
    g.drawText("M", bx , y, bw, fh, Justification::centred);
}

void SSPMonoChannel::resized()  {
    int w = getWidth();
    int bw = w / 2;
    int bx = w / 4;
    channel_.setBounds(bx, 0, bw, getHeight());
}
