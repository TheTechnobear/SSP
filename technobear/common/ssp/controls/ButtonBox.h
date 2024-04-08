#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <memory>


namespace ssp {

class ValueButton;

class ButtonBox : public juce::Component {
public:
    explicit ButtonBox();

    void addButton(unsigned idx, const std::shared_ptr<ValueButton> &p);
    void onButton(unsigned id, bool v);
    std::shared_ptr<ValueButton>  getButton(unsigned idx);

private:
    void paint(juce::Graphics &g) override;
    void resized() override;

    static constexpr unsigned scale = COMPACT_UI_SCALE;
    static constexpr unsigned maxUserBtns = 8;

    void drawButtonBox(juce::Graphics &);
    std::shared_ptr<ValueButton> buttons_[maxUserBtns];
};


}  // namespace ssp