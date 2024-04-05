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
        float gap = 2.f * COMPACT_UI_SCALE;
        g.setColour(active_ ? fg_ : bg_);
        g.fillRect(0, 0, getWidth(), getHeight());

        g.setColour(!active_ ? fg_ : bg_);
        g.drawRect(0, 0, getWidth(), getHeight(), 1 + (selected_ * COMPACT_UI_SCALE));

        g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh_, juce::Font::plain));
        g.drawText(k_, gap, gap, getWidth() - (gap * 2.f), getHeight() - (gap * 2.f), juce::Justification::centred);
    }

    std::string keyValue() { return kv_; }

private:
    std::string k_;
    std::string kv_;
    unsigned fh_;
    juce::Colour fg_ = juce::Colours::red;
    juce::Colour bg_ = juce::Colours::black;
    bool selected_ = false;
    bool active_ = false;
};


TextControl::TextControl() {
    float w = 30.f * COMPACT_UI_SCALE;
    float h = 25.f * COMPACT_UI_SCALE;

    int i;
    std::string charset = "1234567890abcdefghijklmnopqrstuvwxyz-_:";
    for (i = 0; i < charset.length(); i++) {
        std::string k = charset.substr(i, 1);
        auto key = std::make_shared<TextKey>(k, h - 4);
        keys_.push_back(key);
        addAndMakeVisible(*key);
    }

    {
        auto key = std::make_shared<TextKey>("Spc", " ", w / 3);
        keys_.push_back(key);
        addAndMakeVisible(*key);
    }
    keys_[0]->selected(true);
}

TextControl::~TextControl() {
}

void TextControl::resized() {
    Component::resized();
    float w = 30.f * COMPACT_UI_SCALE;
    float h = 25.f * COMPACT_UI_SCALE;

    float x = 5.f * COMPACT_UI_SCALE;
    float y = 30.f * COMPACT_UI_SCALE;

    int i = 0;
    for (auto &key : keys_) {
        float r = i / nCols_;
        float c = i % nCols_;
        key->setBounds((c * (w + 2)) + x, (r * (h + 2)) + y, w, h);
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
    float x = 5.f * COMPACT_UI_SCALE;
    float y = 5.f * COMPACT_UI_SCALE;
    float w = getWidth();
    float h = getHeight();
    float fh = 12.f * COMPACT_UI_SCALE;

    juce::Font font(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain);
    g.setColour(bg_);
    g.fillRect(x, y, w, h);
    g.setColour(fg_);
    g.setFont(font);
    g.drawRect(x, y, w, fh * 1.4f);
    g.drawText(text_, x + 2, y, w - 4, fh * 1.4f, juce::Justification::left);

    static constexpr int duty = 30;
    static constexpr int halfduty = duty / 2;
    cursorFlashCounter = (cursorFlashCounter + 1) % duty;

    if (cursorFlashCounter < halfduty) {
        g.setColour(fg_);
    } else {
        g.setColour(bg_);
    }
    float fpos = font.getStringWidthFloat(text_);
    g.fillRect(x + 2.f + fpos + 2.f, y + 4.f, 4.f, fh - 2.0f);
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


void TextControl::prevKey() {
    if (selected_ > 0) {
        keys_[selected_]->selected(false);
        selected_--;
        keys_[selected_]->selected(true);
        repaint();
    }
}

void TextControl::nextKey() {
    if (selected_ + 1 < keys_.size()) {
        keys_[selected_]->selected(false);
        selected_++;
        keys_[selected_]->selected(true);
        repaint();
    }
}

}  // namespace ssp
