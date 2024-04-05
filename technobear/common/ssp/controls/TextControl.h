#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <memory>


namespace ssp {

class TextKey;

class TextControl : public juce::Component {
public:
    explicit TextControl();
    virtual ~TextControl();

    void paint(juce::Graphics &g) override;

    std::string getText();
    void setText(const std::string &txt);

    void copy() { copyBuffer_ = text_; }

    void paste() { setText(copyBuffer_); }

    void nextKey();
    void prevKey();
    void onDelete();
    void onSelect();

private:
    void resized() override;

    juce::Colour fg_ = juce::Colours::red;
    juce::Colour bg_ = juce::Colours::black;
    std::string text_;
    int selected_ = 0;
    int nCols_ = 10;
    int cursor_ = 0;
    int cursorFlashCounter = 0;

    std::string copyBuffer_;

    std::vector<std::shared_ptr<TextKey>> keys_;
};

}  // namespace ssp
