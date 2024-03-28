#include "MatrixView.h"

#include "Matrix.h"
#include "Track.h"


MatrixView::MatrixView(PluginProcessor& p) : ssp::MiniBasicView(&p, nullptr), processor_(p) {
    addButton(0, std::make_shared<ValueButton>("Mod/Out", [&](bool b) {
                  if (!b) onListSwitchBtn();
              }));

    addButton(3, std::make_shared<ValueButton>("Add Rte", [&](bool b) {
                  if (!b) onAddBtn();
              }));
    addButton(7, std::make_shared<ValueButton>("Del Rte", [&](bool b) {
                  if (!b) onRemoveBtn();
              }));


    addAndMakeVisible(moduleList_);
    addAndMakeVisible(outputList_);
}

MatrixView::~MatrixView() {
}


String MatrixView::channelName(bool inOut, unsigned mod, unsigned ch) {
    auto& track = processor_.track(trackIdx_);
    auto& module = track.modules_[mod];
    if (module.plugin_ && module.descriptor_) {
        String str = module.pluginName_;
        str += "/";
        if (inOut)
            str += String(module.descriptor_->outputChannelNames[ch]);
        else
            str += String(module.descriptor_->inputChannelNames[ch]);
        return str;
    }
    return String("Empty/") + String(ch + 1);
}

String MatrixView::routeAsString(unsigned modIdx, Matrix::Routing& route, bool output) {
    String str;
    switch (route.src_) {
        case Matrix::SRC_INPUT: {
            str += "IN/" + String(route.srcChannel_ + 1);
            break;
        }
        case Matrix::SRC_MOD: {
            str += channelName(true, Track::M_MOD, route.srcChannel_);
            break;
        }
        case Matrix::SRC_WORK: {
            unsigned preMod = modIdx;
            if (!output && preMod) preMod--;

            str += channelName(true, preMod, route.srcChannel_);
            break;
        }
        default: {
            str += "Error/" + String(route.srcChannel_ + 1);
            break;
        }
    }

    str += String(" -> ");

    if (output) {
        str += String("OUT/") + String(route.destChannel_ + 1);
    } else {
        str += channelName(false, modIdx, route.destChannel_);
    }

    return str;
}


void MatrixView::trackIdx(unsigned t) {
    trackIdx_ = t;

    auto& track = processor_.track(trackIdx_);
    auto& matrix = track.matrix_;

    moduleList_.clear();
    unsigned modIdx = 0;
    for (auto& m : matrix.modules_) {
        for (auto& r : m.routes_) {
            String str = routeAsString(modIdx, r, false);
            moduleList_.addItem(str);
        }
        modIdx++;
    }

    outputList_.clear();
    auto& out = matrix.output_;
    for (auto& r : out.routes_) {
        String str = routeAsString(Track::M_POST, r, true);
        outputList_.addItem(str);
    }

    outputListSelected_ = false;
    moduleList_.idx(0);
    outputList_.idx(-1);
}


void MatrixView::resized() {
    MiniBasicView::resized();
    int x = LS;
    int y = LS * 3;
    int width = (SSP_COMPACT_WIDTH - (4 * LS)) / 2;
    int height = SSP_COMPACT_HEIGHT - (5 * LS);
    moduleList_.setBounds(x, y, width, height);
    outputList_.setBounds(x + width + (2 * LS), y, width, height);
}


void MatrixView::drawView(Graphics& g) {
    int x = LS;
    int y = LS;
    int width = (SSP_COMPACT_WIDTH - (4 * LS)) / 2;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
    g.drawSingleLineText("Matrix ( Track " + String(trackIdx_ + 1) + " )", x, y);

    y = LS * 2;
    g.setColour(Colours::green);
    g.drawSingleLineText("Module Routing", x, y);


    g.setColour(Colours::red);
    g.drawSingleLineText("Output Routing", x + width + (2 * LS), y);

    MiniBasicView::drawView(g);
}

void MatrixView::onEncoder(unsigned enc, float v) {
    if (outputListSelected_) {
        if (v > 0)
            outputList_.nextItem();
        else
            outputList_.prevItem();
    }

    if (v > 0)
        moduleList_.nextItem();
    else
        moduleList_.prevItem();
}


void MatrixView::onListSwitchBtn() {
    outputListSelected_ = !outputListSelected_;
    moduleList_.idx(outputListSelected_ ? -1 : 0);
    outputList_.idx(!outputListSelected_ ? -1 : 0);
}

void MatrixView::onRemoveBtn() {
    auto& track = processor_.track(trackIdx_);
    if (outputListSelected_) {
        if (outputList_.idx() >= 0) {
            while (!track.requestMatrixOutputRemove(outputList_.idx()));
            trackIdx(trackIdx_);
        }
    } else {
        if (moduleList_.idx() >= 0) {
            // while(!track.requestMatrixModuleRemove(moduleList_.idx()));
            trackIdx(trackIdx_);
        }
    }
}

void MatrixView::onAddBtn() {
}