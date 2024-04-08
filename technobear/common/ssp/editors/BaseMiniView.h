#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <vector>
#include<memory>

#include "MultiView.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ButtonBox.h"

namespace ssp {

class MiniParamView : public BaseView {
public:
    typedef std::function<float(bool io, unsigned ch)> ioActivity;

    explicit MiniParamView(BaseProcessor *p, ioActivity callback = nullptr);
    ~MiniParamView() = default;

    void addParam(const std::shared_ptr<BaseParamControl> &p);
    void addButton(const std::shared_ptr<ParamButton> &p);

    void resized() override;
protected:
    using base_type = BaseView;

    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;

    void onButton(unsigned int id, bool v) override;

    void eventButton(unsigned int id, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;

    void drawView(Graphics &) override;

private:
    void prevPage();
    void nextPage();
    void drawButtonBox(Graphics &);
    void drawIO(Graphics &);


    bool encoderFine_[4] = { false, false, false, false };
    bool encoderState_[4] = { false, false, false, false };

    std::vector<std::shared_ptr<BaseParamControl> > params_;
    std::vector<std::shared_ptr<ParamButton> > buttons_;
    unsigned paramOffset_ = 0;
    static constexpr unsigned nParamPerPage = 4;
    static constexpr unsigned scale = COMPACT_UI_SCALE;
    static constexpr unsigned maxUserBtns = 8;

    unsigned buttonBarH_ = 0;
    unsigned ioW_ = 0;
    unsigned canvasH_ = 0;
    unsigned canvasW_ = 0;
    unsigned titleH_ = 30;

    ioActivity ioCallback_ = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiniParamView)
};

class MiniBasicView : public BaseView {
public:
    typedef std::function<float(bool io, unsigned ch)> ioActivity;

    explicit MiniBasicView(BaseProcessor *p, ioActivity callback = nullptr);
    ~MiniBasicView() = default;

    void addButton(unsigned idx, const std::shared_ptr<ValueButton> &p);
    std::shared_ptr<ValueButton> getButton(unsigned idx);
    void resized() override;
    
    int  canvasHeight() { return canvasH_;}
protected:
    void onButton(unsigned int id, bool v) override;

    // void drawView(Graphics &) override;

private:
    using base_type = BaseView;
    unsigned paramOffset_ = 0;
    static constexpr unsigned nParamPerPage = 4;
    static constexpr unsigned scale = COMPACT_UI_SCALE;
    static constexpr unsigned maxUserBtns = 8;
    unsigned buttonBarH_ = 0;
    unsigned ioW_ = 0;
    unsigned canvasH_ = 0;
    unsigned canvasW_ = 0;
    unsigned titleH_ = 30;
    std::shared_ptr<ssp::ButtonBox> buttonBox_;

    ioActivity ioCallback_ = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiniBasicView)

};


class BaseMiniView : public MultiView<BaseView> {
public:
    explicit BaseMiniView(BaseProcessor *p);
    ~BaseMiniView() = default;

private:
    using base_type = MultiView<BaseView>;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseMiniView)
};


}  // namespace ssp
