#pragma once

#include "PluginProcessor.h"
#include "ssp/controls/ListControl.h"
#include "ssp/editors/BaseMiniView.h"

class MatrixView : public ssp::MiniBasicView {
public:
    MatrixView(PluginProcessor &p);
    ~MatrixView() override;

    void drawView(Graphics &g) override;
    void trackIdx(unsigned t);
    void resized() override;

    void onListSwitchBtn();
    void onRemoveBtn();
    void onAddBtn();

    void onEncoder(unsigned enc, float v) override;

protected:
private:

    bool listModuleIdx(int listIdx, int& modIdx, int& inIdx);
    static constexpr int FH = 12 * COMPACT_UI_SCALE;
    static constexpr int LS = FH;
    String routeAsString(unsigned modIdx, Matrix::Routing &route, bool output);
    String channelName(bool, unsigned mod, unsigned ch);
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;
    unsigned trackIdx_;
    bool outputListSelected_ = false;

    ssp::ListControl<> moduleList_;
    ssp::ListControl<> outputList_;

    PluginProcessor &processor_;
};
