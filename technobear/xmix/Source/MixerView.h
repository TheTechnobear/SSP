#pragma once


#include "ssp/BaseView.h"


class MixerView : public ssp::BaseView {
public:
    MixerView(PluginProcessor &p);
    ~MixerView() override;
}

