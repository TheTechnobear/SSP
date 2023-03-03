#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"
#include "ssp/editors/BaseEditor.h"

class PluginEditor : public ssp::BaseEditor {
public:
    explicit PluginEditor(PluginProcessor &, unsigned maxModules);
    ~PluginEditor() override = default;

    void drawView(Graphics &) override;
    void resized() override;
protected:
    using base_type = ssp::BaseEditor;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;

    void onSSPTimer() override;
    void editorShown() override;
    void editorHidden() override;

private:
    void drawControls(Graphics &);
    void drawPanel(Graphics &,unsigned);
    void drawIO(Graphics &,unsigned);
    void selectPlugin(Graphics &g, unsigned panel);

    PluginProcessor &processor_;
    int maxModules_=2;
    int activeModule_ = 0;
    enum eViewMode {
        V_MODULE,
        V_LOAD
    } viewMode_[2]= {V_MODULE,V_MODULE};

    int pluginWidth = 1600;
    int pluginHeight = 480;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};

