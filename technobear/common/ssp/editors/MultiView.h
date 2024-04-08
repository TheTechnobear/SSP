#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

#include <vector>

#include "BaseEditor.h"

namespace ssp {

class BaseProcessor;

template <class T>
class MultiView : public BaseView {
public:
    explicit MultiView(BaseProcessor *p, bool compactUI);

    void onEncoder(unsigned id, float v) override;
    void onEncoderSwitch(unsigned id, bool v) override;
    void onButton(unsigned id, bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    void onLeftShiftButton(bool v) override;
    void onRightShiftButton(bool v) override;

    void eventButton(unsigned btn, bool longPress) override;
    void eventUp(bool longPress) override;
    void eventDown(bool longPress) override;
    void eventLeft(bool longPress) override;
    void eventRight(bool longPress) override;
    void eventLeftShift(bool longPress) override;
    void eventRightShift(bool longPress) override;
    void eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) override;

    void editorShown() override;
    void editorHidden() override;

    int getViewIdx() { return view_; }

    void onSSPTimer() override;
protected:

    void drawView(juce::Graphics &g) override;
    void resized() override;
    unsigned addView(std::shared_ptr<T>);

    virtual void setView(unsigned newView);
    void nextView();
    void previousView();

    std::vector<std::shared_ptr<T>> views_;

    int view_ = -1;

private:
    using base_type = BaseView;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiView)
};


template <class T>
MultiView<T>::MultiView(BaseProcessor *p, bool compactUI) : base_type(p, compactUI) {
}

template <class T>
void MultiView<T>::onSSPTimer() {
    for(auto& v: views_) {
        v->onSSPTimer();
    }
}


template <class T>
void MultiView<T>::setView(unsigned newView) {
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


template <class T>
void MultiView<T>::nextView() {
    int newView = view_++;
    if (newView < views_.size()) { setView(newView); }
}

template <class T>
void MultiView<T>::previousView() {
    int newView = view_--;
    if (newView >= 0) { setView(newView); }
}

template <class T>
unsigned MultiView<T>::addView(std::shared_ptr<T> view) {
    views_.push_back(view);
    addChildComponent(view.get());
    return views_.size() - 1;
}

template <class T>
void MultiView<T>::drawView(Graphics &g) {
    base_type::drawView(g);
    if (view_ < 0) return;
    views_[view_]->drawView(g);
}

template <class T>
void MultiView<T>::editorShown() {
    base_type::editorShown();
    int i = 0;
    for (auto v : views_) {
        if (view_ == i) {
            v->setVisible(true);
            v->editorShown();
        } else {
            v->setVisible(false);
            v->editorHidden();
        }
    }
}

template <class T>
void MultiView<T>::editorHidden() {
    base_type::editorHidden();
    for (auto v : views_) {
        v->setVisible(false);
        v->editorHidden();
    }
}

template <class T>
void MultiView<T>::resized() {
    base_type::resized();
    for (auto v : views_) {
        v->setBounds(0, 0, getWidth(), getHeight());
        v->resized();
    }
}

template <class T>
void MultiView<T>::onEncoder(unsigned id, float v) {
    base_type::onEncoder(id, v);
    if (view_ < 0) return;
    views_[view_]->onEncoder(id, v);
}

template <class T>
void MultiView<T>::onEncoderSwitch(unsigned id, bool v) {
    base_type::onEncoderSwitch(id, v);
    if (view_ < 0) return;
    views_[view_]->onEncoderSwitch(id, v);
}

template <class T>
void MultiView<T>::onButton(unsigned id, bool v) {
    base_type::onButton(id, v);
    if (view_ < 0) return;
    views_[view_]->onButton(id, v);
}

template <class T>
void MultiView<T>::onLeftButton(bool v) {
    base_type::onLeftButton(v);
    if (view_ < 0) return;
    views_[view_]->onLeftButton(v);
}

template <class T>
void MultiView<T>::onRightButton(bool v) {
    base_type::onRightButton(v);
    if (view_ < 0) return;
    views_[view_]->onRightButton(v);
}

template <class T>
void MultiView<T>::onUpButton(bool v) {
    base_type::onUpButton(v);
    if (view_ < 0) return;
    views_[view_]->onUpButton(v);
}

template <class T>
void MultiView<T>::onDownButton(bool v) {
    base_type::onDownButton(v);
    if (view_ < 0) return;
    views_[view_]->onDownButton(v);
}

template <class T>
void MultiView<T>::onRightShiftButton(bool v) {
    base_type::onRightShiftButton(v);
    if (view_ < 0) return;
    views_[view_]->onRightShiftButton(v);
}

template <class T>
void MultiView<T>::onLeftShiftButton(bool v) {
    base_type::onLeftShiftButton(v);
    if (view_ < 0) return;
    views_[view_]->onLeftShiftButton(v);
}


template <class T>
void MultiView<T>::eventButton(unsigned btn, bool longPress) {
    base_type::eventButton(btn, longPress);
    if (view_ < 0) return;
    views_[view_]->eventButton(btn, longPress);
}

template <class T>
void MultiView<T>::eventUp(bool longPress) {
    base_type::eventUp(longPress);
    if (view_ < 0) return;
    views_[view_]->eventUp(longPress);
}

template <class T>
void MultiView<T>::eventDown(bool longPress) {
    base_type::eventDown(longPress);
    if (view_ < 0) return;
    views_[view_]->eventDown(longPress);
}

template <class T>
void MultiView<T>::eventLeft(bool longPress) {
    base_type::eventLeft(longPress);
    if (view_ < 0) return;
    views_[view_]->eventLeft(longPress);
}

template <class T>
void MultiView<T>::eventRight(bool longPress) {
    base_type::eventRight(longPress);
    if (view_ < 0) return;
    views_[view_]->eventRight(longPress);
}

template <class T>
void MultiView<T>::eventLeftShift(bool longPress) {
    base_type::eventLeftShift(longPress);
    if (view_ < 0) return;
    views_[view_]->eventLeftShift(longPress);
}

template <class T>
void MultiView<T>::eventRightShift(bool longPress) {
    base_type::eventRightShift(longPress);
    if (view_ < 0) return;
    views_[view_]->eventRightShift(longPress);
}

template <class T>
void MultiView<T>::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    base_type::eventButtonCombo(btn, comboBtn, longPress);
    if (view_ < 0) return;
    views_[view_]->eventButtonCombo(btn, comboBtn, longPress);
}


}  // namespace ssp
