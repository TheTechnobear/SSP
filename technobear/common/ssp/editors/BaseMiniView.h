#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "MultiView.h"

#include <vector>

using namespace juce;


#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"

namespace ssp {

class MiniParamView : public BaseView {
public:
    typedef std::function<float(bool io, unsigned ch)> ioActivity;

    explicit MiniParamView(BaseProcessor *p, ioActivity callback=nullptr);
    ~MiniParamView() = default;
    using base_type = BaseView;

    void addParam(const std::shared_ptr<BaseParamControl>& p);
    void addButton(const std::shared_ptr<ParamButton>& p);
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void onButton(unsigned int id, bool v) override;

    void drawView(Graphics &) override;
    void resized() override;

private:
    void nextPage();
    void drawButtonBox(Graphics &);
    void drawIO(Graphics &);


    bool encoderFine_[4] = {false, false, false, false};
    bool encoderState_[4] = {false, false, false, false};

    std::vector< std::shared_ptr<BaseParamControl> > params_;
    std::vector< std::shared_ptr<ParamButton> > buttons_;
    unsigned paramOffset_=0;
    static constexpr unsigned  nParamPerPage = 4;
    static constexpr unsigned  scale = COMPACT_UI_SCALE;
    static constexpr unsigned  nextPageBtnId = 7;
    static constexpr unsigned maxUserBtns = 6;

    unsigned buttonBarH_ = 0;
    unsigned ioW_ = 0;
    unsigned canvasH_ = 0;
    unsigned canvasW_ = 0;
    unsigned titleH_ = 30;
    ValueButton pageBtn_;

    ioActivity ioCallback_;

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
