#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/controls/ParamControl.h"
#include "ssp/controls/ParamButton.h"


//#include "ssp/SingleViewEditor.h"
#include "ssp/editors/BarParamEditor.h"


using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;


inline float normValue(RangedAudioParameter &p) {
    return p.convertFrom0to1(p.getValue());
}

class LayerView : public ssp::BarParamEditor {
public:
    explicit LayerView(PluginProcessor &p, unsigned lidx, Colour clr) :
        ssp::BarParamEditor(&p, false),
        processor_(p) {

        float inc = 1.0f;
        float finc = 0.01f;

        auto &layer = processor_.params_.layers_[lidx];
        addParamPage(
            std::make_shared<pcontrol_type>(layer->rate_, inc, finc, clr),
            std::make_shared<pcontrol_type>(layer->gain_, inc, finc, clr),
            std::make_shared<pcontrol_type>(layer->begin_, 0.1f, 0.01, clr),
            std::make_shared<pcontrol_type>(layer->end_, 0.1, 0.01, clr)
        );
        addParamPage(
            std::make_shared<pcontrol_type>(layer->xfade_, inc, finc, clr),
            std::make_shared<pcontrol_type>(layer->size_, 1.0, 1.0, clr),
            nullptr,
            nullptr
        );

        addButtonPage(
            std::make_shared<bcontrol_type>(layer->mode_, 24, Colours::red),
            std::make_shared<bcontrol_type>(layer->loop_, 24, Colours::yellow),
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
    }

private:
    PluginProcessor &processor_;
};

class RecordView : public ssp::BarParamEditor {

public:
    explicit RecordView(PluginProcessor &p, Colour clr) :
        ssp::BarParamEditor(&p, false),
        processor_(p) {

        auto &reclayer = processor_.params_.recParams_;

        float inc = 1.0f;
        float finc = 0.01f;


        addParamPage(
            std::make_shared<pcontrol_type>(reclayer->gain_, inc, finc, clr),
            std::make_shared<pcontrol_type>(reclayer->mon_, inc, finc, clr),
            nullptr,
            nullptr
        );
//    pView->addParamPage(
//        std::make_shared<pcontrol_type>(reclayer->begin_, 0.1, 0.01,clr),
//        std::make_shared<pcontrol_type>(reclayer->end_, 0.1, 0.01,clr),
//        nullptr,
//        nullptr
//    );

        addButtonPage(
            std::make_shared<bcontrol_type>(reclayer->mode_, 24, Colours::red),
            std::make_shared<bcontrol_type>(reclayer->loop_, 24, Colours::yellow),
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
    }

private:
    PluginProcessor &processor_;
};

void PluginEditor::onFileButton(bool v) {
    if (fileBtn_.value() == v) return;
    fileBtn_.value(v);
    if (!v) {
        switch (viewMode_) {
            case M_REC :
            case M_LAYER : {
                viewMode_ = M_FILE;
                setView(V_FILE);
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                String fname = processor_.getLayerFile(layer);
                fileBrowser_->setFile(fname);
                break;
            }
            case M_FILE : {
                viewMode_ = M_LAYER;
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                String selFileName = fileBrowser_->selectedFile();
                if (selFileName.length() > 0) {
                    std::string fn = selFileName.toStdString();
                    processor_.loadLayerFile(layer, fn);
                }
                setView(V_LAYER_1 + layer);
                break;
            }
            default:;
        }
    }
}

void PluginEditor::onSaveButton(bool v) {
    if (saveBtn_.value() == v) return;
    saveBtn_.value(v);
    if (!v) {
        switch (viewMode_) {
            case M_REC :
            case M_LAYER : {
                viewMode_ = M_SAVE;
                setView(V_SAVE);
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                String fname = processor_.getLayerFile(layer);

                File f(fname);
                if (!f.exists()) {
                    String fn = f.getFileName();
                    saveEditor_->setText(fname.toStdString());
                } else {
                    String fn = f.getFileNameWithoutExtension();
                    fileBrowser_->setFile(fname);
                    saveEditor_->setText(fn.toStdString());
                }

                break;
            }
            case M_SAVE : {
                viewMode_ = M_LAYER;
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                auto selFileName = saveEditor_->getText();
                if (selFileName.length() > 0) {
                    File baseDir(fileBrowser_->baseDir());
                    File f = baseDir.getChildFile(selFileName);
                    std::string fn = f.getFullPathName().toStdString() + ".wav";
                    processor_.saveLayerFile(layer, fn);
                }
                setView(V_LAYER_1 + layer);
                break;
            }
            default:;
        }
    }
}


void PluginEditor::onCancelButton(bool v) {
    if (cancelBtn_.value() == v) return;
    cancelBtn_.value(v);
    if (!v) {
        switch (viewMode_) {
            case M_FILE :
            case M_SAVE : {
                viewMode_ = M_LAYER;
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                setView(V_LAYER_1 + layer);
                break;
            }
            default:;
        }
    }
}

void PluginEditor::setView(unsigned newView) {
    base_type::setView(newView);
    switch (viewMode_) {
        case M_FILE :
        case M_SAVE : {
            leftBtn_.setVisible(false);
            rightBtn_.setVisible(false);
            upBtn_.setVisible(false);
            downBtn_.setVisible(false);
            rightShiftBtn_.setVisible(false);
            leftShiftBtn_.setVisible(false);
            fileBtn_.setVisible(viewMode_ == M_FILE);
            saveBtn_.setVisible(viewMode_ == M_SAVE);
            cancelBtn_.setVisible(true);
            return;
        }
        default: {
            leftBtn_.setVisible(true);
            rightBtn_.setVisible(true);
            upBtn_.setVisible(true);
            downBtn_.setVisible(true);
            rightShiftBtn_.setVisible(true);
            leftShiftBtn_.setVisible(false);
            fileBtn_.setVisible(true);
            saveBtn_.setVisible(true);
            cancelBtn_.setVisible(false);
        }
    }
    rightShiftBtn_.value(viewMode_ == V_RECORD);

}

void PluginEditor::onButton(unsigned int id, bool v) {
    base_type::onButton(id, v);

    switch (id) {
        case B_FILE : {
            onFileButton(v);
            break;
        }
        case B_SAVE : {
            onSaveButton(v);
            break;
        }
        case B_CANCEL : {
            onCancelButton(v);
            break;
        }
        default:;
    }
}


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p),
      processor_(p),
      clrs_{Colours::green, Colours::blue, Colours::red, Colours::yellow},
      fileBtn_("Load", [&](bool b) { onFileButton(b); }, 24, Colours::cyan),
      saveBtn_("Save", [&](bool b) { onSaveButton(b); }, 24, Colours::yellow),
      cancelBtn_("Cancel", [&](bool b) { onCancelButton(b); }, 24, Colours::white) {

#if DEBUG
    assert(MAX_LAYERS == PluginProcessor::MAX_LAYERS);
#endif

    leftBtn_.label("LYR-");
    rightBtn_.label("LYR+");
    rightShiftBtn_.label("REC");
    addAndMakeVisible(rightShiftBtn_);


    for (int l = 0; l < PluginProcessor::MAX_LAYERS; l++) {
        auto clr = clrs_[l];
        auto pView = std::make_shared<LayerView>(p, l, clr);
        addView(pView);
    }

    auto clr = Colours::cyan;
    auto pView = std::make_shared<RecordView>(p, clr);
    addView(pView);

    String defDir = "/media/BOOT/samples";
#ifdef __APPLE__
    defDir = File::getCurrentWorkingDirectory().getFullPathName();
#endif


    saveEditor_ = std::make_shared<ssp::TextEdit>(&p);
    addView(saveEditor_);
    setButtonBounds(saveBtn_, B_SAVE / 4, B_SAVE % 4);
    addAndMakeVisible(saveBtn_);

    fileBrowser_ = std::make_shared<ssp::FileBrowser>(&p, defDir);
    addView(fileBrowser_);
    setButtonBounds(fileBtn_, B_FILE / 4, B_FILE % 4);
    addAndMakeVisible(fileBtn_);

    setButtonBounds(cancelBtn_, B_CANCEL / 4, B_CANCEL % 4);
    addChildComponent(cancelBtn_);

    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        std::string title = std::string("Layer ") + std::to_string(i);
        scopes_[i].initSignal(0, title, layer.dataBuf_, DATA_POINTS, DATA_POINTS, clrs_[i]);
        addAndMakeVisible(scopes_[i]);
    }

    unsigned layer = normValue(processor_.params_.recParams_->layer_);
    setView(V_LAYER_1 + layer);
    viewMode_ = M_LAYER;

    setSize(1600, 480);
}


void PluginEditor::drawView(Graphics &g) {
    if (viewMode_ == M_FILE || viewMode_ == M_SAVE) return;

    base_type::drawView(g);

    for (int i = 0; i < MAX_LAYERS; i++) {
        auto &layer = layer_[i];
        processor_.fillLayerData(i, layer.dataBuf_, DATA_POINTS,
                                 layer.curPos_, layer.beginPos_, layer.endPos_,
                                 layer.isRec_, layer.recPos_);
        scopes_[i].setPosition(0, layer.curPos_, layer.beginPos_, layer.endPos_);
        scopes_[i].setRecPosition(0, layer.isRec_, layer.recPos_);
        scopes_[i].label(0, processor_.getLayerFile(i));

        if (layer.isRec_) {
            const unsigned h = 75;
            const unsigned w = DATA_POINTS;
            const unsigned sp = 5;
            unsigned y = 50;
            unsigned x = 910;
            g.setColour(Colours::darkgrey);
            g.drawRect(x - 2, (i * (h + sp)) + y - 2, w + 4, h + 4, 1);
        }
    }
}


void PluginEditor::resized() {
    base_type::resized();
    const unsigned h = 75;
    const unsigned w = DATA_POINTS;
    const unsigned sp = 5;
    unsigned y = 50;
    unsigned x = 910;

    for (int i = 0; i < MAX_LAYERS; i++) {
        scopes_[i].setBounds(x, y, w, h);
        y += h + sp;
    }
}


void PluginEditor::onRightShiftButton(bool v) {
    if (viewMode_ == M_FILE) {
        fileBrowser_->onRightShiftButton(v);
        return;
    } else if (viewMode_ == M_SAVE) {
        saveEditor_->onRightShiftButton(v);
        return;
    }

    if (!v) {
        viewMode_ = viewMode_ == M_LAYER ? M_REC : M_LAYER;
        switch (viewMode_) {
            case M_LAYER : {
                rightShiftBtn_.value(false);
                unsigned layer = normValue(processor_.params_.recParams_->layer_);
                setView(V_LAYER_1 + layer);
                return;
            }
            case M_REC : {
                rightShiftBtn_.value(true);
                setView(V_RECORD);
                return;
            }
        }
    }
}


void PluginEditor::onLeftButton(bool v) {
    if (viewMode_ == M_FILE) {
        fileBrowser_->onLeftButton(v);
    } else if (viewMode_ == M_SAVE) {
        saveEditor_->onLeftButton(v);
        return;
    }

    leftBtn_.value(v);
    if (!v) {
        auto &p = processor_.params_.recParams_->layer_;
        int nLayer = normValue(p) - 1.0f;
        if (nLayer >= 0) {
            p.beginChangeGesture();
            p.setValueNotifyingHost(p.convertTo0to1(nLayer));
            p.endChangeGesture();
            if (viewMode_ == M_LAYER) {
                setView(V_LAYER_1 + nLayer);
            }
        }
    }
}

void PluginEditor::onRightButton(bool v) {
    if (viewMode_ == M_FILE) {
        fileBrowser_->onRightButton(v);
        return;
    } else if (viewMode_ == M_SAVE) {
        saveEditor_->onRightButton(v);
        return;
    }

    rightBtn_.value(v);
    if (!v) {
        auto &p = processor_.params_.recParams_->layer_;
        int nLayer = normValue(p) + 1.0f;
        if (nLayer < MAX_LAYERS) {
            p.beginChangeGesture();
            p.setValueNotifyingHost(p.convertTo0to1(nLayer));
            p.endChangeGesture();
            if (viewMode_ == M_LAYER && nLayer) {
                setView(V_LAYER_1 + nLayer);
            }
        }
    }
}

