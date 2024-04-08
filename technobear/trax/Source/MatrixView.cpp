#include "MatrixView.h"

#include "Matrix.h"
#include "Track.h"


MatrixView::MatrixView(PluginProcessor& p) : base_type(&p, nullptr), processor_(p) {
    addButton(7, std::make_shared<ssp::ValueButton>("Delete", [&](bool b) {
                  if (!b) onRemoveBtn();
              }));
    addAndMakeVisible(connectionList_);
}

MatrixView::~MatrixView() {
}


String MatrixView::jackName(bool inOut, const Matrix::Jack& jack) {
    auto& track = processor_.track(trackIdx_);
    auto& module = track.modules_[jack.modIdx_];
    int ch = jack.chIdx_;

    if (module.plugin_ && module.descriptor_) {
        String str = module.pluginName_;
        str += "/";
        if (inOut)
            str += String(module.descriptor_->outputChannelNames[ch]);
        else
            str += String(module.descriptor_->inputChannelNames[ch]);
        return str;
    }
    return String("Empty-") + String(jack.modIdx_) + String("/") + String(ch + 1);
}

String MatrixView::wireAsString(Matrix::Wire& wire) {
    return jackName(true, wire.src_) + String(" -> ") + jackName(false, wire.dest_);
}


void MatrixView::trackIdx(unsigned t) {
    trackIdx_ = t;

    auto& track = processor_.track(trackIdx_);

    connectionList_.clear();
    auto connections = track.connections();

    for (auto& w : connections) { connectionList_.addItem(wireAsString(w)); }

    connectionList_.idx(0);
}


void MatrixView::resized() {
    MiniBasicView::resized();
    int x = LS;
    int y = LS * 3;
    int width = (SSP_COMPACT_WIDTH - (4 * LS)) / 2;
    int height = SSP_COMPACT_HEIGHT - (5 * LS);
    connectionList_.setBounds(x, y, width, height);
}


void MatrixView::drawView(Graphics& g) {
    base_type::drawView(g);
    int x = LS;
    int y = LS;
    int width = (SSP_COMPACT_WIDTH - (4 * LS)) / 2;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));
    g.drawSingleLineText("Matrix ( Track " + String(trackIdx_ + 1) + " )", x, y);

    MiniBasicView::drawView(g);
}

void MatrixView::onEncoder(unsigned enc, float v) {
    if (v > 0)
        connectionList_.nextItem();
    else
        connectionList_.prevItem();
}


void MatrixView::onRemoveBtn() {
    auto& track = processor_.track(trackIdx_);

    int idx = connectionList_.idx();
    auto connections = track.connections();

    for (auto& w : connections) {
        if (idx <= 0) {
            while (!track.requestMatrixDisconnect(w.src_, w.dest_));
            trackIdx(trackIdx_);  // redraw
            return;
        }
        idx--;
    }
}
