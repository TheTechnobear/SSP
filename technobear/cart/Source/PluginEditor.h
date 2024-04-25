#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/MultiBarEditor.h"

using namespace juce;

class PluginEditor : public ssp::MultiBarEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    bool useNotesForSteps(unsigned layer);
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

    class SeqCell : public Component {
    public:
        using Parameter = juce::RangedAudioParameter;

        SeqCell(PluginProcessor::PluginParams &params, unsigned step);
        void activeStep(unsigned xP, unsigned yP, unsigned cP);

        void paint(Graphics &g);

    private:
        unsigned step_;
        bool activeX_, activeY_, activeC_;
        PluginProcessor::PluginParams &params_;
    };


    PluginProcessor &processor_;

    static constexpr unsigned L_CLRS = 3;

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


    juce::Colour clrs_[L_CLRS];

    std::vector<std::shared_ptr<SeqCell>> cells_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};



