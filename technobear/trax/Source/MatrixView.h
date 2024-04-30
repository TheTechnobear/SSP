#pragma once

#include "PluginProcessor.h"
#include "ssp/controls/ListControl.h"
#include "ssp/editors/BaseMiniView.h"

#include "components/ModuleComponent.h"

class MatrixView : public ssp::MiniBasicView {
public:
    MatrixView(PluginProcessor &p);
    ~MatrixView() override;

    void drawView(Graphics &g) override;
    void resized() override;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    void moduleIdx(unsigned t, unsigned m) {
        trackIdx_ = t;
        moduleAIdx_ = m;
        moduleBIdx_ = 0;

        moduleA_.trackIdx(trackIdx_);
        moduleA_.slotIdx(moduleAIdx_);
        moduleB_.trackIdx(trackIdx_);
        moduleB_.slotIdx(moduleBIdx_);
        refreshView();
    }

    unsigned  trackIdx() { return trackIdx_;}
    unsigned  moduleIdx() { return moduleAIdx_;}

protected:
    void editorShown() override;

private:
    using base_type = ssp::MiniBasicView;
    static constexpr int FH = 12 * COMPACT_UI_SCALE;
    static constexpr int LS = FH;

    String channelAsString(unsigned midx, unsigned ch, bool inOut);

    void refreshView();
    void refreshModuleB();
    void refreshSelected();

    unsigned trackIdx_;

    int moduleAIdx_;
    ssp::ListControl<> channelA_;

    int moduleBIdx_;
    ssp::ListControl<> channelB_;

    std::vector<bool> chASelected_;
    std::vector<bool> chBSelected_;

    bool isOutput_ = true;

    bool isChASelected(unsigned idx);
    bool isChBSelected(unsigned idx);

    ModuleComponent moduleA_;
    ModuleComponent moduleB_;

    PluginProcessor &processor_;
};
