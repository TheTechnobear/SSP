
#include "OptionEditor.h"

#include "OptionView.h"
// #include "LoadPresetView.h"
// #include "SavePresetView.h"

#include "ssp/controls/FileSelector.h"
#include "ssp/controls/TextControl.h"

class LoadPresetView : public ssp::MiniBasicView {
public:
    LoadPresetView(PluginProcessor &p, const String &defDir) : base_type(&p), processor_(p) {
        fileControl_ = std::make_shared<ssp::FileSelector>(defDir);
        addAndMakeVisible(fileControl_.get());
        addButton(0, std::make_shared<ssp::ValueButton>("Load", [&](bool b) {
                      if (!b) onLoad();
                  }));
        addButton(3, std::make_shared<ssp::ValueButton>("Init", [&](bool b) {
                      if (!b) onInit();
                  }));
    }

    void onLoad() { processor_.presetLoadRequest(fileControl_->selectedFile()); }
    void onInit() { processor_.initPreset(); }


    void eventUp(bool longPress) override {
        if (!longPress) { fileControl_->prevEntry(); }
    }

    void eventDown(bool longPress) override {
        if (!longPress) { fileControl_->nextEntry(); }
    }

    void onEncoder(unsigned id, float v) override {
        if (id == 0) {
            if (v > 0) {
                fileControl_->nextEntry();
            } else {
                fileControl_->prevEntry();
            }
        }
    }

    void onEncoderSwitch(unsigned id, bool v) override {
        if (id == 0 && !v) { fileControl_->selectFile(); }
    }

    void editorShown() override {
        base_type::editorShown();
        String fn = fileControl_->baseDir() + File::getSeparatorChar() + processor_.presetName();
        fileControl_->setFile(fn);
    }

    void resized() override {
        base_type::resized();
        fileControl_->setBounds(0, 0, getWidth(), canvasHeight());
    }


private:
    using base_type = ssp::MiniBasicView;
    PluginProcessor &processor_;
    std::shared_ptr<ssp::FileSelector> fileControl_;
};


class SavePresetView : public ssp::MiniBasicView {
public:
    SavePresetView(PluginProcessor &p, const String &defDir) : ssp::MiniBasicView(&p), processor_(p), baseDir_(defDir) {
        textControl_ = std::make_shared<ssp::TextControl>();
        addAndMakeVisible(textControl_.get());
        addButton(0, std::make_shared<ssp::ValueButton>("Save", [&](bool b) {
                      if (!b) onSave();
                  }));
        addButton(4, std::make_shared<ssp::ValueButton>("Default", [&](bool b) {
                      if (!b) { textControl_->setText("default"); };
                  }));
        addButton(3, std::make_shared<ssp::ValueButton>("Del", [&](bool b) {
                      if (!b) { textControl_->onDelete(); }
                  }));
        addButton(7, std::make_shared<ssp::ValueButton>("Clear", [&](bool b) {
                      if (!b) { textControl_->setText(""); }
                  }));
    }

    void onSave() {
        std::string fn = textControl_->getText();
        if (fn.empty()) return;
        processor_.presetSaveRequest(baseDir_ + File::getSeparatorChar() + fn);
    }

    void eventUp(bool longPress) override {
        if (!longPress) { textControl_->prevKey(); }
    }

    void eventDown(bool longPress) override {
        if (!longPress) { textControl_->nextKey(); }
    }

    void onEncoder(unsigned id, float v) override {
        switch (id) {
            case 0: {
                if (v > 0) {
                    textControl_->nextKey();
                } else {
                    textControl_->prevKey();
                }
                break;
            }
            case 1: {
                if (v > 0) {
                    textControl_->nextKey(textControl_->nCols());
                } else {
                    textControl_->prevKey(textControl_->nCols());
                }
                break;
            }
        }
    }


    void onEncoderSwitch(unsigned id, bool v) override {
        if (id == 0 && !v) { textControl_->onSelect(); }
    }

    void editorShown() override {
        base_type::editorShown();
        File f(processor_.presetName());
        if (f.exists()) {
            textControl_->setText(f.getFileName().toStdString());
        } else {
            textControl_->setText("");
        }
    }
    void resized() override {
        base_type::resized();
        textControl_->setBounds(0, 0, getWidth(), canvasHeight());
    }

private:
    using base_type = ssp::MiniBasicView;

    PluginProcessor &processor_;
    std::shared_ptr<ssp::TextControl> textControl_;
    String baseDir_ = "~";
};


OptionEditor::OptionEditor(PluginProcessor &p) : base_type(&p, false), processor_(p) {
    optionView_ = std::make_shared<OptionView>(processor_);
    optionViewIdx_ = addView(optionView_);

    String defDir = File::getCurrentWorkingDirectory().getFullPathName() + File::getSeparatorChar() + "trax_presets";

    File dir(defDir);
    if (!dir.exists()) { dir.createDirectory(); }

    loadView_ = std::make_shared<LoadPresetView>(processor_, defDir);
    loadViewIdx_ = addView(loadView_);

    saveView_ = std::make_shared<SavePresetView>(processor_, defDir);
    saveViewIdx_ = addView(saveView_);
}

OptionEditor::~OptionEditor() {
}

void OptionEditor::resized() {
    base_type::resized();
    static constexpr int pluginWidth = SSP_COMPACT_WIDTH;
    static constexpr int pluginHeight = SSP_COMPACT_HEIGHT;
    saveView_->setBounds(0, 0, pluginWidth, pluginHeight);
    loadView_->setBounds(0, 0, pluginWidth, pluginHeight);
    optionView_->setBounds(0, 0, pluginWidth, pluginHeight);
}

void OptionEditor::editorShown() {
    setView(optionViewIdx_);
    base_type::editorShown();
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
    } else if (v == loadViewIdx_) {
        if (btn == 0 || btn == 1) setView(optionViewIdx_);
        return;
    } else if (v == saveViewIdx_) {
        if (btn == 0) setView(optionViewIdx_);
        return;
    }


    base_type::eventButton(btn, longPress);
}
