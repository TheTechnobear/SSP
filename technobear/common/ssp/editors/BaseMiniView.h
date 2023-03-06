#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "MultiView.h"

#include <vector>

using namespace juce;


#include "ssp/controls/ParamControl.h"

namespace ssp {

class MiniParamView : public BaseView {
public:
    explicit MiniParamView(BaseProcessor *p);
    ~MiniParamView() = default;
    void addParam(const std::shared_ptr<BaseParamControl>& p);
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;

    void drawView(Graphics &) override;


    using base_type = BaseView;
private:
    bool encoderFine_[4] = {false, false, false, false};
    bool encoderState_[4] = {false, false, false, false};

    std::vector< std::shared_ptr<BaseParamControl> > params_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniParamView)
};

class BaseMiniView : public MultiView {
public:
    explicit BaseMiniView(BaseProcessor *p);
    ~BaseMiniView() = default;

    using base_type = MultiView;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseMiniView)
};


}
