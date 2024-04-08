#include "BaseViewEditor.h"

#include "SSP.h"
#include "ssp/BaseProcessor.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"

namespace ssp {

BaseViewEditor::BaseViewEditor(BaseProcessor* p) : base_type(p) {
    setButtonBounds(leftBtn_, 1, 4);
    addAndMakeVisible(leftBtn_);

    setButtonBounds(rightBtn_, 1, 6);
    addAndMakeVisible(rightBtn_);
}


void BaseViewEditor::setView(unsigned newView) {
    if (view_ >= 0) {
        auto& view = views_[view_];
        view->setVisible(false);
        view->editorHidden();
    }
    if (newView < views_.size()) {
        view_ = newView;
        auto& view = views_[view_];
        view->setVisible(true);
        view->editorShown();
    }
}


void BaseViewEditor::nextView() {
    int newView = view_++;
    if (newView < views_.size()) { setView(newView); }
}

void BaseViewEditor::previousView() {
    int newView = view_--;
    if (newView >= 0) { setView(newView); }
}

unsigned  BaseViewEditor::addView(std::shared_ptr<view_type> view) {
    views_.push_back(view);
    view->setBounds(0, 0, 1600, 480);
    addChildComponent(view.get());
    return views_.size() - 1;
}

// default right/left buttons to view switch
// everything else is simple redirect
void BaseViewEditor::onLeftButton(bool v) {
    base_type::onLeftButton(v);
    //    if(view_< 0) return;
    //    views_[view_]->onLeftButton(v);
    previousView();
}

void BaseViewEditor::onRightButton(bool v) {
    base_type::onRightButton(v);
    //    if(view_< 0) return;
    //    views_[view_]->onRightButton(v);
    nextView();
}

void BaseViewEditor::drawView(Graphics& g) {
    base_type::drawView(g);
    if (view_ < 0) return;
    views_[view_]->drawView(g);
}
void BaseViewEditor::editorShown() {
    base_type::editorShown();
    if (view_ < 0) return;
    views_[view_]->editorShown();
}

void BaseViewEditor::editorHidden() {
    base_type::editorHidden();
    if (view_ < 0) return;
    views_[view_]->editorHidden();
}

void BaseViewEditor::resized() {
    base_type::resized();
    if (view_ < 0) return;
    views_[view_]->resized();
}


void BaseViewEditor::onEncoder(unsigned id, float v) {
    base_type::onEncoder(id, v);
    if (view_ < 0) return;
    views_[view_]->onEncoder(id, v);
}

void BaseViewEditor::onEncoderSwitch(unsigned id, bool v) {
    base_type::onEncoderSwitch(id, v);
    if (view_ < 0) return;
    views_[view_]->onEncoderSwitch(id, v);
}

void BaseViewEditor::onButton(unsigned id, bool v) {
    base_type::onButton(id, v);
    if (view_ < 0) return;
    views_[view_]->onButton(id, v);
}

void BaseViewEditor::onUpButton(bool v) {
    base_type::onUpButton(v);
    if (view_ < 0) return;
    views_[view_]->onUpButton(v);
}

void BaseViewEditor::onDownButton(bool v) {
    base_type::onDownButton(v);
    if (view_ < 0) return;
    views_[view_]->onDownButton(v);
}

void BaseViewEditor::onRightShiftButton(bool v) {
    base_type::onRightShiftButton(v);
    if (view_ < 0) return;
    views_[view_]->onRightShiftButton(v);
}

void BaseViewEditor::onLeftShiftButton(bool v) {
    base_type::onLeftShiftButton(v);
    if (view_ < 0) return;
    views_[view_]->onLeftShiftButton(v);
}


void BaseViewEditor::eventButton(unsigned btn, bool longPress) {
    base_type::eventButton(btn, longPress);
    if (view_ < 0) return;
    views_[view_]->eventButton(btn, longPress);
}

void BaseViewEditor::eventUp(bool longPress) {
    base_type::eventUp(longPress);
    if (view_ < 0) return;
    views_[view_]->eventUp(longPress);
}

void BaseViewEditor::eventDown(bool longPress) {
    base_type::eventDown(longPress);
    if (view_ < 0) return;
    views_[view_]->eventDown(longPress);
}

void BaseViewEditor::eventLeft(bool longPress) {
    base_type::eventLeft(longPress);
    if (view_ < 0) return;
    views_[view_]->eventLeft(longPress);
}

void BaseViewEditor::eventRight(bool longPress) {
    base_type::eventRight(longPress);
    if (view_ < 0) return;
    views_[view_]->eventRight(longPress);
}

void BaseViewEditor::eventLeftShift(bool longPress) {
    base_type::eventLeftShift(longPress);
    if (view_ < 0) return;
    views_[view_]->eventLeftShift(longPress);
}

void BaseViewEditor::eventRightShift(bool longPress) {
    base_type::eventRightShift(longPress);
    if (view_ < 0) return;
    views_[view_]->eventRightShift(longPress);
}

void BaseViewEditor::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn, comboBtn, longPress);
    if (view_ < 0) return;
    views_[view_]->eventButtonCombo(btn, comboBtn, longPress);
}


}  // namespace ssp