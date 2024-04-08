#include "TableParamEditor.h"

#include "ssp/controls/ParamControl.h"

namespace ssp {

static constexpr unsigned paramSpaceY = 36;


TableParamEditor::TableParamEditor(BaseProcessor *p)
    : base_type(p) {
    ;
}


void TableParamEditor::drawView(Graphics &g) {
    base_type::drawView(g);
    int h = paramSpaceY;
    int w = int(900.0f / 4.0f);

    int x = 0;
    int y = (paramPage_ + 1) * (h * 2);
    g.setColour(Colours::darkgrey);
    g.fillRect(x, y, w * 4, h);

    static juce::Colour clrs[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
    g.setColour(clrs[paramPage_ % 4]);
    g.fillRect(x, y, w * 4, h);
}


void TableParamEditor::setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

    unsigned h = 2 * paramSpaceY;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = idx * w;
    unsigned y = (page * h) + (h);

    unsigned sp = 2;
    c->setBounds(x + sp, y, w - sp * 2, h);
}


BaseEditor::ControlPage TableParamEditor::addParamPage(
    std::shared_ptr<BaseParamControl> c1,
    std::shared_ptr<BaseParamControl> c2,
    std::shared_ptr<BaseParamControl> c3,
    std::shared_ptr<BaseParamControl> c4
) {
    auto page = base_type::addParamPage(c1, c2, c3, c4);
    unsigned pageN = controlPages_.size() - 1;

    setParamBounds(pageN, 0, c1);
    setParamBounds(pageN, 1, c2);
    setParamBounds(pageN, 2, c3);
    setParamBounds(pageN, 3, c4);

    static juce::Colour clrs[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
    for (auto i = 0; i < 4; i++) {
        auto c = page.control_[i];
        if (c) {
            c->fg(clrs[pageN % 4]);
        }
    }


//    if (c1) c1->fg(Colours::red);
//    if (c2) c2->fg(Colours::blue);
//    if (c3) c3->fg(Colours::green);
//    if (c4) c4->fg(Colours::yellow);

    for (auto i = 0; i < 4; i++) {
        auto c = page.control_[i];
        if (c) {
            addAndMakeVisible(c.get());
            c->active(paramPage_ == pageN);
        }
    }

    return page;
}


void TableParamEditor::onUpButton(bool v) {
    upBtn_.onButton(v);
}

void TableParamEditor::onDownButton(bool v) {
    downBtn_.onButton(v);
}



void TableParamEditor::eventUp(bool v) {
    if (v) return; // change on button up
    chgParamPage(-11,false);
}

void TableParamEditor::eventDown(bool v) {
    if (v) return; // change on button up
    chgParamPage(1,false);
}

} // namespace
