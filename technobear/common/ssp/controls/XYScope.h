#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

namespace ssp {

class XYScope : public juce::Component {
public:
    XYScope(bool grid = true, bool legend = false) : grid_(grid), legend_(legend) {
    }

    void init(const std::string &l1, float *buf1, unsigned bufn1, const std::string &l2, float *buf2, unsigned bufn2, unsigned n, juce::Colour clr) {
        n_ = n;
        colour_ = clr;
        label_[0] = l1;
        buffer_[0] = buf1;
        label_[1] = l2;
        buffer_[1] = buf2;
        bufN_ = bufn1 < bufn2 ? bufn1 : bufn2;
    }

    void scaleOffset(unsigned sigN, float s, float o) {
        scale_[sigN] = s;
        offset_[sigN] = o;
    }

    void pos(unsigned pos) {
        pos_ = pos;
    }

    void paint(juce::Graphics &g) {
        if (grid_) drawGrid(g);
        if (visible_) paintSignal(g);
    }


private:
    juce_UseDebuggingNewOperator
    static constexpr unsigned N = 2;
    bool grid_ = false;
    bool legend_ = false;
    juce::Colour colour_;
    bool visible_ = true;
    unsigned n_ = 0;
    unsigned pos_ = 0;
    unsigned bufN_ = 0;

    std::string label_[N];
    float *buffer_[N] = {nullptr, nullptr};
    float scale_[N] = {1.0f, 1.0f};
    float offset_[N] = {0.0f, 0.0f};

///// implementations
    inline float constrain(float v, float vMin, float vMax) {
        return std::max<float>(vMin, std::min<float>(vMax, v));
    }


    void paintSignal(juce::Graphics &g) {
        float w = (float) getWidth();
        float h = (float) getHeight();


        for (unsigned sigN = 0; sigN < N; sigN++) {
            if (legend_) {
                static constexpr unsigned fh = 8 * COMPACT_UI_SCALE;
                Font f(Font::getDefaultMonospacedFontName(), fh, Font::plain);
                g.setFont(f);
                g.setColour(colour_);

                if (!label_[sigN].empty()) g.drawText(label_[sigN], 0, 16 + (sigN * (fh + 10)), w, 24, Justification::left);
            }
        }

        if (buffer_[0] == nullptr || buffer_[1] == nullptr) return;

        // draw scope
        g.setColour(colour_);
        float valX = 0.0f, x = 0.0f, valY = 0.0f, y = 0.0f;
        float lastX = 0.0f, lastY = 0.5f * h;

        for (int t = 0; t < n_; t++) {
            unsigned idx = (t + pos_) % bufN_;
            valX = constrain(buffer_[0][idx] * scale_[0] + offset_[0], -1.0f, 1.0f);
            valY = constrain(buffer_[1][idx] * scale_[1] + offset_[1], -1.0f, 1.0f);

            x = (1.0f - (valX + 1.0f) * 0.5f) * w;
            y = (1.0f - (valY + 1.0f) * 0.5f) * h;
            if (t > 0) {
                g.drawLine(lastX, lastY, x, y, 2.0f);
            }
            lastX = x;
            lastY = y;
        }
    }


    void drawGrid(juce::Graphics &g) {
        int x = 0;
        int y = 0;
        int w = getWidth();
        int h = getHeight();

        g.setColour(juce::Colours::darkgrey);
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