#include "AddRouteView.h"

#include "Matrix.h"
#include "Track.h"


AddRouteView::AddRouteView(PluginProcessor& p) : ssp::MiniBasicView(&p, nullptr), processor_(p) {
    addButton(3, std::make_shared<ssp::ValueButton>("Prev", [&](bool b) {
                  if (!b) onPrevBtn();
              }));
    addButton(7, std::make_shared<ssp::ValueButton>("Next", [&](bool b) {
                  if (!b) onNextBtn();
              }));
    addAndMakeVisible(listCtrl_);
}

AddRouteView::~AddRouteView() {
}


void AddRouteView::drawView(Graphics& g) {
    int x = LS;
    int y = LS;
    int width = (SSP_COMPACT_WIDTH - (4 * LS)) / 2;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
    g.drawSingleLineText("AddRoute ( Track " + String(trackIdx_ + 1) + " )", x, y);

    MiniBasicView::drawView(g);
}

void AddRouteView::trackIdx(unsigned t) {
    trackIdx_ = t;
    modeIdx_ = LM_SRC;
    srcIdx_ = 0;
    srcChIdx_ = -1;
    destChIdx_ = -1;
    setupList();
}

void AddRouteView::resized() {
    MiniBasicView::resized();
    int x = LS;
    int y = LS * 3;
    int width = (SSP_COMPACT_WIDTH - (4 * LS)) / 2;
    int height = SSP_COMPACT_HEIGHT - (5 * LS);
    listCtrl_.setBounds(x, y, width, height);
}

void AddRouteView::onPrevBtn() {
    switch (modeIdx_) {
        case LM_SRC: {
            // ignore (at beginning!)
            break;
        }
        case LM_SRC_CH: {
            modeIdx_ = LM_SRC;
            srcChIdx_ = -1;
            setupList();
            break;
        }
        case LM_DEST: {
            modeIdx_ = LM_SRC_CH;
            destIdx_ = -1;
            setupList();
            break;
        }
        case LM_DEST_CH: {
            modeIdx_ = LM_DEST;
            destChIdx_ = -1;
            setupList();
            break;
        }
        default: {
            break;
        }
    }
}
void AddRouteView::onNextBtn() {
    switch (modeIdx_) {
        case LM_SRC: {
            srcIdx_ = listCtrl_.idx();
            modeIdx_ = LM_SRC_CH;
            setupList();
            break;
        }
        case LM_SRC_CH: {
            srcChIdx_ = listCtrl_.idx();
            modeIdx_ = LM_DEST;
            setupList();
            break;
        }
        case LM_DEST: {
            destIdx_ = listCtrl_.idx();
            modeIdx_ = LM_DEST_CH;
            setupList();
            break;
        }
        case LM_DEST_CH: {
            destChIdx_ = listCtrl_.idx();
            addRoute(trackIdx_, srcIdx_, srcChIdx_, destIdx_, destChIdx_);
            break;
        }
        default: {
            break;
        }
    }
}

void AddRouteView::onEncoder(unsigned enc, float v) {
    if (v > 0)
        listCtrl_.nextItem();
    else
        listCtrl_.prevItem();
}


void AddRouteView::addRoute(int trackIdx, int srcIdx, int srcChIdx, int destIdx, int destChIdx) {
    auto& track = processor_.track(trackIdx_);
    while (!track.requestMatrixConnect(Matrix::Jack(srcIdx, srcChIdx),Matrix::Jack(destIdx,destChIdx)));
    AddRouteView::trackIdx(trackIdx_); // reset!
}


void AddRouteView::setupList() {
    listCtrl_.clear();
    switch (modeIdx_) {
        case LM_SRC: {
            for (int i = 0; i < Track::M_MAX; i++) { listCtrl_.addItem(srcName(i)); }
            break;
        }
        case LM_SRC_CH: {
            int modIdx = srcIdx_;
            if (modIdx >= 0 && modIdx < Track::M_MAX) {
                auto& track = processor_.track(trackIdx_);
                auto& module = track.modules_[modIdx];
                if (module.plugin_ && module.descriptor_) {
                    int sz = module.descriptor_->outputChannelNames.size();
                    for (int i = 0; i < sz; i++) { listCtrl_.addItem(channelName(true, modIdx, i)); }
                }
            }
            break;
        }
        case LM_DEST: {
            for (int i = 0; i < Track::M_MAX; i++) { listCtrl_.addItem(srcName(i)); }
            break;
        }
        case LM_DEST_CH: {
            int modIdx = destIdx_;
            if (modIdx >= 0 && modIdx < Track::M_MAX) {
                auto& track = processor_.track(trackIdx_);
                auto& module = track.modules_[modIdx];
                if (module.plugin_ && module.descriptor_) {
                    int sz = module.descriptor_->inputChannelNames.size();
                    for (int i = 0; i < sz; i++) { listCtrl_.addItem(channelName(false, modIdx, i)); }
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}


String AddRouteView::channelName(bool inOut, unsigned mod, unsigned ch) {
    if (mod < Track::M_MAX) {
        auto& track = processor_.track(trackIdx_);
        auto& module = track.modules_[mod];
        if (module.plugin_ && module.descriptor_) {
            String str = "";
            if (inOut)
                str += String(module.descriptor_->outputChannelNames[ch]);
            else
                str += String(module.descriptor_->inputChannelNames[ch]);
            return str;
        }
        return String("Unknown/") + String(ch + 1);
    } else {
        return "Error/"+String(ch);
    }
}

String AddRouteView::srcName(unsigned mod) {
    if (mod < Track::M_MAX) {
        auto& track = processor_.track(trackIdx_);
        auto& module = track.modules_[mod];
        if (module.plugin_ && module.descriptor_) {
            return module.pluginName_;
        } else {
            return String("Empty-")+String(mod);
        }
         
    }
    return "Error-"+ String(mod);
}
