#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

#include "ssp/controls/VuMeter.h"

class MixerView : public ssp::MiniBasicView {
public:
    MixerView(PluginProcessor &p);
    ~MixerView() override;

    void drawView(Graphics &g) override;
    void onSSPTimer() override;
private:
    ssp::StereoVuMeter vuMeters_[4];

    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    PluginProcessor &processor_;
};
