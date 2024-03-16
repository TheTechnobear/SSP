#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"
#include <vector>
#include <memory>


class TrackView;
class MixerView;

class PluginEditor : public ssp::MultiView {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    using base_type = ssp::MultiView;

    void eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) override;

private:

    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;

    std::vector<std::shared_ptr<TrackView>> trackViews_;
    std::shared_ptr<MixerView> mixerView_;

    PluginProcessor &processor_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
