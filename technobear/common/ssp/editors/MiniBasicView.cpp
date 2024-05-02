
#include "BaseMiniView.h"
#include "ssp/BaseProcessor.h"

namespace ssp {

MiniBasicView::MiniBasicView(BaseProcessor *p, ioActivity cb) : base_type(p, true), ioCallback_(cb) {
    buttonBox_ = std::make_shared<ssp::ButtonBox>();
    addAndMakeVisible(buttonBox_.get());
}

void MiniBasicView::addButton(unsigned idx, const std::shared_ptr<ValueButton> &p) {
    buttonBox_->addButton(idx, p);
}

void MiniBasicView::onButton(unsigned int id, bool v) {
    base_type::onButton(id, v);
    buttonBox_->onButton(id, v);
}

std::shared_ptr<ValueButton> MiniBasicView::getButton(unsigned idx) {
    return buttonBox_->getButton(idx);
}


void MiniBasicView::resized() {
    base_type::resized();

    buttonBox_->setBounds(gap, SSP_COMPACT_HEIGHT - buttonBarH, canvasW, buttonBarH);
}

}  // namespace ssp