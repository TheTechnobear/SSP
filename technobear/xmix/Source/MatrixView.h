#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class MatrixView : public ssp::MiniBasicView {
public:
    MatrixView(PluginProcessor &p);
    ~MatrixView() override;

    void drawView(Graphics &g) override;


    void trackIdx(unsigned t) { trackIdx_ = t; }
private:

    String  routeAsString(unsigned modIdx, Matrix::Routing& route, bool output);
    String  channelName(bool, unsigned mod, unsigned ch);
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;
    unsigned trackIdx_;

    PluginProcessor &processor_;
};
