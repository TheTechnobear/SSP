#include "ValueControl.h"

using namespace juce;

namespace ssp {


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

BaseValueControl::BaseValueControl(const std::string &name, const std::string &label,
                                   float min, float max, float def)
    : name_(name), label_(label), value_(def), min_(min), max_(max), active_(false) {

}

void BaseValueControl::set(float v) {
    float nv = constrain(v, min_, max_);
    if (nv != value_) {
        value_ = nv;
        valueChanged(nv);
    }
}


void BaseValueControl::valueChanged(float) {
    repaint();
}


SimpleValueControl::SimpleValueControl(const std::string &name, const std::string &label,
                                       std::function<void(float v)> cb,
                                       float min, float max, float def, float coarse, float fine)
    : BaseValueControl(name, label, min, max, def), callback_(cb), coarseInc_(coarse), fineInc_(fine) {
}


/// SimpleValueControl

void SimpleValueControl::inc(bool fine) {
    float inc = fine ? fineInc_ : coarseInc_;
    float nv = value_ + (inc != 0 ? inc : 0.01f);
    set(nv);
}

void SimpleValueControl::dec(bool fine) {
    float inc = fine ? fineInc_ : coarseInc_;
    float nv = value_ - (inc != 0 ? inc : 0.01f);
    set(nv);
}

void SimpleValueControl::reset() {
    set(default_);
}


void SimpleValueControl::valueChanged(float v) {
    BaseValueControl::valueChanged(v);
    if (callback_) callback_(v);
}

void SimpleValueControl::paint(Graphics &g) {
    static constexpr unsigned fh = 36;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(fg_);
    g.drawText(name_, 0, 0, w, fh, Justification::centred);

    String val = String(value_);
    if (!label_.empty()) { val = val + " " + label_; }
    g.setColour(Colours::white);
    g.drawText(val, 0, h / 2, w, fh, Justification::centred);
}


LineValueControl::LineValueControl(const std::string &name, const std::string &label,
                                   std::function<void(float v)> cb,
                                   float min, float max, float def, float coarse, float fine)
    : SimpleValueControl(name, label, cb, min, max, def, coarse, fine) {
};

void LineValueControl::paint(Graphics &g) {
    static constexpr unsigned fh = 32;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(active() ? fg_ : Colours::grey);
    g.drawText(name_, 0, 0, w, fh, Justification::centred);

    String val = String(value_);
    if (!label_.empty()) { val = val + " " + label_; }
    g.setColour(active() ? Colours::white : Colours::grey);
    g.drawText(val, 0, h / 2, w, fh, Justification::centred);
}


BarValueControl::BarValueControl(const std::string &name, const std::string &label,
                                 std::function<void(float v)> cb,
                                 float min, float max, float def, float coarse, float fine)
    : SimpleValueControl(name, label, cb, min, max, def, coarse, fine) {
};

void BarValueControl::paint(Graphics &g) {
    static constexpr unsigned fh = 28;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(active() ? fg_ : Colours::grey);
    g.drawText(name_, 0, 0, w, fh, Justification::centred);

    g.setColour(Colours::black);
    g.drawRect(0, h / 2, w, fh); // border

    g.setColour(Colour(0xFF222222));
    g.fillRect(1, (h / 2) + 1, w - 2, fh - 2); // border

    g.setColour(active() ? Colours::darkcyan : Colours::grey);

    int be = (w - 4) * value_;
    if (min_) {
        g.fillRect(2, h / 2 + 2, be, fh - 4);
    } else {
        float v = value_ - 0.5f;
        int bl = (w - 4) * v;
        int bs = (bl > 0.0f ? 0.5f : 0.5 + v) * (w - 4);
        g.fillRect(bs, int(h / 2) + 2, (bl > 0 ? bl : -bl), fh - 4);
    }
    g.setColour(Colours::white);
    g.drawVerticalLine(be + 2, int(h / 2) + 2, int(h / 2) + 2 + fh - 4);


    if (active()) {
        g.setColour(active() ? Colours::white : Colours::lightgrey);
        String val = String(value_);
        if (!label_.empty()) { val = val + " " + label_; }
        g.drawText(val, 1, (h / 2) + 1, w - 2, fh - 2, Justification::centred);
    }
}

ListValueControl::ListValueControl(const std::string &name,
                                   std::function<void(float idx, const std::string &str)> cb,
                                   std::vector<std::string> values, float def)
    : BaseValueControl(name, "", values.empty() ? 0 : 1, values.size(), values.empty() ? 0 : def),
      callback_(cb),
      values_(values) {
}

void ListValueControl::inc(bool coarse) {
    set(value_ + 1);
}

void ListValueControl::dec(bool coarse) {
    set(value_ - 1);
}

void ListValueControl::reset() {
    set(default_);
}

void ListValueControl::setValues(std::vector<std::string> &v, int selIdx) {
    values_ = v;
    min_ = 0;
    max_ = v.size() - 1;
    default_ = min_;
    if (selIdx > -1) set(selIdx);
    else set(default_);
}

void ListValueControl::valueChanged(float nv) {
    BaseValueControl::valueChanged(nv);
    std::string val;
    if (!values_.empty()) val = values_[(unsigned) nv];
    if (callback_) callback_(nv, val);
}

void ListValueControl::paint(Graphics &g) {
    static constexpr unsigned fh = 36;
    int h = getHeight();
    int w = getWidth();

    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));

    g.setColour(fg_);
    g.drawText(name_, 0, 0, w, fh, Justification::centred);

    String val;
    if (!values_.empty()) val = values_[(unsigned) value_];
    if (!label_.empty()) { val = val + " " + label_; }
    g.setColour(Colours::white);
    g.drawText(val, 0, h / 2, w, fh, Justification::centred);
}

} //namespace ssp
