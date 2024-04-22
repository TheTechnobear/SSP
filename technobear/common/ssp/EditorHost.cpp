
#include "EditorHost.h"

#include "BaseProcessor.h"
#include "BaseView.h"
#include "SSP.h"


namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;

EditorHost::EditorHost(BaseProcessor *p, BaseView *e, bool compactUI, bool enableSysEditor, bool drawDefaults)
    : AudioProcessorEditor(p),
      processor_(p),
      editor_(e),
      system_(nullptr),
      compactUI_(compactUI),
      sysActive_(false),
      drawDefaults_(drawDefaults),
      globalBtn_("G", nullptr, 32, Colours::red),
      networkBtn_("N", nullptr, 32, Colours::red),
      plugInBtn_("P", nullptr, 32, Colours::red),
      recBtn_("R", nullptr, 32, Colours::red) {
    setWantsKeyboardFocus(true);
    addChildComponent(editor_);
    if (compactUI_) { drawDefaults_ = false; }


    if (enableSysEditor) {
        if (compactUI_) {
            system_ = new SystemMiniEditor(p);
            system_->setBounds(0, 0, SSP_COMPACT_WIDTH, SSP_COMPACT_HEIGHT);
        } else {
            system_ = new SystemFullEditor(p);
            system_->setBounds(0, 0, SSP_FULL_WIDTH, SSP_FULL_HEIGHT);
        }
        setVisible(sysActive_);
        addChildComponent(system_);
    }

    if (drawDefaults_) {
        addAndMakeVisible(globalBtn_);
        addAndMakeVisible(networkBtn_);
        addAndMakeVisible(plugInBtn_);
        addAndMakeVisible(recBtn_);

        setMenuBounds(globalBtn_, 0);
        setMenuBounds(networkBtn_, 1);
        setMenuBounds(plugInBtn_, 2);
        setMenuBounds(recBtn_, 3);
    }


    editor_->setVisible(!sysActive_);
    if (!compactUI_) {
        int uih = 0;
#ifdef __APPLE__
        uih = 220;
        sspui_ = new SSPUI(processor_, this);
        sspui_->setBounds(0, SSP_FULL_HEIGHT, SSP_FULL_WIDTH, uih);
        addChildComponent(sspui_);
        sspui_->setVisible(true);
#endif
        editor_->setBounds(0, 0, SSP_FULL_WIDTH, SSP_FULL_HEIGHT);
        setSize(SSP_FULL_WIDTH, SSP_FULL_HEIGHT + uih);
    } else {
        editor_->setBounds(0, 0, SSP_COMPACT_WIDTH, SSP_COMPACT_HEIGHT);
        if (system_) system_->setBounds(0, 0, SSP_COMPACT_WIDTH, SSP_COMPACT_HEIGHT);
        setSize(SSP_COMPACT_WIDTH, SSP_COMPACT_HEIGHT);
    }
}

EditorHost::~EditorHost() {
    delete editor_;
    if (system_) delete system_;
    delete sspui_;
}

void EditorHost::resized() {
    editor_->resized();
    if (system_) system_->resized();
    if (sspui_) sspui_->resized();
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


    if (drawDefaults_) {
        // title
        g.setFont(Font(Font::getDefaultMonospacedFontName(), 12 * COMPACT_UI_SCALE, Font::plain));
        g.setColour(Colours::yellow);
        g.drawSingleLineText(String(JucePlugin_Name) + " : " + String(JucePlugin_Desc) + String(" @ thetechnobear"), 10,
                             30);

        g.setColour(Colours::grey);
        g.drawSingleLineText("v " + String(JucePlugin_VersionString), 1505, 30);

        drawMenuBox(g);
        drawButtonBox(g);
    }
}


void EditorHost::drawMenuBox(Graphics &g) {
    unsigned y = menuTopY - 1;
    unsigned x = 1530 - 1;
    unsigned butTopY = btnTopY;
    g.setColour(Colours::grey);
    g.drawVerticalLine(x, y, butTopY);
    g.drawVerticalLine(1600 - 1, y, butTopY);
    for (int i = 0; i < 5; i++) { g.drawHorizontalLine(y + i * 45, x, 1600 - 1); }
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
    for (int i = 0; i < 8; i++) { g.drawVerticalLine(x + i * 100, butTopY, 480 - 1); }
}

void EditorHost::sysEditor() {
    if (!system_) return;

    sysActive_ = !sysActive_;
    editor_->setVisible(!sysActive_);
    system_->setVisible(sysActive_);
}


void EditorHost::onEncoder(unsigned enc, float v) {
    if (sysActive_)
        system_->onEncoder(enc, v);
    else
        editor_->onEncoder(enc, v);
}

void EditorHost::onEncoderSwitch(unsigned enc, bool v) {
    if (sysActive_)
        system_->onEncoderSwitch(enc, v);
    else
        editor_->onEncoderSwitch(enc, v);
}

void EditorHost::onButton(unsigned btn, bool v) {
    if (sysActive_)
        system_->onButton(btn, v);
    else
        editor_->onButton(btn, v);
}

void EditorHost::onLeftButton(bool v) {
    if (sysActive_)
        system_->onLeftButton(v);
    else
        editor_->onLeftButton(v);
}

void EditorHost::onRightButton(bool v) {
    if (sysActive_)
        system_->onRightButton(v);
    else
        editor_->onRightButton(v);
}

void EditorHost::onUpButton(bool v) {
    if (sysActive_)
        system_->onUpButton(v);
    else
        editor_->onUpButton(v);
}

void EditorHost::onDownButton(bool v) {
    if (sysActive_)
        system_->onDownButton(v);
    else
        editor_->onDownButton(v);
}

void EditorHost::onLeftShiftButton(bool v) {
    if (sysActive_)
        system_->onLeftShiftButton(v);
    else
        editor_->onLeftShiftButton(v);

    LSActive_ = v;
    if (LSActive_ && RSActive_ && system_) sysEditor();
}

void EditorHost::onRightShiftButton(bool v) {
    if (sysActive_)
        system_->onRightShiftButton(v);
    else
        editor_->onRightShiftButton(v);

    RSActive_ = v;
    if (LSActive_ && RSActive_ && system_) sysEditor();
}

void EditorHost::onSSPTimer() {
    editor_->onSSPTimer();
}


void EditorHost::eventButton(unsigned btn, bool longPress) {
    if (sysActive_)
        system_->eventButton(btn, longPress);
    else
        editor_->eventButton(btn, longPress);
}

void EditorHost::eventUp(bool longPress) {
    if (sysActive_)
        system_->eventUp(longPress);
    else
        editor_->eventUp(longPress);
}

void EditorHost::eventDown(bool longPress) {
    if (sysActive_)
        system_->eventDown(longPress);
    else
        editor_->eventDown(longPress);
}

void EditorHost::eventLeft(bool longPress) {
    if (sysActive_)
        system_->eventLeft(longPress);
    else
        editor_->eventLeft(longPress);
}

void EditorHost::eventRight(bool longPress) {
    if (sysActive_)
        system_->eventRight(longPress);
    else
        editor_->eventRight(longPress);
}

void EditorHost::eventLeftShift(bool longPress) {
    if (sysActive_)
        system_->eventLeftShift(longPress);
    else
        editor_->eventLeftShift(longPress);
}

void EditorHost::eventRightShift(bool longPress) {
    if (sysActive_)
        system_->eventRightShift(longPress);
    else
        editor_->eventRightShift(longPress);
}

void EditorHost::eventButtonCombo(unsigned btn, unsigned comboBtn, bool longPress) {
    if (sysActive_)
        system_->eventButtonCombo(btn, comboBtn, longPress);
    else
        editor_->eventButtonCombo(btn, comboBtn, longPress);

    if(compactUI_) {
        if ((comboBtn == SSP_Up && btn == SSP_Down) || (comboBtn == SSP_Down && btn == SSP_Up)) { sysEditor(); }
    }
}


// up, down, left , right , rs, ls
bool EditorHost::keyStateChanged(bool isKeyDown) {
    int ks = 0;
    bool ls = false;
    bool ns = false;
    bool ret = false;
    ls = keyStates_[ks];
    ns = juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::upKey);
    if (ls != ns) onUpButton(ns);
    ret |= ls != ns;
    keyStates_[ks] = ns;
    ks++;
    ls = keyStates_[ks];
    ns = juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::downKey);
    if (ls != ns) onDownButton(ns);
    ret |= ls != ns;
    keyStates_[ks] = ns;
    ks++;
    ls = keyStates_[ks];
    ns = juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::leftKey);
    if (ls != ns) onLeftButton(ns);
    ret |= ls != ns;
    keyStates_[ks] = ns;
    ks++;
    ls = keyStates_[ks];
    ns = juce::KeyPress::isKeyCurrentlyDown(juce::KeyPress::rightKey);
    if (ls != ns) onRightButton(ns);
    ret |= ls != ns;
    keyStates_[ks] = ns;
    ks++;

    return ret;
}

void EditorHost::modifierKeysChanged(const ModifierKeys &modifiers) {
    int ks = 3;
    bool ls = false;
    bool ns = false;
    bool ret = false;
    ls = keyStates_[ks];
    ns = modifiers.isShiftDown();
    if (ls != ns) onLeftShiftButton(ns);
    ret |= ls != ns;
    keyStates_[ks] = ns;
    ks++;
    ls = keyStates_[ks];
    ns = modifiers.isCtrlDown();
    if (ls != ns) onRightShiftButton(ns);
    ret |= ls != ns;
    keyStates_[ks] = ns;
}


}  // namespace ssp
