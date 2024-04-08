#include "BarParamEditor.h"

#include "ssp/controls/ParamControl.h"

namespace ssp {

static constexpr unsigned paramSpaceY = 34;

BarParamEditor::BarParamEditor(BaseProcessor *p,bool autoColour)
    : base_type(p), autoColour_(autoColour) {
    ;
}


void BarParamEditor::drawView(Graphics &g) {
    base_type::drawView(g);

    if(autoColour_) {
        static juce::Colour clrs[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
        g.setColour(clrs[paramPage_ % 4]);

        int x = 0;
        int w = 900;
        int h = paramSpaceY / 4;
        int y = 480 - h;
        int sp = 2;
        g.fillRect(x + sp, y, w - (2 * sp), h);
    }
}


void BarParamEditor::setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

    int w = int(900.0f / 4.0f);
    int x = int(idx) * w;
    int h = 2 * paramSpaceY;
    int y = (int(page) * h) + h;

    int sp = 2;
    c->setBounds(x + sp, y, w - sp * 2, h);
}


BaseEditor::ControlPage BarParamEditor::addParamPage(
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

    if(autoColour_) {
        static juce::Colour clrs[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
        for (const auto& c : page.control_) {
            if (c) {
                c->fg(clrs[pageN % 4]);
            }
        }
    }

    for (const auto& c : page.control_) {
        if (c) {
            addAndMakeVisible(c.get());
            c->active(paramPage_ == pageN);
        }
    }

    return page;
}


void BarParamEditor::onUpButton(bool v) {
    upBtn_.onButton(v);

    if (v) return; // change on button up

    chgParamPage(-1, false);
}

void BarParamEditor::onDownButton(bool v) {
    downBtn_.onButton(v);

    if (v) return; // change on button up
    chgParamPage(1, false);
}

} // namespace
