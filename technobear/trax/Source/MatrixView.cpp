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
    : base_type(&p, nullptr),
      processor_(p),
      moduleA_(p, 0, 0),
      moduleB_(p, 0, 0),
      channelA_([&](unsigned idx) { return isChASelected(idx); }),
      channelB_([&](unsigned idx) { return isChBSelected(idx); }) {
    addAndMakeVisible(moduleA_);
    addAndMakeVisible(channelA_);
    addAndMakeVisible(moduleB_);
    addAndMakeVisible(channelB_);


    // load is done in track editor!
    auto loadBtn = std::make_shared<ssp::ValueButton>("Load", [&](bool b) {});
    addButton(0, loadBtn);

    auto inBtn = std::make_shared<ssp::ValueButton>("Flip IO", [&](bool b) {
        isOutput_ = !b;
        refreshView();
    });
    inBtn->setToggle(true);
    addButton(3, inBtn);
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

    g.setColour(juce::Colours::yellow);
    int gap = 5 * COMPACT_UI_SCALE;
    int x = canvasX();
    int y = canvasY();
    int mh = canvasHeight() / 6;
    int gw = (canvasWidth() - gap) / 2;
    int mw = gw / 3;

    int lh = canvasHeight() - 2 * gap;
    float lx1 = x + (mw * 1.5f);

    float lx2 = lx1 + (mw * 1.0f);
    float ly = y + (mh / 4.0f);
    float arrowHead = mh / 2.0f;

    float lxo = isOutput_ ? lx1 : lx2;
    float lxe = isOutput_ ? lx2 : lx1;

    juce::Line<float> line(lxo, ly, lxe, ly);
    g.drawArrow(line, arrowHead / 2.0f, arrowHead, arrowHead);

    Matrix::Jack src(isOutput_ ? moduleAIdx_ : moduleBIdx_, isOutput_ ? channelA_.idx() : channelB_.idx());
    Matrix::Jack dest(isOutput_ ? moduleBIdx_ : moduleAIdx_, isOutput_ ? channelB_.idx() : channelA_.idx());
    for (auto& w : processor_.track(trackIdx_).connections()) {
        if (w.src_ == src && w.dest_ == dest) {
            g.setColour(juce::Colours::green);
            static constexpr int fh = 12 * COMPACT_UI_SCALE;
            g.setFont(fh);
            // display gain/offset
            ly += mh / 3;
            g.drawText("x" + juce::String(w.gain_, 2, false) + " +" + juce::String(w.offset_, 2, false), gw / 2, ly,
                       gw / 2, fh, juce::Justification::centredLeft);
            break;
        }
    }
}

void MatrixView::onEncoder(unsigned enc, float v) {
    switch (enc) {
        case 0: {
            if (v > 0) {
                int modB = moduleBIdx_ + 1;
                modB += moduleAIdx_ == modB;
                if (modB < Track::MAX_MODULES) {
                    moduleBIdx_ = modB;
                    moduleB_.slotIdx(moduleBIdx_);
                    refreshModuleB();
                }
            } else {
                int modB = moduleBIdx_ - 1;
                modB -= moduleAIdx_ == modB;
                if (moduleBIdx_ > 0) {
                    moduleBIdx_ = modB;
                    moduleB_.slotIdx(moduleBIdx_);
                    refreshModuleB();
                }
            }
            break;
        }
        case 1: {
            if (v > 0)
                channelA_.nextItem();
            else
                channelA_.prevItem();
            refreshSelected();
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
            updateGainOffset(encoderState_[enc], v);
            break;
        }
        default: {
            break;
        }
    }
}


void MatrixView::onEncoderSwitch(unsigned enc, bool v) {
    encoderState_[enc] = v;
    if (v) return;

    switch (enc) {
        case 0: {
            break;
        }
        case 1: {
            break;
        }
        case 2: {
            // for now, do not allow connections to self
            if (moduleAIdx_ == moduleBIdx_) return;
            if (chASelected_.size() <= 0 || chBSelected_.size() <= 0) return;

            if (channelA_.idx() >= 0 && moduleBIdx_ >= 0 && channelB_.idx() >= 0) {
                Matrix::Jack src(isOutput_ ? moduleAIdx_ : moduleBIdx_, isOutput_ ? channelA_.idx() : channelB_.idx());
                Matrix::Jack dest(isOutput_ ? moduleBIdx_ : moduleAIdx_, isOutput_ ? channelB_.idx() : channelA_.idx());
                if (!chBSelected_[channelB_.idx()]) {
                    // add new connection
                    while (!processor_.track(trackIdx_).requestMatrixConnect(src, dest));
                } else {
                    // remove connection.
                    while (!processor_.track(trackIdx_).requestMatrixDisconnect(src, dest));
                }
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

void MatrixView::updateGainOffset(bool isOffset, float delta) {
    Matrix::Jack src(isOutput_ ? moduleAIdx_ : moduleBIdx_, isOutput_ ? channelA_.idx() : channelB_.idx());
    Matrix::Jack dest(isOutput_ ? moduleBIdx_ : moduleAIdx_, isOutput_ ? channelB_.idx() : channelA_.idx());
    while (!processor_.track(trackIdx_).requestMatrixAttenuate(src, dest, isOffset, delta / 100.0f));
}

void MatrixView::editorShown() {
    refreshView();
    base_type::editorShown();
}

void MatrixView::refreshView() {
    channelA_.clear();
    chASelected_.clear();

    auto& track = processor_.track(trackIdx_);
    auto& module = track.modules_[moduleAIdx_];

    if (module.plugin_ && module.descriptor_) {
        channelA_.clear();
        // module A
        if (isOutput_) {
            auto& outputs = module.descriptor_->outputChannelNames;
            for (int i = 0; i < outputs.size(); i++) {
                channelA_.addItem(channelAsString(moduleAIdx_, i, isOutput_));
                chASelected_.push_back(false);
            }
        } else {
            auto& inputs = module.descriptor_->inputChannelNames;
            for (int i = 0; i < inputs.size(); i++) {
                channelA_.addItem(channelAsString(moduleAIdx_, i, isOutput_));
                chASelected_.push_back(false);
            }
        }
    }
    refreshModuleB();
}

void MatrixView::refreshModuleB() {
    channelB_.clear();
    chBSelected_.clear();


    if (moduleBIdx_ >= 0 && moduleBIdx_ < Track::M_MAX) {
        auto& track = processor_.track(trackIdx_);
        auto& moduleB = track.modules_[moduleBIdx_];
        if (moduleB.descriptor_ == nullptr) return;

        if (isOutput_) {
            auto& inputs = moduleB.descriptor_->inputChannelNames;
            for (int i = 0; i < inputs.size(); i++) {
                channelB_.addItem(channelAsString(moduleBIdx_, i, !isOutput_));
                chBSelected_.push_back(false);
            }

        } else {
            auto& outputs = moduleB.descriptor_->outputChannelNames;
            for (int i = 0; i < outputs.size(); i++) {
                channelB_.addItem(channelAsString(moduleBIdx_, i, !isOutput_));
                chBSelected_.push_back(false);
            }
        }

        refreshSelected();
    }
}


void MatrixView::refreshSelected() {
    auto& track = processor_.track(trackIdx_);

    // reset all selected vectors
    for (unsigned i = 0; i < chASelected_.size(); i++) { chASelected_[i] = false; }
    for (unsigned i = 0; i < chBSelected_.size(); i++) { chBSelected_[i] = false; }

    for (auto& w : track.connections()) {
        Matrix::Jack jackA(moduleAIdx_, channelA_.idx());
        if (isOutput_) {
            if (w.src_.modIdx_ == moduleAIdx_) { chASelected_[w.src_.chIdx_] = true; }
            if (w.src_ == jackA && w.dest_.modIdx_ == moduleBIdx_) { chBSelected_[w.dest_.chIdx_] = true; }
        } else {
            if (w.dest_.modIdx_ == moduleAIdx_) { chASelected_[w.dest_.chIdx_] = true; }
            if (w.dest_ == jackA && w.src_.modIdx_ == moduleBIdx_) { chBSelected_[w.src_.chIdx_] = true; }
        }
    }
    repaint();
}


bool MatrixView::isChASelected(unsigned idx) {
    return idx < chASelected_.size() && chASelected_[idx];
}


bool MatrixView::isChBSelected(unsigned idx) {
    return idx < chBSelected_.size() && chBSelected_[idx];
}
