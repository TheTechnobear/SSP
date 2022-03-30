
#include "EditorHost.h"


#include "BaseEditor.h"
#include "BaseProcessor.h"
#include "SSP.h"


namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;

EditorHost::EditorHost(BaseProcessor *p, BaseEditor *e) :
    AudioProcessorEditor(p),
    processor_(p),
    editor_(e),
    globalBtn_("G", nullptr, 32, Colours::red),
    networkBtn_("N", nullptr, 32, Colours::red),
    plugInBtn_("P", nullptr, 32, Colours::red),
    recBtn_("R", nullptr, 32, Colours::red) {

    sysActive_ = false;
    system_ = new SystemEditor(p);

    addChildComponent(editor_);
    addChildComponent(system_);
    editor_->setVisible(!sysActive_);
    system_->setVisible(sysActive_);

    int uih = 0;
#ifdef __APPLE__
    uih = 220;
    sspui_ = new SSPUI(processor_, this);
    sspui_->setBounds(0, 480, 1600, 220);
    addChildComponent(sspui_);
    sspui_->setVisible(true);
#endif

    setSize(1600, 480 + uih);


    addAndMakeVisible(globalBtn_);
    addAndMakeVisible(networkBtn_);
    addAndMakeVisible(plugInBtn_);
    addAndMakeVisible(recBtn_);

    setMenuBounds(globalBtn_, 0);
    setMenuBounds(networkBtn_, 1);
    setMenuBounds(plugInBtn_, 2);
    setMenuBounds(recBtn_, 3);

    editor_->setBounds(0, 0, 1600, 480);
    system_->setBounds(0, 0, 1600, 480);
}

EditorHost::~EditorHost() {
    delete editor_;
    delete system_;
    delete sspui_;
}


void EditorHost::setMenuBounds(ValueButton &btn, unsigned r) {
    const int w = 70;
    const int h = 45;
    unsigned x = 1530 + 1;
    unsigned y = menuTopY + (r * h);
    btn.setBounds(x, y, w, h);
}

void EditorHost::drawBasicPanel(Graphics &g) {
    g.fillAll(Colour(0xff111111));


    // title
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
    g.setColour(Colours::yellow);
    g.drawSingleLineText(String(JucePlugin_Name) + " : "
                         + String(JucePlugin_Desc)
                         + String(" @ thetechnobear"),
                         10, 30);

//    g.drawSingleLineText("thetechnobear's "
//                         + String(JucePlugin_Name) + " : "
//                         + String(JucePlugin_Desc),
//                         10, 30);

    g.setColour(Colours::grey);
    g.drawSingleLineText("v " + String(JucePlugin_VersionString), 1505, 30);

    drawMenuBox(g);
    drawButtonBox(g);
}


void EditorHost::drawMenuBox(Graphics &g) {
    unsigned y = menuTopY - 1;
    unsigned x = 1530 - 1;
    unsigned butTopY = btnTopY;
    g.setColour(Colours::grey);
    g.drawVerticalLine(x, y, butTopY);
    g.drawVerticalLine(1600 - 1, y, butTopY);
    for (int i = 0; i < 5; i++) {
        g.drawHorizontalLine(y + i * 45, x, 1600 - 1);
    }
}

void EditorHost::drawButtonBox(Graphics &g) {
    unsigned butTopY = btnTopY;
    unsigned butLeftX = 900 - 1;
    float x = butLeftX;
    float y = butTopY;
    g.setColour(Colours::grey);
    g.drawHorizontalLine(y, x, 1600 - 1);
    g.drawHorizontalLine(y + btnSpaceY, x, 1600 - 1);
    g.drawHorizontalLine(480 - 1, x, 1600 - 1);
    for (int i = 0; i < 8; i++) {
        g.drawVerticalLine(x + i * 100, butTopY, 480 - 1);
    }
}

void EditorHost::sysEditor() {
    sysActive_ = !sysActive_;
    editor_->setVisible(!sysActive_);
    system_->setVisible(sysActive_);
}


void EditorHost::onEncoder(unsigned enc, float v) {
    if (sysActive_) system_->onEncoder(enc, v);
    else editor_->onEncoder(enc, v);
}

void EditorHost::onEncoderSwitch(unsigned enc, bool v) {
    if (sysActive_) system_->onEncoderSwitch(enc, v);
    else editor_->onEncoderSwitch(enc, v);
}

void EditorHost::onButton(unsigned btn, bool v) {
    if (sysActive_) system_->onButton(btn, v);
    else editor_->onButton(btn, v);
}

void EditorHost::onLeftButton(bool v) {
    if (sysActive_) system_->onLeftButton(v);
    else editor_->onLeftButton(v);
}

void EditorHost::onRightButton(bool v) {
    if (sysActive_) system_->onRightButton(v);
    else editor_->onRightButton(v);
}

void EditorHost::onUpButton(bool v) {
    if (sysActive_) system_->onUpButton(v);
    else editor_->onUpButton(v);
}

void EditorHost::onDownButton(bool v) {
    if (sysActive_) system_->onDownButton(v);
    else editor_->onDownButton(v);
}

void EditorHost::onLeftShiftButton(bool v) {
    if (sysActive_) system_->onLeftShiftButton(v);
    else editor_->onLeftShiftButton(v);

    LSActive_ = v;
    if (RSActive_ && LSActive_) sysEditor();
}

void EditorHost::onRightShiftButton(bool v) {
    if (sysActive_) system_->onRightShiftButton(v);
    else editor_->onRightShiftButton(v);

    RSActive_ = v;
    if (LSActive_ && RSActive_) sysEditor();
}

void EditorHost::onSSPTimer() {
    editor_->onSSPTimer();
}


}
