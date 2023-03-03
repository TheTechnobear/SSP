#include "MultiView.h"

#include "ssp/BaseProcessor.h"

#include "SSP.h"


namespace ssp {

MultiView::MultiView(BaseProcessor *p)
    : base_type(p) {
}


void MultiView::setView(unsigned newView) {
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
    view->setBounds(0, 0, 1600, 480);
    addChildComponent(view.get());
}

// default right/left buttons to view switch
// everything else is simple redirect
void MultiView::onLeftButton(bool v) {
    base_type::onLeftButton(v);
//    if(view_< 0) return;
//    views_[view_]->onLeftButton(v);
    previousView();
}

void MultiView::onRightButton(bool v) {
    base_type::onRightButton(v);
//    if(view_< 0) return;
//    views_[view_]->onRightButton(v);
    nextView();
}

void MultiView::drawView(Graphics &g) {
    base_type::drawView(g);
    if (view_ < 0) return;
    views_[view_]->drawView(g);
}
void MultiView::editorShown()  {
    base_type::editorShown();
    if (view_ < 0) return;
    views_[view_]->editorShown();
}

void MultiView::editorHidden()  {
    base_type::editorHidden();
    if (view_ < 0) return;
    views_[view_]->editorHidden();
}

void MultiView::resized() {
    base_type::resized();
    if (view_ < 0) return;
    views_[view_]->resized();
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

}//ssp