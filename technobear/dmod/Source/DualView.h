#pragma once



#include "PluginProcessor.h"
#include "ssp/BaseView.h"

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

class ModuleView;

class DualView : public ssp::BaseView {
public:
    DualView(PluginProcessor &p);
    ~DualView() override;

    void drawView(Graphics &g) override;
    void drawModulePanel(Graphics &, unsigned);

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;

    void moduleIdx(int m);
    void refreshComponents();
    
private:
    std::shared_ptr<ModuleView> leftView_;
    std::shared_ptr<ModuleView> rightView_;

    int activeModule_ = 0;
    PluginProcessor &processor_;
    Component *pComponent_[2] = { nullptr, nullptr };
};
