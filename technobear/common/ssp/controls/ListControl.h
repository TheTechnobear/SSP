#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

#include <algorithm>

#include "SSP.h"
namespace ssp {

template <class T = juce::String>
class ListControl : public juce::Component {
public:
    explicit ListControl(std::function<bool(unsigned)> isSelected = nullptr);
    ~ListControl() = default;

    void nextItem();
    void prevItem();
    int idx();
    void idx(int i);
    void clear();
    void addItem(const juce::String& str);

private:
    int curIdx_ = -1;
    int offset_ = 0;
    int nLines_ = 0;
    void paint(juce::Graphics& g) override;
    void resized() override;

    static constexpr int FH = 14 * COMPACT_UI_SCALE;
    static constexpr int LS = FH;
    std::vector<juce::String> items_;
    std::function<bool(unsigned)> isSelected_ = nullptr;
};

template <class T>
ListControl<T>::ListControl(std::function<bool(unsigned)> isSelected) : isSelected_(isSelected) {
}

template <class T>
void ListControl<T>::clear() {
    items_.clear();
    curIdx_ = -1;
    repaint();
}

template <class T>
int ListControl<T>::idx() {
    return curIdx_;
}
template <class T>
void ListControl<T>::idx(int i) {
    curIdx_ = i;
    repaint();
}


template <class T>
void ListControl<T>::addItem(const juce::String& str) {
    items_.push_back(str);
    if (curIdx_ == -1) curIdx_ = 0;
    repaint();
}


template <class T>
void ListControl<T>::nextItem() {
    curIdx_ += curIdx_ < (items_.size() - 1);
    repaint();
}

template <class T>
void ListControl<T>::prevItem() {
    curIdx_ -= curIdx_ != 0;
    repaint();
}


template <class T>
void ListControl<T>::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), FH, juce::Font::plain));
    int s = offset_;
    int e = std::min(offset_ + nLines_, int(items_.size()));
    int x = 0;
    int y = LS;
    for (int i = s; i < e; i++) {
        if (i == curIdx_) {
            g.setColour(juce::Colours::yellow);
        } else {
            g.setColour(juce::Colours::white);
        }

        juce::String str = items_[i];
        if (isSelected_ && isSelected_(i)) { str = str + String("*"); }

        g.drawSingleLineText(str, x, y);
        y += LS;
    }
}

template <class T>
void ListControl<T>::resized() {
    int h = getHeight();
    int w = getWidth();
    nLines_ = (h / LS);
}

}  // namespace ssp