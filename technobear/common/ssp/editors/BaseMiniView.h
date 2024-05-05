#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <memory>
#include <vector>

#include "MultiView.h"
#include "ssp/controls/ButtonBox.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"

namespace ssp {


// MiniParamView is a view that displays parameters and buttons
class MiniParamView : public BaseView {
public:
    typedef std::function<float(bool io, unsigned ch)> ioActivity;

    explicit MiniParamView(BaseProcessor *p, ioActivity callback = nullptr);
    ~MiniParamView() = default;

    void addParam(const std::shared_ptr<BaseParamControl> &p, const juce::Colour& fg = juce::Colours::white);
    void addButton(const std::shared_ptr<ParamButton> &p);

    void setButtonBounds(unsigned idx, juce::Component *c);

protected:
    using base_type = BaseView;

    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;

    void onButton(unsigned int id, bool v) override;

    void eventButton(unsigned int id, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;

    void drawView(Graphics &) override;

    virtual int canvasHeight() { return canvasH; }
    virtual int canvasWidth() { return canvasW - ioW_; }
    virtual int canvasX() { return ioW_; }
    virtual int canvasY() { return titleH; }

    void resized() override;
    void drawButtonBox(Graphics &);


private:
    void prevPage();
    void nextPage();
    void drawIO(Graphics &);


    bool encoderFine_[4] = { false, false, false, false };
    bool encoderState_[4] = { false, false, false, false };

    std::vector<std::shared_ptr<BaseParamControl>> params_;
    std::vector<std::shared_ptr<ParamButton>> buttons_;
    unsigned paramOffset_ = 0;
    static constexpr unsigned maxUserBtns = 8;

    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned buttonBarH = 32 * COMPACT_UI_SCALE;
    static constexpr unsigned titleH = 15 * COMPACT_UI_SCALE;
    static constexpr unsigned titleFH = 10 * COMPACT_UI_SCALE;

    static constexpr unsigned canvasW = SSP_COMPACT_WIDTH - 2 * gap;
    static constexpr unsigned canvasH = SSP_COMPACT_HEIGHT - buttonBarH - titleH;

    static constexpr unsigned nParamsPerPage = 4;
    static constexpr unsigned gridW = canvasW / nParamsPerPage;
    static constexpr unsigned paramWidth = gridW - gap;
    static constexpr unsigned paramHeight = 32 * COMPACT_UI_SCALE;

    unsigned ioW_ = 0;

    ioActivity ioCallback_ = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiniParamView)
};


// MiniBasicView is a view that displays just displays button
class MiniBasicView : public BaseView {
public:
    typedef std::function<float(bool io, unsigned ch)> ioActivity;

    explicit MiniBasicView(BaseProcessor *p, ioActivity callback = nullptr);
    ~MiniBasicView() = default;

    void addButton(unsigned idx, const std::shared_ptr<ValueButton> &p);
    std::shared_ptr<ValueButton> getButton(unsigned idx);
    void resized() override;

    virtual int canvasHeight() { return canvasH; }
    virtual int canvasWidth() { return canvasW - ioW_; }
    virtual int canvasX() { return gap + ioW_; }
    virtual int canvasY() { return titleH; }

protected:
    void onButton(unsigned int id, bool v) override;

    // void drawView(Graphics &) override;

private:
    using base_type = BaseView;
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned buttonBarH = 32 * COMPACT_UI_SCALE;
    static constexpr unsigned titleH = 15 * COMPACT_UI_SCALE;
    static constexpr unsigned titleFH = 10 * COMPACT_UI_SCALE;
    static constexpr unsigned canvasW = SSP_COMPACT_WIDTH - 2 * gap;
    static constexpr unsigned canvasH = SSP_COMPACT_HEIGHT - buttonBarH - titleH;

    static constexpr unsigned nParamsPerPage = 4;
    static constexpr unsigned gridW = canvasW / nParamsPerPage;
    static constexpr unsigned paramWidth = gridW - gap;
    static constexpr unsigned paramHeight = 32 * COMPACT_UI_SCALE;

    unsigned ioW_ = 0;
    static constexpr unsigned maxUserBtns = 8;
    std::shared_ptr<ssp::ButtonBox> buttonBox_;

    ioActivity ioCallback_ = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiniBasicView)
};


// BaseMiniView is a multi-view
class BaseMiniView : public MultiView<BaseView> {
public:
    explicit BaseMiniView(BaseProcessor *p);
    ~BaseMiniView() = default;

private:
    using base_type = MultiView<BaseView>;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseMiniView)
};


// LineMiniEditor is a mini editor that has buttons and display parameters on single line
class LineMiniEditor : public BaseView {
public:
    LineMiniEditor(BaseProcessor *p);

    virtual int canvasHeight() { return canvasH - paramHeight; }
    virtual int canvasWidth() { return canvasW; }
    virtual int canvasX() { return gap; }
    virtual int canvasY() { return titleH; }
protected:
    using base_type = BaseView;

    void resized() override;
    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;

    void onButton(unsigned int id, bool v) override;

    void eventButton(unsigned int id, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;

    void drawView(Graphics &) override;

    void addParamPage(std::shared_ptr<BaseParamControl> c1, std::shared_ptr<BaseParamControl> c2,
                      std::shared_ptr<BaseParamControl> c3, std::shared_ptr<BaseParamControl> c4, 
                      const juce::Colour& fg=juce::Colours::white);

    void addButtonPage(std::shared_ptr<ParamButton> c1, std::shared_ptr<ParamButton> c2,
                      std::shared_ptr<ParamButton> c3, std::shared_ptr<ParamButton> c4,
                      std::shared_ptr<ParamButton> c5, std::shared_ptr<ParamButton> c6,
                      std::shared_ptr<ParamButton> c7, std::shared_ptr<ParamButton> c8
                      );

protected:
    void setParamBounds(unsigned idx,  juce::Component* p);
    void setButtonBounds(unsigned idx,  juce::Component* p);

    void chgParamPage(int inc, bool changeVis);

    void drawButtonBox(Graphics &);

private:
    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned buttonBarH = 32 * COMPACT_UI_SCALE;
    static constexpr unsigned titleH = 15 * COMPACT_UI_SCALE;
    static constexpr unsigned titleFH = 10 * COMPACT_UI_SCALE;
    static constexpr unsigned canvasW = SSP_COMPACT_WIDTH - 2 * gap;
    static constexpr unsigned canvasH = SSP_COMPACT_HEIGHT - buttonBarH - titleH;

    static constexpr unsigned nParamsPerPage = 4;
    static constexpr unsigned gridW = canvasW / nParamsPerPage;
    static constexpr unsigned paramWidth = gridW - gap;
    static constexpr unsigned paramHeight = 32 * COMPACT_UI_SCALE;


    bool encoderFine_[nParamsPerPage] = { false, false, false, false };
    bool encoderState_[nParamsPerPage] = { false, false, false, false };

    struct ControlPage {
        std::shared_ptr<BaseParamControl> control_[nParamsPerPage] = { nullptr, nullptr, nullptr, nullptr };
    };

    static constexpr unsigned maxUserBtns = 8;
    struct ButtonPage {
        std::shared_ptr<ParamButton> control_[maxUserBtns] = { nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr};
    };

    std::vector<std::shared_ptr<ControlPage>> controlPages_;
    std::vector<std::shared_ptr<ButtonPage>> buttonPages_;
    int paramPage_ = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LineMiniEditor)
};

class PageMiniView : public BaseView {
public:
    explicit PageMiniView(BaseProcessor *p);
    ~PageMiniView() = default;

    void addParamPage(std::shared_ptr<BaseParamControl> c1, std::shared_ptr<BaseParamControl> c2,
                      std::shared_ptr<BaseParamControl> c3, std::shared_ptr<BaseParamControl> c4, 
                      const juce::Colour& fg=juce::Colours::white);

    void addButtonPage(std::shared_ptr<ParamButton> c1, std::shared_ptr<ParamButton> c2,
                      std::shared_ptr<ParamButton> c3, std::shared_ptr<ParamButton> c4,
                      std::shared_ptr<ParamButton> c5, std::shared_ptr<ParamButton> c6,
                      std::shared_ptr<ParamButton> c7, std::shared_ptr<ParamButton> c8
                      );


protected:
    using base_type = BaseView;

    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;

    void onButton(unsigned int id, bool v) override;

    void eventButton(unsigned int id, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;

    void drawView(Graphics &) override;

    virtual int canvasHeight() { return canvasH; }
    virtual int canvasWidth() { return canvasW; }
    virtual int canvasX() { return gap; }
    virtual int canvasY() { return titleH; }

    void setParamBounds(unsigned idx,  juce::Component* p);
    void setButtonBounds(unsigned idx, juce::Component *c);


    void resized() override;
    void drawButtonBox(Graphics &);
    void chgParamPage(int inc, bool changeVis);


private:

    bool encoderFine_[4] = { false, false, false, false };
    bool encoderState_[4] = { false, false, false, false };

    static constexpr unsigned maxUserBtns = 8;

    static constexpr unsigned gap = 5 * COMPACT_UI_SCALE;
    static constexpr unsigned buttonBarH = 32 * COMPACT_UI_SCALE;
    static constexpr unsigned titleH = 15 * COMPACT_UI_SCALE;
    static constexpr unsigned titleFH = 10 * COMPACT_UI_SCALE;
    static constexpr unsigned canvasW = SSP_COMPACT_WIDTH - 2 * gap;
    static constexpr unsigned canvasH = SSP_COMPACT_HEIGHT - buttonBarH - titleH;

    static constexpr unsigned nParamsPerPage = 4;
    static constexpr unsigned gridW = canvasW / nParamsPerPage;
    static constexpr unsigned paramWidth = gridW - gap;
    static constexpr unsigned paramHeight = 32 * COMPACT_UI_SCALE;

    struct ControlPage {
        std::shared_ptr<BaseParamControl> control_[nParamsPerPage] = { nullptr, nullptr, nullptr, nullptr };
    };

    struct ButtonPage {
        std::shared_ptr<ParamButton> control_[maxUserBtns] = { nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr};
    };
    int paramPage_ = 0;

    std::vector<std::shared_ptr<ControlPage>> controlPages_;
    std::vector<std::shared_ptr<ButtonPage>> buttonPages_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageMiniView)
};

}  // namespace ssp
