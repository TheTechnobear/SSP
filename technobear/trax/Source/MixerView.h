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

    void resized() override;
    void editorShown() override;
    
private:
    using base_type = ssp::MiniBasicView;

    ssp::StereoVuMeter vuMeters_[4];
    PluginProcessor &processor_;
};
