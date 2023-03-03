#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "MultiView.h"

#include <vector>

using namespace juce;

namespace ssp {

class MiniParamView : public BaseView {
public:
    explicit MiniParamView(BaseProcessor *p);
    ~MiniParamView() = default;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiniParamView)
};

class BaseMiniView : public MultiView {
public:
    explicit BaseMiniView(BaseProcessor *p);
    ~BaseMiniView() = default;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseMiniView)
};


}
