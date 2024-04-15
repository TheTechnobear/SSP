#pragma once


#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

#include "ssp/controls/VuMeter.h"

class MixerView : public ssp::MiniBasicView {
public:
    MixerView(PluginProcessor &p);
    ~MixerView() override;


    void onEncoderSwitch(unsigned enc, bool v) override;
    void onEncoder(unsigned id, float v) override;
    
    void drawView(Graphics &g) override;
    void onSSPTimer() override;

    void resized() override;
    void editorShown() override;
    
private:
    using base_type = ssp::MiniBasicView;

    ssp::StereoVuMeter vuMeters_[4];
    bool encDown_[4] = { false, false, false, false };
    bool encFine_[4] = { false, false, false, false };
    PluginProcessor &processor_;
};
