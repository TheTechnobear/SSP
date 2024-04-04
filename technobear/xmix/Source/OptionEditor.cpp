
#include "OptionEditor.h"

#include "OptionView.h"
// #include "LoadPresetView.h"
// #include "SavePresetView.h"

#include "ssp/editors/FileBrowser.h"
#include "ssp/editors/TextEdit.h"

class LoadPresetView : public ssp::FileBrowser {
public:
    LoadPresetView(PluginProcessor &p, String defDir) : ssp::FileBrowser(&p, defDir) {}
};


class SavePresetView : public ssp::TextEdit {
public:
    SavePresetView(PluginProcessor &p) : ssp::TextEdit(&p) {}
};


OptionEditor::OptionEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    optionView_ = std::make_shared<OptionView>(processor_);
    optionView_->setBounds(0, 0, pluginWidth, pluginHeight);
    optionView_->resized();
    optionViewIdx_ = addView(optionView_);

    String defDir = "/media/BOOT/trax";
#ifdef __APPLE__
    defDir = File::getCurrentWorkingDirectory().getFullPathName();
#endif

    loadView_ = std::make_shared<LoadPresetView>(processor_,defDir);
    loadView_->setBounds(0, 0, pluginWidth, pluginHeight);
    loadView_->resized();
    loadViewIdx_ = addView(loadView_);

    saveView_ = std::make_shared<SavePresetView>(processor_);
    saveView_->setBounds(0, 0, pluginWidth, pluginHeight);
    saveView_->resized();
    saveViewIdx_ = addView(saveView_);
}

OptionEditor::~OptionEditor() {
}

void OptionEditor::editorShown() {
    setView(optionViewIdx_);
}


void OptionEditor::eventButton(unsigned btn, bool longPress) {
    auto v = getViewIdx();
    if (v == optionViewIdx_) {
        switch (btn) {
            case 0: {
                setView(loadViewIdx_);
                return;
            }
            case 1: {
                setView(saveViewIdx_);
                return;
            }
        }
    }


    base_type::eventButton(btn, longPress);
}
