#pragma once

#include "PluginProcessor.h"
#include "ssp/editors/MultiView.h"

class OptionView;
class LoadPresetView;
class SavePresetView;


class OptionEditor : public ssp::MultiView<ssp::BaseView> {
public:
    OptionEditor(PluginProcessor &p);
    ~OptionEditor() override;

    void editorShown() override;
    void resized() override;

    void eventButton(unsigned btn, bool longPress) override;
private:
    using base_type = ssp::MultiView<ssp::BaseView>;

    PluginProcessor &processor_;

    std::shared_ptr<OptionView> optionView_;
    std::shared_ptr<LoadPresetView> loadView_;
    std::shared_ptr<SavePresetView> saveView_;

    unsigned loadViewIdx_, saveViewIdx_, optionViewIdx_;
};
