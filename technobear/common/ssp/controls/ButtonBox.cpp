#include "ButtonBox.h"

#include "ValueButton.h"

namespace ssp {

ButtonBox::ButtonBox() {
    for (int i = 0; i < maxUserBtns; i++) buttons_[i] = nullptr;
}

void ButtonBox::paint(juce::Graphics &g) {
    drawButtonBox(g);
}


void ButtonBox::resized() {
    juce::Component::resized();
    static constexpr unsigned gap = 5 * scale;
    unsigned butTopY = 0;
    unsigned butLeftX = (gap / 2);
    unsigned barW = getWidth();
    unsigned barH = getHeight();
    unsigned gw = barW / 4;
    unsigned bw = gw - gap;

    unsigned bidx = 0;
    for (auto p : buttons_) {
        if (bidx < maxUserBtns) {
            unsigned r = bidx / 4;
            unsigned c = bidx % 4;
            if (p) p->setBounds(butLeftX + (c * gw), butTopY + r * (barH / 2), bw, barH / 2);
        }
        bidx++;
    }
}

void ButtonBox::drawButtonBox(juce::Graphics &g) {
    static constexpr unsigned gap = 5 * scale;
    unsigned barW = getWidth();
    unsigned barH = getHeight();
    unsigned gw = barW / 4;

    g.setColour(Colours::darkgrey);
    g.drawHorizontalLine(0, 0, barW - 1);
    g.drawHorizontalLine(barH / 2, 0, barW - 1);
    g.drawHorizontalLine(barH - 1, 0, barW - 1);

    for (int i = 0; i < 5; i++) { g.drawVerticalLine(i * gw - (i > 0 ? 1 : 0), 0, barH - 1); }
}

void ButtonBox::addButton(unsigned idx, const std::shared_ptr<ValueButton> &p) {
    if (idx < maxUserBtns) {
        buttons_[idx] = p;
        addAndMakeVisible(*buttons_[idx]);
    }
}

std::shared_ptr<ValueButton> ButtonBox::getButton(unsigned idx) {
    if (idx < maxUserBtns) return buttons_[idx];
    return nullptr;
}

void ButtonBox::onButton(unsigned id, bool v) {
    if (id >= maxUserBtns || buttons_[id] == nullptr) return;
    auto &btn = buttons_[id];
    btn->onButton(v);
}

void ButtonBox::visibilityChanged() {
    juce::Component::visibilityChanged();
    for (auto p : buttons_) {
        if (p) p->setVisible(isVisible());
    }
}


}  // namespace ssp