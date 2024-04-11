#pragma once

#include "PluginProcessor.h"
#include "ssp/controls/ListControl.h"
#include "ssp/editors/BaseMiniView.h"

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
        moduleIdx_ = m;
        refreshView();
    }

protected:
    void editorShown() override;

private:
    using base_type = ssp::MiniBasicView;
    static constexpr int FH = 12 * COMPACT_UI_SCALE;
    static constexpr int LS = FH;

    String moduleAsString(unsigned idx);
    String channelAsString(unsigned midx, unsigned ch, bool inOut);

    void refreshView();
    void refreshModuleB();
    void refreshSelected();

    unsigned trackIdx_;
    unsigned moduleIdx_;
    int moduleBIdx_ = -1;

    ssp::ListControl<> channelA_;
    ssp::ListControl<> moduleB_;
    ssp::ListControl<> channelB_;

    std::vector<bool> chASelected_;
    std::vector<bool> moduleBSelected_;
    std::vector<bool> chBSelected_;

    bool isOutput_ = true;

    bool isChASelected(unsigned idx);
    bool isModuleBSelected(unsigned idx);
    bool isChBSelected(unsigned idx);

    PluginProcessor &processor_;
};
