#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/MultiBarEditor.h"

using namespace juce;
class SeqCellComponent;

class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::MultiBarEditor;

    void drawView(Graphics &) override;
    void resized() override;

    void eventLeftShift(bool v) override;
    void eventRightShift(bool v) override;
    void eventUp(bool v) override;
    void eventDown(bool v) override;
    void eventLeft(bool v) override;
    void eventRight(bool v) override;
private:

    static constexpr unsigned L_CLRS = 3;
    static juce::Colour LAYER_COLOURS[L_CLRS];
    PluginProcessor &processor_;

    unsigned currentLayer_ = 0;

    enum CVButtonMode {
        B_GATEACCESS,
        B_GLIDE,
        MAX_CV_BUTTON_MODE
    } cvButtonMode_;

    enum EncoderMode {
        E_CV,
        E_FUN,
        MAX_ENCODER_MODE
    } encoderMode_;


    std::vector<std::shared_ptr<SeqCellComponent>> cells_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



