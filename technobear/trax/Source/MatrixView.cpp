#include "MatrixView.h"

#include "Matrix.h"
#include "Track.h"


// todo
// need to create multi selection listcontrol


// notes
// channel A and module B can be 'anything'
// for a connection we have a 'selection'in all 3 chA, modB, chB
// if we select chB then we add the connection
// if we deselet chB then we remove the connection

// next/prev on chA updates module B, including showing any with connections
// next/prev on module B updates chB, including showing any with connections


MatrixView::MatrixView(PluginProcessor& p)
    : base_type(&p, nullptr), processor_(p), moduleA_(p, 0, 0), moduleB_(p, 0, 0) {
    //   channelA_([&](unsigned idx) { return isChASelected(idx); }),
    //   moduleB_([&](unsigned idx) { return isModuleBSelected(idx); }),
    //   channelB_([&](unsigned idx) { return isChBSelected(idx); }) {
    addAndMakeVisible(moduleA_);
    addAndMakeVisible(channelA_);


    addAndMakeVisible(moduleB_);
    addAndMakeVisible(channelB_);


    auto inBtn = std::make_shared<ssp::ValueButton>("Inputs", [&](bool b) {
        isOutput_ = !b;
        refreshView();
    });
    inBtn->setToggle(true);
    addButton(7, inBtn);
}

MatrixView::~MatrixView() {
}


String MatrixView::channelAsString(unsigned midx, unsigned ch, bool inOut) {
    if (midx == Track::M_IN) {
        return "IN " + String(ch);
    } else if (midx == Track::M_OUT) {
        return "OUT " + String(ch);
    } else {
        auto& track = processor_.track(trackIdx_);
        auto& module = track.modules_[midx];
        if (module.plugin_ && module.descriptor_) {
            if (inOut) {
                if (ch >= module.descriptor_->outputChannelNames.size()) return "Invalid " + String(ch);
                return String(module.descriptor_->outputChannelNames[ch]);
            } else {
                if (ch >= module.descriptor_->inputChannelNames.size()) return "Invalid" + String(ch);
                return String(module.descriptor_->inputChannelNames[ch]);
            }
        }
        return "Empty" + String(ch);
    }
}


void MatrixView::resized() {
    base_type::resized();
    int gap = 5 * COMPACT_UI_SCALE;
    int x = canvasX();
    int y = canvasY();
    int mh = canvasHeight() / 6;
    int gw = (canvasWidth() - gap) / 2;
    int mw = gw / 3;
    int lh = canvasHeight() - 2 * gap;
    moduleA_.setBounds(x, y, mw, mh);
    channelA_.setBounds(x, y + mh + gap, gw, lh - mh - gap);

    x = x + gw + gap;
    moduleB_.setBounds(x, y, mw, mh);
    channelB_.setBounds(x, y + mh + gap, gw, lh - mh - gap);
}


void MatrixView::drawView(Graphics& g) {
    base_type::drawView(g);
    int fh = 14 * COMPACT_UI_SCALE;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
    // g.drawSingleLineText("Matrix ( Track " + String(trackIdx_ + 1) + " , Slot " + moduleAsString(moduleIdx_) + " )",
    // x,
    //                      y);

    MiniBasicView::drawView(g);

    int gap = 5 * COMPACT_UI_SCALE;
    int x = canvasX();
    int y = canvasY();
    int mh = canvasHeight() / 6;
    int gw = (canvasWidth() - gap) / 2;
    int mw = gw / 3;

    int lh = canvasHeight() - 2 * gap;
    float lx1 = x + (mw * 1.5f);

    float lx2 = lx1 + (mw * 1.0f);
    float ly = y + (mh / 2.0f);
    float arrowHead = mh / 2.0f;

    float lxo = isOutput_ ? lx1 : lx2;
    float lxe = isOutput_ ? lx2 : lx1;

    juce::Line<float> line(lxo, ly, lxe, ly);
    g.drawArrow(line, arrowHead / 2.0f, arrowHead, arrowHead);
}

void MatrixView::onEncoder(unsigned enc, float v) {
    switch (enc) {
        case 0: {
            if (v > 0)
                channelA_.nextItem();
            else
                channelA_.prevItem();
            refreshSelected();
            break;
        }
        case 1: {
            if (v > 0) {
                if (moduleBIdx_ < (Track::MAX_MODULES - 1)) {
                    moduleBIdx_++;
                    moduleB_.slotIdx(moduleBIdx_);
                    refreshModuleB();
                }
            } else {
                if (moduleBIdx_ > 0) {
                    moduleBIdx_--;
                    moduleB_.slotIdx(moduleBIdx_);
                    refreshModuleB();
                }
            }
            // if (v > 0)
            //     moduleB_.nextItem();
            // else
            //     moduleB_.prevItem();
            // refreshModuleB();
            break;
        }
        case 2: {
            if (v > 0)
                channelB_.nextItem();
            else
                channelB_.prevItem();
            break;
        }
        case 3: {
            break;
        }
        default: {
            break;
        }
    }
}


void MatrixView::onEncoderSwitch(unsigned enc, bool v) {
    if (v) return;

    switch (enc) {
        case 0: {
            // repaint();
            break;
        }
        case 1: {
            // repaint();
            break;
        }
        case 2: {
            // for now, do not allow connections to self
            if (moduleAIdx_ == moduleBIdx_) return;

            if (channelA_.idx() >= 0 && moduleBIdx_ >= 0 && channelB_.idx() >= 0) {
                Matrix::Jack jackA(moduleAIdx_, channelA_.idx());
                Matrix::Jack jackB(moduleBIdx_, channelB_.idx());
                // if (!chBSelected_[channelB_.idx()]) {
                //     // add new connection
                //     if (isOutput_)
                //         while (!processor_.track(trackIdx_).requestMatrixConnect(jackA, jackB));
                //     else
                //         while (!processor_.track(trackIdx_).requestMatrixConnect(jackB, jackA));
                // } else {
                //     // remove connection.
                //     if (isOutput_)
                //         while (!processor_.track(trackIdx_).requestMatrixDisconnect(jackA, jackB));
                //     else
                //         while (!processor_.track(trackIdx_).requestMatrixDisconnect(jackB, jackA));
                // }
                refreshSelected();
            }
            repaint();
            break;
        }
        case 3: {
            break;
        }
        default: {
            break;
        }
    }
}


void MatrixView::editorShown() {
    refreshView();
}

void MatrixView::refreshView() {
    // channelA_.clear();
    // chASelected_.clear();
    // moduleB_.clear();
    // moduleBSelected_.clear();
    // moduleBIdx_ = -1;

    auto& track = processor_.track(trackIdx_);
    auto& module = track.modules_[moduleAIdx_];

    if (module.plugin_ && module.descriptor_) {
        channelA_.clear();
        // module A
        if (isOutput_) {
            auto& outputs = module.descriptor_->outputChannelNames;
            for (int i = 0; i < outputs.size(); i++) {
                channelA_.addItem(channelAsString(moduleAIdx_, i, isOutput_));
                // chASelected_.push_back(false);
            }
        } else {
            auto& inputs = module.descriptor_->inputChannelNames;
            for (int i = 0; i < inputs.size(); i++) {
                channelA_.addItem(channelAsString(moduleAIdx_, i, isOutput_));
                // chASelected_.push_back(false);
            }
        }
    }


    //     // module B
    //     for (int i = 0; i < Track::M_MAX; i++) {
    //         moduleB_.addItem(moduleAsString(i));
    //         moduleBSelected_.push_back(false);
    //     }

    //     // moduleB_.idx(0);
    refreshModuleB();
    //     refreshSelected();
    // }
}

void MatrixView::refreshModuleB() {
    // int moduleBIdx = moduleB_.idx();
    // if (moduleBIdx_ == moduleBIdx) { return; }

    // moduleBIdx_ = moduleBIdx;
    channelB_.clear();
    // chBSelected_.clear();


    if (moduleBIdx_ >= 0 && moduleBIdx_ < Track::M_MAX) {
        auto& track = processor_.track(trackIdx_);
        auto& moduleB = track.modules_[moduleBIdx_];
        if (moduleB.descriptor_ == nullptr) return;

        if (isOutput_) {
            auto& inputs = moduleB.descriptor_->inputChannelNames;
            for (int i = 0; i < inputs.size(); i++) {
                channelB_.addItem(channelAsString(moduleBIdx_, i, !isOutput_));
                // chBSelected_.push_back(false);
            }

        } else {
            auto& outputs = moduleB.descriptor_->outputChannelNames;
            for (int i = 0; i < outputs.size(); i++) {
                channelB_.addItem(channelAsString(moduleBIdx_, i, !isOutput_));
                // chBSelected_.push_back(false);
            }
        }

        refreshSelected();
    }
}


void MatrixView::refreshSelected() {
    // auto& track = processor_.track(trackIdx_);

    // // reset all selected vectors
    // for (unsigned i = 0; i < chASelected_.size(); i++) { chASelected_[i] = false; }
    // for (unsigned i = 0; i < moduleBSelected_.size(); i++) { moduleBSelected_[i] = false; }
    // for (unsigned i = 0; i < chBSelected_.size(); i++) { chBSelected_[i] = false; }

    // for (auto& w : track.connections()) {
    //     Matrix::Jack jackA(moduleIdx_, channelA_.idx());
    //     if (isOutput_) {
    //         if (w.src_.modIdx_ == moduleIdx_) { chASelected_[w.src_.chIdx_] = true; }
    //         if (w.src_ == jackA) { moduleBSelected_[w.dest_.modIdx_] = true; }
    //         if (w.src_ == jackA && w.dest_.modIdx_ == moduleB_.idx()) { chBSelected_[w.dest_.chIdx_] = true; }
    //     } else {
    //         if (w.dest_.modIdx_ == moduleIdx_) { chASelected_[w.dest_.chIdx_] = true; }
    //         if (w.dest_ == jackA) { moduleBSelected_[w.src_.modIdx_] = true; }
    //         if (w.dest_ == jackA && w.src_.modIdx_ == moduleB_.idx()) { chBSelected_[w.src_.chIdx_] = true; }
    //     }
    // }
    // repaint();
}


// bool MatrixView::isChASelected(unsigned idx) {
//     return idx < chASelected_.size() && chASelected_[idx];
// }

// bool MatrixView::isModuleBSelected(unsigned idx) {
//     return idx < moduleBSelected_.size() && moduleBSelected_[idx];
// }

// bool MatrixView::isChBSelected(unsigned idx) {
//     return idx < chBSelected_.size() && chBSelected_[idx];
// }
