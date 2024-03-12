#include "TextEdit.h"


namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;

class Key : public Component {
public:
    Key(const std::string &k, unsigned fh = 55) : Key(k, k, fh) {
    }

    Key(const std::string &k, const std::string &kv, unsigned fh) : k_(k), fh_(fh), kv_(kv) {
    }

    bool active() { return active_; }

    void active(bool a) { active_ = a; }

    bool selected() { return selected_; }

    void selected(bool a) { selected_ = a; }

    void paint(Graphics &g) {

        g.setColour(active_ ? fg_ : bg_);
        g.fillRect(0, 0, getWidth(), getHeight());

        g.setColour(!active_ ? fg_ : bg_);
        g.drawRect(0, 0, getWidth(), getHeight(), 1 + (selected_ * 3));

        g.setFont(Font(Font::getDefaultMonospacedFontName(), fh_, Font::plain));
        g.drawText(k_, 4.0f, 4.0f, getWidth() - 8.f, getHeight() - 8.f, Justification::centred);
    }

    std::string keyValue() { return kv_; }

private:
    std::string k_;
    std::string kv_;
    unsigned fh_;
    Colour fg_ = Colours::red;
    Colour bg_ = Colours::black;
    bool selected_ = false;
    bool active_ = false;
};


TextEdit::TextEdit(BaseProcessor *p) : BaseEditor(p),
                                       copyBtn_("Copy", [&](bool b) { onCopyButton(b); }, 24, Colours::cyan),
                                       pasteBtn_("Paste", [&](bool b) { onPasteButton(b); }, 24, Colours::yellow) {
//    upBtn_.setVisible(false);
//    downBtn_.setVisible(false);
    int bfh = 48;
    upBtn_.label("^", bfh);
    downBtn_.label("v", bfh);
    leftBtn_.label("<", bfh);
    rightBtn_.label(">", bfh);

    addAndMakeVisible(rightBtn_);
    addAndMakeVisible(leftBtn_);

    leftShiftBtn_.label("Del");
    rightShiftBtn_.label("Sel");
    addAndMakeVisible(leftShiftBtn_);
    addAndMakeVisible(rightShiftBtn_);


    setButtonBounds(copyBtn_, B_COPY / 4, B_COPY % 4);
    addAndMakeVisible(copyBtn_);
    setButtonBounds(pasteBtn_, B_PASTE / 4, B_PASTE % 4);
    addAndMakeVisible(pasteBtn_);

    float x = 50.0f;
    float y = 250.0f;
    float w = 70;
    float h = 50;

    int i;
    std::string charset = "1234567890abcdefghijklmnopqrstuvwxyz-_:";
    for (i = 0; i < charset.length(); i++) {
        std::string k = charset.substr(i, 1);

        float r = i / nCols_;
        float c = i % nCols_;


        auto key = std::make_shared<Key>(k, h - 4);
        key->setBounds((c * (w + 2)) + x, (r * (h + 2)) + y, w, h);
        keys_.push_back(key);
        addAndMakeVisible(*key);
    }

    {
        float r = i / nCols_;
        float c = i % nCols_;
        auto key = std::make_shared<Key>("Spc", " ", w / 3);
        key->setBounds((c * (w + 2)) + x, (r * (h + 2)) + y, w, h);
        keys_.push_back(key);
        addAndMakeVisible(*key);
    }
    keys_[0]->selected(true);
}

TextEdit::~TextEdit() {
}

std::string TextEdit::getText() {
    return text_;
}

void TextEdit::setText(const std::string &txt) {
    text_ = txt;
    cursor_ = text_.length();
}

void TextEdit::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)

    float x = 20.0f;
    float y = 50.0f;
    float w = 800;
    float h = 400;
    float fh = 24;
    Font font(Font::getDefaultMonospacedFontName(), fh, Font::plain);
    g.setColour(bg_);
    g.fillRect(x, y, w, h);
    g.setColour(fg_);
    g.setFont(font);
    g.drawRect(x, y, w, fh * 1.4f);
    g.drawText(text_, x + 2, y, w - 4, fh * 1.4f, Justification::left);

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

void TextEdit::onDelete() {
    if (text_.length()) {
        text_ = text_.substr(0, text_.length() - 1);
        cursor_ = text_.length();
    }
}

void TextEdit::onSelect() {
    text_ = text_ + keys_[selected_]->keyValue();
    cursor_ = text_.length();
}


void TextEdit::onEncoder(unsigned id, float v) {
    base_type::onEncoder(id, v);
    switch (id) {
        case 0 : {
            if (v > 0) {
                if (selected_ + 1 < keys_.size()) {
                    keys_[selected_]->selected(false);
                    selected_++;
                    keys_[selected_]->selected(true);
                }
            } else {
                if (selected_ > 0) {
                    keys_[selected_]->selected(false);
                    selected_--;
                    keys_[selected_]->selected(true);
                }
            }
            break;
        }
        default:
            ;
    }
}

void TextEdit::onEncoderSwitch(unsigned id, bool v) {
    base_type::onEncoderSwitch(id, v);
    if (v) return;
    switch (id) {
        case 0 : {
            keys_[selected_]->active(v);
            if (!v) onSelect();
            break;
        }
        default:
            ;
    }
}

void TextEdit::onButton(unsigned id, bool v) {
    base_type::onButton(id, v);
    switch (id) {
        case B_COPY : {
            onCopyButton(v);
            break;
        }
        case B_PASTE : {
            onPasteButton(v);
            break;
        }
    }
}

void TextEdit::eventUp(bool v) {
    base_type::eventUp(v);
    if (v) return;
    if (selected_ >= nCols_) {
        keys_[selected_]->selected(false);
        selected_ -= nCols_;
        keys_[selected_]->selected(true);
    }
}

void TextEdit::eventDown(bool v) {
    base_type::eventDown(v);
    if (v) return;
    if (selected_ + nCols_ < keys_.size()) {
        keys_[selected_]->selected(false);
        selected_ += nCols_;
        keys_[selected_]->selected(true);
    }
}


void TextEdit::eventLeft(bool v) {
    base_type::eventLeft(v);
    if (v) return;
    if (selected_ > 0) {
        keys_[selected_]->selected(false);
        selected_--;
        keys_[selected_]->selected(true);
    }
}

void TextEdit::eventRight(bool v) {
    base_type::eventRight(v);
    if (v) return;
    if (selected_ + 1 < keys_.size()) {
        keys_[selected_]->selected(false);
        selected_++;
        keys_[selected_]->selected(true);
    }
}

void TextEdit::eventLeftShift(bool v) {
    base_type::eventLeftShift(v);
    if(v) return;
    onDelete();
}

void TextEdit::eventRightShift(bool v) {
    base_type::eventRightShift(v);
    if(v) return;
    keys_[selected_]->active(true);
    onSelect();
}


}
