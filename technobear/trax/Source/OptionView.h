#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"


class OptionView : public ssp::MiniBasicView {
public:
    OptionView(PluginProcessor &p);
    ~OptionView() override;

    void drawView(Graphics &g) override;

private:
    using base_type = MiniBasicView;
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    PluginProcessor &processor_;
};