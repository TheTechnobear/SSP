#pragma once

#include "PluginProcessor.h"
#include "ssp/controls/ListControl.h"
#include "ssp/editors/BaseMiniView.h"

class AddRouteView : public ssp::MiniBasicView {
public:
    AddRouteView(PluginProcessor &p);
    ~AddRouteView() override;

    void drawView(Graphics &g) override;
    void trackIdx(unsigned t);
    void resized() override;

    void onPrevBtn();
    void onNextBtn();

    void onEncoder(unsigned enc, float v) override;

protected:
private:
    void addRoute(int trackIdx, int srcIdx, int srcChIdx, int destIdx, int destChIdx);

    static constexpr int FH = 12 * COMPACT_UI_SCALE;
    static constexpr int LS = FH;
    String channelName(bool, unsigned mod, unsigned ch);
    String srcName(unsigned mod);
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;
    unsigned trackIdx_;

    void setupList();
    int srcIdx_ = -1;
    int srcChIdx_ = -1;
    int destIdx_ = -1;
    int destChIdx_ = -1;
    ssp::ListControl<> listCtrl_;
    enum { LM_SRC, LM_SRC_CH, LM_DEST, LM_DEST_CH, LM_MAX } modeIdx_ = LM_SRC;

    PluginProcessor &processor_;
};
