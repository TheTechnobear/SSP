#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/BaseMiniView.h"

class PluginMiniEditor : public ssp::BaseView {
public:
    explicit PluginMiniEditor(PluginProcessor &);
    ~PluginMiniEditor() override = default;

protected:
    void drawView(Graphics &g) override;
    void onSSPTimer() override;

    using base_type = ssp::BaseView;

private:

    struct Msg {
        String channel_;
        String type_;
        String num_;
        String value_;
        bool dispNum_;
        bool active_ = false;
    };

    static constexpr unsigned MAX_DATA = 32;
    std::array<Msg, MAX_DATA> dataBuf_;
    unsigned wrPos_ = 0;
    bool clock_ = false;

    PluginProcessor &processor_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginMiniEditor)
};
