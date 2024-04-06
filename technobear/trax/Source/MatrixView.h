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

    void onRemoveBtn();

    void onEncoder(unsigned enc, float v) override;

protected:
private:

    static constexpr int FH = 12 * COMPACT_UI_SCALE;
    static constexpr int LS = FH;

    String jackName(bool inOut, const Matrix::Jack& jack);
    String wireAsString( Matrix::Wire& wire);

    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;
    unsigned trackIdx_;

    ssp::ListControl<> connectionList_;

    PluginProcessor &processor_;
};
