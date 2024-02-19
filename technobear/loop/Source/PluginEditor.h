#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseViewEditor.h"
#include "ssp/controls/WaveDisp.h"
#include "ssp/editors/FileBrowser.h"
#include "ssp/editors/TextEdit.h"

using namespace juce;

class PluginEditor : public ssp::BaseViewEditor {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;


    void onFileButton(bool v);
    void onSaveButton(bool v);
    void onCancelButton(bool v);
protected:
    using base_type = ssp::BaseViewEditor;

    void onRightShiftButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onButton(unsigned int id, bool v) override;

    void setView(unsigned newView) override;
private:
    enum {
        V_LAYER_1,
        V_LAYER_2,
        V_LAYER_3,
        V_LAYER_4,
        V_RECORD,
        V_SAVE,
        V_FILE,
        V_MAX,
    };

    static const constexpr unsigned MAX_LAYERS = V_LAYER_4 + 1;

    juce::Colour clrs_[MAX_LAYERS];
    static constexpr unsigned DATA_POINTS = 600;

    struct {
        float dataBuf_[DATA_POINTS];
        float beginPos_ = 0.0f;
        float endPos_ = 1.0f;
        float curPos_ = 0.0;
        bool isRec_ = false;
        float recPos_ = 0.0;
    } layer_[MAX_LAYERS];

    enum E_ViewMode {
        M_LAYER,
        M_REC,
        M_SAVE,
        M_FILE,
        M_MAX
    };


    // note: this overlap with parameter buttons!
    enum  {
        B_1,
        B_2,
        B_3,
        B_4,
        B_FILE,
        B_SAVE,
        B_7,
        B_CANCEL,
        B_MAX
    };

    unsigned viewMode_ = M_LAYER;

    std::shared_ptr<ssp::FileBrowser> fileBrowser_;
    std::shared_ptr<ssp::TextEdit> saveEditor_;
    ValueButton fileBtn_, saveBtn_, cancelBtn_;

    ssp::WaveDisp<1> scopes_[MAX_LAYERS] = {
        ssp::WaveDisp<1>(false, true),
        ssp::WaveDisp<1>(false, true),
        ssp::WaveDisp<1>(false, true),
        ssp::WaveDisp<1>(false, true)
    };


    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

