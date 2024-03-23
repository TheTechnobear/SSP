#pragma once


#include "PluginProcessor.h"
#include "ssp/BaseView.h"


class MixerView : public ssp::BaseView {
public:
    MixerView(PluginProcessor &p);
    ~MixerView() override;

    void drawView(Graphics &g) override;
private:
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    PluginProcessor &processor_;
};
