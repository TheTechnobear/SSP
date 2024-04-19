#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

namespace ssp {

template<unsigned N>
class LineScope : public juce::Component {
public:
    LineScope(bool grid = true, bool legend = false) : grid_(grid), legend_(legend) {
        juce::Colour clrs[4]{Colours::green, Colours::blue, Colours::red, Colours::yellow};
        for (unsigned i = 0; i < N; i++) {
            visible_[i] = true;
            scale_[i] = 1.0f;
            offset_[i] = 0.0f;
            pos_[i] = 0;
            colour_[i] = clrs[i % 4];
            buffer_[i] = nullptr;
        }
    }

    void initSignal(unsigned sigN, const std::string &label, float *buf, unsigned bufn, unsigned n, Colour c = juce::Colours::red) {
        label_[sigN] = label;
        buffer_[sigN] = buf;
        bufN_[sigN] = bufn;
        n_[sigN] = n;
        colour_[sigN] = c;
    }

    void scaleOffset(unsigned sigN, float s, float o) {
        scale_[sigN] = s;
        offset_[sigN] = o;
    }

    void pos(unsigned sigN, unsigned pos) {
        pos_[sigN] = pos;
    }

    void signalVisible(unsigned sigN, bool v) {
        visible_[sigN] = v;
    }

    void paint(Graphics &g) {

        if (grid_) drawGrid(g);

        for (int sigN = 0; sigN < N; sigN++) {
            if (visible_[sigN]) paintSignal(g, sigN);
        }
    }

private:

    juce_UseDebuggingNewOperator
    bool grid_ = false;
    bool legend_ = false;

    juce::Colour colour_[N];
    std::string label_[N];
    float *buffer_[N];
    unsigned bufN_[N];
    unsigned n_[N];
    unsigned pos_[N];
    bool visible_[N];
    float scale_[N];
    float offset_[N];

///// implementations
    inline float constrain(float v, float vMin, float vMax) {
        return std::max<float>(vMin, std::min<float>(vMax, v));
    }


    void paintSignal(Graphics &g, unsigned sigN) {
        float w = (float) getWidth();
        float h = (float) getHeight();


        if (legend_) {
            static constexpr unsigned fh = 8 * COMPACT_UI_SCALE;
            Font f(Font::getDefaultMonospacedFontName(), fh, Font::plain);
            g.setFont(f);
            g.setColour(colour_[sigN]);

            if (!label_[sigN].empty()) g.drawText(label_[sigN], 0, 16 + (sigN * (fh + 10)), w, 24, Justification::left);
        }

        if (!buffer_[sigN]) return;

        // draw scope
        g.setColour(colour_[sigN]);
        float val = 0.00f, y = 0.0f, x = 0.0f;
        float lastX = 0.0f, lastY = 0.5f * h;
        float stepW = (float) w / (float) n_[sigN];
        for (int t = 0; t < n_[sigN]; t++) {
            unsigned idx = (t + pos_[sigN]) % bufN_[sigN];
            val = constrain(buffer_[sigN][idx] * scale_[sigN] + offset_[sigN], -1.0f, 1.0f);

            y = (1.0f - (val + 1.0f) * 0.5f) * h;
            x = t * stepW;
            if (t > 0) {
                g.drawLine(lastX, lastY, x, y, 2.0f);
            }
            lastX = x;
            lastY = y;
        }
    }


    void drawGrid(Graphics &g) {
        int x = 0;
        int y = 0;
        int w = getWidth();
        int h = getHeight();

        g.setColour(Colours::darkgrey);
        unsigned div = 10;
        int sw = w / div;
        int sh = h / div;
        for (unsigned i = 0; i < 10; i++) {
            g.fillRect(x + (i * sw), y, 1, h); //vert
            g.fillRect(x, y + (i * sh), w, i != 5 ? 1 : 3); //horz
        }
        g.fillRect(w - 1, y, 1, h);
        g.fillRect(x, h - 1, w, 1);
    }
};

}