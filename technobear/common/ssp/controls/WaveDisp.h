#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

namespace ssp {

template<unsigned N>
class WaveDisp : public juce::Component {
public:
    WaveDisp(bool grid = true, bool legend = false) : grid_(grid), legend_(legend) {
        juce::Colour clrs[4]{Colours::green, Colours::blue, Colours::red, Colours::yellow};
        for (unsigned i = 0; i < N; i++) {
            auto &layer = layer_[i];
            layer.visible_ = true;
            layer.scale_ = 1.0f;
            layer.offset_ = 0.0f;
            layer.pos_ = 0;
            layer.colour_ = clrs[i % 4];
            layer.buffer_ = nullptr;
        }
    }

    void initSignal(unsigned sigN, const std::string &label, float *buf, unsigned bufn, unsigned n, Colour c = juce::Colours::red) {
        auto &layer = layer_[sigN];
        layer.label_ = label;
        layer.buffer_ = buf;
        layer.bufN_ = bufn;
        layer.n_ = n;
        layer.colour_ = c;
    }

    void label(unsigned sigN,const std::string &str) {
        auto &layer = layer_[sigN];
        layer.label_=str;
    }

    void scaleOffset(unsigned sigN, float s, float o) {
        auto &layer = layer_[sigN];
        layer.scale_ = s;
        layer.offset_ = o;
    }

    void pos(unsigned sigN, unsigned pos) {
        auto &layer = layer_[sigN];
        layer.pos_ = pos;
    }

    void signalVisible(unsigned sigN, bool v) {
        auto &layer = layer_[sigN];
        layer.visible_ = v;
    }

    void setPosition(unsigned sigN, float cur, float begin, float end) {
        auto &layer = layer_[sigN];
        layer.cur_ = cur;
        layer.begin_ = begin;
        layer.end_ = end;
    }

    void setRecPosition(unsigned sigN, bool isRec, float recPos) {
        auto &layer = layer_[sigN];
        layer.isRec_ = isRec;
        layer.recPos_ = recPos;
    }

    void paint(Graphics &g) {
        if (grid_) drawGrid(g);

        for (int sigN = 0; sigN < N; sigN++) {
            auto &layer = layer_[sigN];
            if (layer.visible_) paintSignal(g, sigN);
        }
    }

private:

    juce_UseDebuggingNewOperator
    bool grid_ = false;
    bool legend_ = false;


    struct {
        juce::Colour colour_;
        std::string label_;
        float *buffer_;
        unsigned bufN_;
        unsigned n_;
        unsigned pos_;
        bool visible_;
        float scale_;
        float offset_;

        float begin_ = 0.0f;
        float cur_ = 0.0f;
        float end_ = 1.0f;
        bool isRec_ = false;
        float recPos_ = 0.0f;
    } layer_[N];

///// implementations
    inline float constrain(float v, float vMin, float vMax) {
        return std::max<float>(vMin, std::min<float>(vMax, v));
    }


    void paintSignal(juce::Graphics &g, unsigned sigN) {
        auto &layer = layer_[sigN];
        float w = (float) getWidth();
        float h = (float) getHeight();


        if (legend_) {
            static constexpr unsigned fh = 6 * COMPACT_UI_SCALE;
            Font f(Font::getDefaultMonospacedFontName(), fh, Font::plain);
            g.setFont(f);
            g.setColour(layer.colour_);

            if (!layer.label_.empty()) g.drawText(layer.label_, 0, (sigN * (fh + 10)), w, fh, Justification::bottomLeft);
        }

        if (!layer.buffer_) return;

        float val = 0.00f, y = 0.0f, x = 0.0f;
        static constexpr float sp = 2.0f;

        // draw scope
        g.setColour(layer.colour_);
        float lastX = 0.0f, lastY = 0.5f * h;
        float stepW = (float) w / (float) layer.n_;
        float sigW = layer.bufN_ / (float) layer.n_;
        for (int t = 0; t < layer.n_; t++) {
            unsigned idx = (int(t * sigW) + layer.pos_) % layer.bufN_;
            val = constrain(layer.buffer_[idx] * layer.scale_ + layer.offset_, -1.0f, 1.0f);

            y = (1.0f - (val + 1.0f) * 0.5f) * h;
            x = t * stepW;
            if (t > 0) {
                g.drawLine(lastX, lastY, x, y, 2.0f);
            }
            lastX = x;
            lastY = y;
        }

        g.setColour(Colours::green);
        x = layer.begin_ * (w - sp);
        g.fillRect(x, sp, sp, h - 2.0f * sp);

        g.setColour(Colours::blue);
        x = layer.end_ * (w - sp);
        g.fillRect(x, sp, sp, h - 2.0f * sp);

        g.setColour(Colours::white);
        x = layer.cur_ * (w - sp);
        g.fillRect(x, sp, sp, h - 2.0f * sp);

        if (layer.isRec_) {
            g.setColour(Colours::red);
            x = layer.recPos_ * (w - sp);
            g.fillRect(x, sp, sp, h - 2.0f * sp);
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