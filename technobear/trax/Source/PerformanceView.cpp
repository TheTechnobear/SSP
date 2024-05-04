#include "PerformanceView.h"

#include "components/PerfParamComponent.h"

PerformanceView::PerformanceView(PluginProcessor &p) : base_type(&p), processor_(p) {
}

PerformanceView::~PerformanceView() {
}

void PerformanceView::resized() {
    base_type::resized();
    auto r = getLocalBounds();
}


void PerformanceView::drawView(juce::Graphics &g) {
    base_type::drawView(g);
}

void PerformanceView::editorShown() {
    base_type::editorShown();
    removeAllChildren();
    params_.clear();
    page_ = 0;

    int x = canvasX();
    int y = canvasY();
    int w = canvasWidth();
    int h = canvasHeight();
    int ph = h / paramsPerPage;
    int py = y;

    int idx = 0;
    for (auto &param : processor_.performanceParams()) {
        int pidx = idx % paramsPerPage;
        auto c = std::make_shared<PerfParamComponent>(processor_, param);
        params_.push_back(c);
        c->setBounds(x, py + ph * pidx, w, ph);

        if (idx < paramsPerPage) {
            addAndMakeVisible(c.get());
        } else {
            addChildComponent(c.get());
        }
        idx++;
    }
}

void PerformanceView::eventButton(unsigned btn, bool longPress) {
    base_type::eventButton(btn, longPress);
}

void PerformanceView::eventUp(bool longPress) {
    if (!longPress) {
        if (page_ > 0) { changePage(-1); }
    }
    base_type::eventUp(longPress);
}

void PerformanceView::eventDown(bool longPress) {
    if (!longPress) {
        int maxPage = params_.size() / paramsPerPage;
        if (page_ < maxPage) { changePage(1); }
    }
    base_type::eventDown(longPress);
}

void PerformanceView::changePage(int delta) {
    for (int i = 0; i < paramsPerPage; i++) {
        int idx = page_ * paramsPerPage + i;
        if (idx < params_.size()) {
            auto c = params_[idx];
            c->setVisible(false);
        }
    }

    page_ += delta;

    for (int i = 0; i < paramsPerPage; i++) {
        int idx = page_ * paramsPerPage + i;
        if (idx < params_.size()) {
            auto c = params_[idx];
            c->setVisible(true);
        }
    }
}

void PerformanceView::onSSPTimer() {
    base_type::onSSPTimer();

    int sidx = page_ * paramsPerPage;
    int eidx = sidx + paramsPerPage;
    if (eidx > params_.size()) eidx = params_.size();
    for (int i = sidx; i < eidx; i++) {
        auto &c = params_[i];
        if (c->isVisible()) c->onSSPTimer();
    }
}

void PerformanceView::onEncoder(unsigned id, float v) {
    int sidx = page_ * paramsPerPage;
    int pidx = sidx + id;
    if (pidx < params_.size()) {
        auto &c = params_[pidx];
        c->onEncoder(id, v);
    }

    base_type::onEncoder(id, v);
}

void PerformanceView::onEncoderSwitch(unsigned id, bool v) {
    int sidx = page_ * paramsPerPage;
    int pidx = sidx + id;
    if (pidx < params_.size()) {
        auto &c = params_[pidx];
        c->onEncoderSwitch(id, v);
    }

    base_type::onEncoderSwitch(id, v);
}
