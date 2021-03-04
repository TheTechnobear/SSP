#include "BarParamEditor.h"

#include "BaseParamControl.h"

namespace ssp {

//static constexpr unsigned paramSpaceY = 36;
static constexpr unsigned paramSpaceY = 34;


BarParamEditor::BarParamEditor(BaseProcessor *p, String title, String version)
    : base_type(p, title, version) {
    ;
}


void BarParamEditor::paint(Graphics &g) {
    base_type::paint(g);
    int h = paramSpaceY;
    int w = int(900.0f / 4.0f);

    int x = 0;
    int y = (paramPage_ + 1) * (h * 2);
//    g.setColour(Colours::darkgrey);
//    g.fillRect(x, y, w * 4, h);

    static juce::Colour clrs[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
    g.setColour(clrs[paramPage_ % 4]);

    h = paramSpaceY / 4;
    y = 480 - h;
    g.fillRect(x, y, w * 4, h);

//    // draw bands
//    h = paramSpaceY /4 ;
//    y = 480 - h;
//    g.setColour(Colours::red);
//    g.fillRect(x, y, w, h);
//    x += w;
//    g.setColour(Colours::blue);
//    g.fillRect(x, y, w, h);
//    x += w;
//    g.setColour(Colours::green);
//    g.fillRect(x, y, w, h);
//    x += w;
//    g.setColour(Colours::yellow);
//    g.fillRect(x, y, w, h);
}


void BarParamEditor::setParamBounds(unsigned page, unsigned idx, std::shared_ptr<BaseParamControl> c) {
    if (c == nullptr) return;

//    unsigned w = unsigned(900.0f / 4.0f);
//    unsigned x = idx * w;
    unsigned sp = 10.0f;
    unsigned w = unsigned(900.0f / 4.0f);
    unsigned x = (idx * w) + (sp / 2);
    unsigned h = 2 * paramSpaceY;
    unsigned y = (page * h) + (h);
    c->setBounds(x, y, w - sp, h);
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


    static juce::Colour clrs[4] = {Colours::red, Colours::blue, Colours::yellow, Colours::green};
    for (auto i = 0; i < 4; i++) {
        auto c = page.control_[i];
        if (c) {
            c->fg(clrs[pageN % 4]);
        }
    }
//    if(c1) c1->fg(Colours::red);
//    if(c2) c2->fg(Colours::blue);
//    if(c3) c3->fg(Colours::green);
//    if(c4) c4->fg(Colours::yellow);

    for (auto i = 0; i < 4; i++) {
        auto c = page.control_[i];
        if (c) {
            addAndMakeVisible(c.get());
            c->active(paramPage_ == pageN);
        }
    }

    return page;
}


void BarParamEditor::onUpButton(bool v) {
    base_type::onUpButton(v);

    if (v) return; // change on button up

    chgParamPage(-1, false);
}

void BarParamEditor::onDownButton(bool v) {
    base_type::onDownButton(v);

    if (v) return; // change on button up
    chgParamPage(1, false);
}

} // namespace
