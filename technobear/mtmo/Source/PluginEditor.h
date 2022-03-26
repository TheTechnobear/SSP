#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/BaseEditor.h"

class PluginEditor : public ssp::BaseEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
protected:
    void timerCallback() override;

    void onSSPTimer() override {
        base_type::onSSPTimer();
        timerCallback();
    }

    using base_type = ssp::BaseEditor;
private:

    struct Msg {
        String channel_;
        String type_;
        String num_;
        String value_;
        bool dispNum_;
        bool active_ = false;
    };

    static constexpr unsigned MAX_DATA = 32;
    std::array<Msg, MAX_DATA> dataBuf_;
    unsigned wrPos_ = 0;
    bool clock_ = false;

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};


