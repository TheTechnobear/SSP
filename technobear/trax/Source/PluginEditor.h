#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"
#include <vector>
#include <memory>


class TrackEditor;
class MixerView;
class OptionEditor;
class PerformanceEditor;

class PluginEditor : public ssp::MultiView<ssp::BaseView> {
public:
    explicit PluginEditor(PluginProcessor &);
    ~PluginEditor() override = default;

protected:
    void eventButton(unsigned btn,bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;
    void eventButtonHeld(unsigned btn) override;
private:
    using base_type = ssp::MultiView<ssp::BaseView>;

    PluginProcessor &processor_;

    std::shared_ptr<MixerView> mixerView_;
    std::shared_ptr<TrackEditor> trackEditor_;
    std::shared_ptr<OptionEditor> optionEditor_;
    std::shared_ptr<PerformanceEditor> perfEditor_;
    unsigned trackEditorIdx_, mixerViewIdx_, optionViewIdx_, perfEditorIdx_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
