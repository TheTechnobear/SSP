#pragma once


#include "PluginProcessor.h"
#include "ssp/BaseView.h"


class MixerView : public ssp::BaseView {
public:
    MixerView(PluginProcessor &p);
    ~MixerView() override;

private:
    PluginProcessor &processor_;
};
