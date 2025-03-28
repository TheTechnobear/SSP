#include "TextControl.h"


namespace ssp {

class TextKey : public juce::Component {
public:
    TextKey(const std::string &k, unsigned fh = 27 * COMPACT_UI_SCALE) : TextKey(k, k, fh) {}

    TextKey(const std::string &k, const std::string &kv, unsigned fh) : k_(k), fh_(fh), kv_(kv) {}

    bool active() { return active_; }

    void active(bool a) { active_ = a; }

    bool selected() { return selected_; }

    void selected(bool a) { selected_ = a; }

    void paint(juce::Graphics &g) {
        unsigned gap = 2 * COMPACT_UI_SCALE;
        g.setColour(active_ ? fg_ : bg_);
        g.fillRect(0, 0, getWidth(), getHeight());

        g.setColour(!active_ ? fg_ : bg_);
        g.drawRect(0, 0, getWidth(), getHeight(), 1 + (selected_ * 2 * COMPACT_UI_SCALE));

        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh_, juce::Font::plain)));

        g.drawText(k_, gap, gap, getWidth() - (gap * 2.f), getHeight() - (gap * 2.f), juce::Justification::centred);
    }

    std::string keyValue() { return kv_; }

private:
    std::string k_;
    std::string kv_;
    int fh_;
    juce::Colour fg_ = juce::Colours::red;
    juce::Colour bg_ = juce::Colours::black;
    bool selected_ = false;
    bool active_ = false;
};


TextControl::TextControl() {
    int w = 15 * COMPACT_UI_SCALE;
    int h = 25 * COMPACT_UI_SCALE;

    int i;
    std::string charset = "1234567890abcdefghijklmnopqrstuvwxyz-_:";
    for (i = 0; i < charset.length(); i++) {
        std::string k = charset.substr(i, 1);
        auto key = std::make_shared<TextKey>(k, h - 4);
        keys_.push_back(key);
        addAndMakeVisible(*key);
    }

    {
        auto key = std::make_shared<TextKey>("SP", " ", w / 2);
        keys_.push_back(key);
        addAndMakeVisible(*key);
    }
    keys_[0]->selected(true);
}

TextControl::~TextControl() {
}

void TextControl::resized() {
    Component::resized();
    unsigned w = 30 * COMPACT_UI_SCALE;
    unsigned h = 25 * COMPACT_UI_SCALE;

    unsigned x = 5 * COMPACT_UI_SCALE;
    unsigned y = 75 * COMPACT_UI_SCALE;

    int i = 0;
    for (auto &key : keys_) {
        float r = i / nCols_;
        float c = i % nCols_;
        key->setBounds((c * (w + COMPACT_UI_SCALE)) + x, (r * (h + COMPACT_UI_SCALE)) + y, w, h);
        i++;
    }
}


std::string TextControl::getText() {
    return text_;
}

void TextControl::setText(const std::string &txt) {
    text_ = txt;
    cursor_ = text_.length();
    repaint();
}

void TextControl::paint(juce::Graphics &g) {
    int gap = 5 * COMPACT_UI_SCALE;
    int x = gap;
    int y = gap;
    int w = getWidth() - gap * 2;
    int h = getHeight() - gap * 2;
    int fh = 18 * COMPACT_UI_SCALE;
    int smallSp = 2 * COMPACT_UI_SCALE;


    juce::Font font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),fh, juce::Font::plain));
    g.setColour(bg_);
    g.fillRect(x, y, w, h);
    g.setColour(fg_);
    g.setFont(font);
    g.drawRect(x, y, w, fh * 2);
    g.drawText(text_, x + smallSp, y, w - smallSp * 2, fh * 2, juce::Justification::left);

    static constexpr int duty = 30;
    static constexpr int halfduty = duty / 2;
    cursorFlashCounter = (cursorFlashCounter + 1) % duty;

    if (cursorFlashCounter < halfduty) {
        g.setColour(fg_);
    } else {
        g.setColour(bg_);
    }
    int fpos = juce::GlyphArrangement::getStringWidth(font,text_);
    g.fillRect(x + smallSp + fpos + smallSp, y + smallSp * 4, smallSp * 2, fh - smallSp);
}

void TextControl::onDelete() {
    if (text_.length()) {
        text_ = text_.substr(0, text_.length() - 1);
        cursor_ = text_.length();
        repaint();
    }
}

void TextControl::onSelect() {
    text_ = text_ + keys_[selected_]->keyValue();
    cursor_ = text_.length();
    repaint();
}


void TextControl::prevKey(int n) {
    if (selected_ > 0) {
        keys_[selected_]->selected(false);
        if (selected_ - n < 0)
            selected_ = 0;
        else
            selected_ -= n;

        keys_[selected_]->selected(true);
        repaint();
    }
}

void TextControl::nextKey(int n) {
    if (selected_ + 1 < keys_.size()) {
        keys_[selected_]->selected(false);
        if (selected_ + n >= keys_.size())
            selected_ = keys_.size() - 1;
        else
            selected_ += n;

        keys_[selected_]->selected(true);
        repaint();
    }
}

}  // namespace ssp
