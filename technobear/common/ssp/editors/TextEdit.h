#pragma once

#include "BaseEditor.h"

namespace ssp {

class Key;

class TextEdit : public BaseEditor {

public:
    explicit TextEdit(BaseProcessor *p);
    virtual ~TextEdit();

    void drawView(juce::Graphics &g) override;

    void editorShown() override {};

    void editorHidden() override {};

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    void onButton(unsigned btn, bool v) override;
    
    void eventUp(bool v) override;
    void eventDown(bool v) override;
    void eventLeft(bool v) override;
    void eventRight(bool v) override;
    void eventLeftShift(bool v) override;
    void eventRightShift(bool v) override;

    std::string getText();
    void setText(const std::string &txt);

    void onCopyButton(bool v) {
        copyBtn_.onButton(v);
        if (!v) {
            copyBuffer_ = text_;
        }
    }

    void onPasteButton(bool v) {
        pasteBtn_.onButton(v);
        if (!v) {
            setText(copyBuffer_);
        }
    }

    void onDelete();
    void onSelect();

private:
    using base_type = BaseEditor;

    juce::Colour fg_ = Colours::red;
    juce::Colour bg_ = defaultBg_;
    std::string text_;
    int selected_ = 0;
    int nCols_ = 10;
    int cursor_ = 0;
    int cursorFlashCounter = 0;


    enum {
        B_COPY,
        B_PASTE,
        B_3,
        B_4,
        B_5,
        B_6,
        B_7,
        B_8,
        B_MAX
    };
    std::string copyBuffer_;
    ValueButton copyBtn_, pasteBtn_;


    std::vector<std::shared_ptr<Key>> keys_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextEdit)
};

}




