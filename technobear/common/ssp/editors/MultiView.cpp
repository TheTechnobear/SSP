#include "MultiView.h"

#include "ssp/BaseProcessor.h"

#include "SSP.h"


namespace ssp {

MultiView::MultiView(BaseProcessor *p, bool compactUI)
    : base_type(p, compactUI) {
}


void MultiView::setView(unsigned newView) {
    // do not setVisible outside message thread !
    if (view_ >= 0) {
        auto &view = views_[view_];
        view->setVisible(false);
        view->editorHidden();
    }

    if (newView < views_.size()) {
        view_ = newView;
        auto &view = views_[view_];
        view->setVisible(true);
        view->editorShown();
    }
}


void MultiView::nextView() {
    int newView = view_++;
    if (newView < views_.size()) {
        setView(newView);
    }
}

void MultiView::previousView() {
    int newView = view_--;
    if (newView >= 0) {
        setView(newView);
    }
}

void MultiView::addView(std::shared_ptr<base_type> view) {
    views_.push_back(view);
    addChildComponent(view.get());
}

void MultiView::drawView(Graphics &g) {
    base_type::drawView(g);
    if (view_ < 0) return;
    views_[view_]->drawView(g);
}

void MultiView::editorShown() {
    base_type::editorShown();
    int i = 0;
    for (auto v: views_) {
        if (view_ == i) {
            v->setVisible(true);
            v->editorShown();
        } else {
            v->setVisible(false);
            v->editorHidden();
        }
    }
}

void MultiView::editorHidden() {
    base_type::editorHidden();
    for (auto v: views_) {
        v->setVisible(false);
        v->editorHidden();
    }
}

void MultiView::resized() {
    base_type::resized();
    for (auto v: views_) {
        v->setBounds(0, 0, getWidth(), getHeight());
        v->resized();
    }
}


void MultiView::onEncoder(unsigned id, float v) {
    base_type::onEncoder(id, v);
    if (view_ < 0) return;
    views_[view_]->onEncoder(id, v);
}

void MultiView::onEncoderSwitch(unsigned id, bool v) {
    base_type::onEncoderSwitch(id, v);
    if (view_ < 0) return;
    views_[view_]->onEncoderSwitch(id, v);
}

void MultiView::onButton(unsigned id, bool v) {
    base_type::onButton(id, v);
    if (view_ < 0) return;
    views_[view_]->onButton(id, v);
}

void MultiView::onLeftButton(bool v) {
    base_type::onLeftButton(v);
    if (view_ < 0) return;
    views_[view_]->onLeftButton(v);
}

void MultiView::onRightButton(bool v) {
    base_type::onRightButton(v);
    if (view_ < 0) return;
    views_[view_]->onRightButton(v);
}

void MultiView::onUpButton(bool v) {
    base_type::onUpButton(v);
    if (view_ < 0) return;
    views_[view_]->onUpButton(v);
}

void MultiView::onDownButton(bool v) {
    base_type::onDownButton(v);
    if (view_ < 0) return;
    views_[view_]->onDownButton(v);
}

void MultiView::onRightShiftButton(bool v) {
    base_type::onRightShiftButton(v);
    if (view_ < 0) return;
    views_[view_]->onRightShiftButton(v);
}

void MultiView::onLeftShiftButton(bool v) {
    base_type::onLeftShiftButton(v);
    if (view_ < 0) return;
    views_[view_]->onLeftShiftButton(v);
}


void MultiView::eventButton(unsigned btn, bool longPress) {
    base_type::eventButton(btn, longPress);
    if (view_ < 0) return;
    views_[view_]->eventButton(btn, longPress);
}

void MultiView::eventUp(bool longPress) {
    base_type::eventUp(longPress);
    if (view_ < 0) return;
    views_[view_]->eventUp(longPress);
}

void MultiView::eventDown(bool longPress) {
    base_type::eventDown(longPress);
    if (view_ < 0) return;
    views_[view_]->eventDown(longPress);
}

void MultiView::eventLeft(bool longPress) {
    base_type::eventLeft(longPress);
    if (view_ < 0) return;
    views_[view_]->eventLeft(longPress);
}

void MultiView::eventRight(bool longPress) {
    base_type::eventRight(longPress);
    if (view_ < 0) return;
    views_[view_]->eventRight(longPress);
}

void MultiView::eventLeftShift(bool longPress) {
    base_type::eventLeftShift(longPress);
    if (view_ < 0) return;
    views_[view_]->eventLeftShift(longPress);
}

void MultiView::eventRightShift(bool longPress) {
    base_type::eventRightShift(longPress);
    if (view_ < 0) return;
    views_[view_]->eventRightShift(longPress);
}

void MultiView::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn, comboBtn, longPress);
    if (view_ < 0) return;
    views_[view_]->eventButtonCombo(btn, comboBtn, longPress);
}


}//ssp