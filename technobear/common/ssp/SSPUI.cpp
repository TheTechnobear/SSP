
#include "SSPUI.h"

#include "EditorHost.h"

namespace ssp {

static constexpr unsigned LONG_PRESS_COUNT = 30;

SSPUI::SSPUI(BaseProcessor *processor, EditorHost *actions) : processor_(processor), actions_(actions) {
    startTimer(50);

    for (unsigned i = 0; i < NENC; i++) {
        auto &up = encUp_[i];
        auto &down = encDown_[i];
        auto &sw = encSW_[i];

        static constexpr int x = 60, y = 10;
        static constexpr int bw = 55, bh = 30;

        down.setButtonText("-");
        down.setBounds(((i * 4) * bw) + (0 * bw) + x, y, bw, bh);
        down.addListener(this);
        addAndMakeVisible(down);

        up.setButtonText("+");
        up.setBounds(((i * 4) * bw) + (1 * bw) + x, y, bw, bh);
        up.addListener(this);
        addAndMakeVisible(up);

        sw.setBounds(((i * 4) * bw) + (2 * bw) + x, y, bw, bh);
        sw.addListener(this);
        addAndMakeVisible(sw);
    }

    for (unsigned i = 0; i < NBUTS; i++) {
        auto &b = buttons_[i];
        static constexpr int x = 940, y = 10;
        static constexpr int bw = 95, bh = 30;

        int x1 = (i % 4);
        int y1 = i < 4 ? 0 : 1;

        b.setBounds((x1 * bw) + x, y + (y1 * bh), bw, bh);
        b.addListener(this);
        addAndMakeVisible(b);
    }

    {
        int i = 0;
        static constexpr int x = 1350, y = 10;
        static constexpr int bw = 75, bh = 30;
        ls_.setButtonText("LS");
        ls_.setBounds(i * bw + x, y, bw, bh);
        ls_.addListener(this);
        addAndMakeVisible(ls_);

        i++;
        up_.setButtonText("UP");
        up_.setBounds(i * bw + x, y, bw, bh);
        up_.addListener(this);
        addAndMakeVisible(up_);

        i++;
        rs_.setButtonText("RS");
        rs_.setBounds(i * bw + x, y, bw, bh);
        rs_.addListener(this);
        addAndMakeVisible(rs_);


        i = 0;
        left_.setButtonText("LEFT");
        left_.setBounds(i * bw + x, y + bh, bw, bh);
        left_.addListener(this);
        addAndMakeVisible(left_);

        i++;
        down_.setButtonText("DOWN");
        down_.setBounds(i * bw + x, y + bh, bw, bh);
        down_.addListener(this);
        addAndMakeVisible(down_);

        i++;
        right_.setButtonText("RIGHT");
        right_.setBounds(i * bw + x, y + bh, bw, bh);
        right_.addListener(this);
        addAndMakeVisible(right_);
    }

    for (unsigned i = 0; i < NIO; i++) {
        auto &bI = inputs_[i];
        auto &bO = outputs_[i];

        static constexpr int x = 100, y = 150;
        static constexpr int bw = 30, bh = 30;
        bI.setBounds(i * bw + x, y, bw, bh);
        bO.setBounds(i * bw + x, y + bh, bw, bh);

        bI.setToggleState(processor_->isInputEnabled(i), juce::dontSendNotification);
        bO.setToggleState(processor_->isOutputEnabled(i), juce::dontSendNotification);
        bI.addListener(this);
        bO.addListener(this);
        addAndMakeVisible(bI);
        addAndMakeVisible(bO);
    }

    for (int i = 0; i < SSP_LastBtn; i++) {
        buttonCounter_[i] = 0;
        buttonState_[i] = false;
    }
}

SSPUI::~SSPUI() {
    stopTimer();
}


void SSPUI::timerCallback() {
    actions_->onSSPTimer();
    for (int i = 0; i < SSP_LastBtn; i++) { buttonCounter_[i] -= (buttonCounter_[i] > 0); }
    actions_->repaint();
}

void SSPUI::paint(juce::Graphics &g) {
    //    g.fillAll(Colours::grey);
    g.setColour(juce::Colours::white);
    g.drawText("Inputs :", 30, 150, 65, 30, juce::Justification::left);
    g.drawText("Outputs :", 30, 180, 65, 30, juce::Justification::left);
}

void SSPUI::resized() {
}

void SSPUI::buttonStateChanged(juce::Button *button) {
    Listener::buttonStateChanged(button);
}

void SSPUI::buttonClicked(juce::Button *button) {
    for (unsigned i = 0; i < NENC; i++) {
        if (button == &encUp_[i]) {
            actions_->onEncoder(i, 1.0f);
            return;
        }
        if (button == &encDown_[i]) {
            actions_->onEncoder(i, -1.0f);
            return;
        }
        if (button == &encSW_[i]) {
            actions_->onEncoderSwitch(i, button->getToggleState());
            return;
        }
    }
    for (unsigned i = 0; i < NBUTS; i++) {
        if (button == &buttons_[i]) {
            if (button == &buttons_[i]) {
                buttonPressed(i, button->getToggleState());
                // actions_->onButton(i, button->getToggleState());
                return;
            }
            return;
        }
    }
    if (button == &ls_) {
        buttonPressed(SSP_Shift_L, button->getToggleState());
        // actions_->onLeftShiftButton(button->getToggleState());
        return;
    }
    if (button == &rs_) {
        buttonPressed(SSP_Shift_R, button->getToggleState());
        // actions_->onRightShiftButton(button->getToggleState());
        return;
    }
    if (button == &up_) {
        buttonPressed(SSP_Up, button->getToggleState());
        // actions_->onUpButton(button->getToggleState());
        return;
    }
    if (button == &down_) {
        buttonPressed(SSP_Down, button->getToggleState());
        // actions_->onDownButton(button->getToggleState());
        return;
    }
    if (button == &left_) {
        buttonPressed(SSP_Left, button->getToggleState());
        // actions_->onLeftButton(button->getToggleState());
        return;
    }
    if (button == &right_) {
        buttonPressed(SSP_Right, button->getToggleState());
        // actions_->onRightButton(button->getToggleState());
        return;
    }

    for (unsigned i = 0; i < NIO; i++) {
        if (button == &inputs_[i]) {
            processor_->onInputChanged(i, button->getToggleState());
            return;
        }
        if (button == &outputs_[i]) {
            processor_->onOutputChanged(i, button->getToggleState());
            return;
        }
    }
}


void SSPUI::buttonPressed(int n, bool val) {
    if (n <= SSP_Soft_8) {
        actions_->onButton(n, val);
    } else {
        switch (n) {
            case SSP_Left: {
                actions_->onLeftButton(val);
                break;
            }
            case SSP_Right: {
                actions_->onRightButton(val);
                break;
            }
            case SSP_Up: {
                actions_->onUpButton(val);
                break;
            }
            case SSP_Down: {
                actions_->onDownButton(val);
                break;
            }
            case SSP_Shift_L: {
                actions_->onLeftShiftButton(val);
                break;
            }
            case SSP_Shift_R: {
                actions_->onRightShiftButton(val);
                break;
            }
            default: {
                // ignore
            }
        }
    }

    generateButtenEvents(n, val);
}

void SSPUI::generateButtenEvents(int n, bool val) {
    if (buttonState_[n] == val) return;
    // only look at transitions

    buttonState_[n] = val;

    if (val) {
        // button pressed
        buttonCounter_[n] = LONG_PRESS_COUNT;
        // no other action on press..
        return;
    }

    // on release...
    bool longPress = buttonCounter_[n] == 0;

    for (int i = 0; i < SSP_LastBtn; i++) {
        if (i == n) continue;
        if (buttonState_[i])  {
            // consume combo
            buttonCounter_[i] = 0;
            buttonState_[i] = false;
            actions_->eventButtonCombo(n, i, longPress);
            return;
        }
    }

    switch (n) {
        case SSP_Soft_1:
        case SSP_Soft_2:
        case SSP_Soft_3:
        case SSP_Soft_4:
        case SSP_Soft_5:
        case SSP_Soft_6:
        case SSP_Soft_7:
        case SSP_Soft_8: {
            actions_->eventButton(n, longPress);
            break;
        }
        case SSP_Left: {
            actions_->eventLeft(longPress);
            break;
        }
        case SSP_Right: {
            actions_->eventRight(longPress);
            break;
        }
        case SSP_Up: {
            actions_->eventUp(longPress);
            break;
        }
        case SSP_Down: {
            actions_->eventDown(longPress);
            break;
        }
        case SSP_Shift_L: {
            actions_->eventLeftShift(longPress);
            break;
        }
        case SSP_Shift_R: {
            actions_->eventRightShift(longPress);
            break;
        }
    }
}

}  // namespace ssp
