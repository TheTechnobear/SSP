#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"
#include "ssp/BaseView.h"
using namespace juce;


class ModuleView : public ssp::BaseView {
public:
    ModuleView(PluginProcessor &p);
    ~ModuleView() override;

    void drawView(Graphics &g) override;
    void drawModulePanel(Graphics &);
    void drawIO(Graphics &);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    //    void onLeftButton(bool v) override;
    //    void onRightButton(bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    //    void onLeftShiftButton(bool v) override;
    //    void onRightShiftButton(bool v) override;

private:
    PluginProcessor &processor_;
    Component *pComponent_ = nullptr;
};
