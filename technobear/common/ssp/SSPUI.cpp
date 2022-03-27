
#include "SSPUI.h"

namespace ssp {

SSPUI::SSPUI(BaseProcessor *processor, SSPActions *actions)
    : processor_(processor), actions_(actions) {

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

        bI.setToggleState(processor_->isInputEnabled(i), dontSendNotification);
        bO.setToggleState(processor_->isOutputEnabled(i), dontSendNotification);
        bI.addListener(this);
        bO.addListener(this);
        addAndMakeVisible(bI);
        addAndMakeVisible(bO);
    }
}


void SSPUI::paint(juce::Graphics &g) {
//    g.fillAll(Colours::grey);
    g.setColour(Colours::white);
    g.drawText("Inputs :", 30, 150, 65, 30, Justification::left);
    g.drawText("Outputs :", 30, 180, 65, 30, Justification::left);

}

void SSPUI::resized() {
}

void SSPUI::buttonStateChanged(Button *button) {
    Listener::buttonStateChanged(button);
}

void SSPUI::buttonClicked(Button *button) {
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
                actions_->onButton(i, button->getToggleState());
                return;
            }
            return;
        }
    }
    if (button == &ls_) {
        actions_->onLeftShiftButton(button->getToggleState());
        return;
    }
    if (button == &rs_) {
        actions_->onRightShiftButton(button->getToggleState());
        return;
    }
    if (button == &up_) {
        actions_->onUpButton(button->getToggleState());
        return;
    }
    if (button == &down_) {
        actions_->onDownButton(button->getToggleState());
        return;
    }
    if (button == &left_) {
        actions_->onLeftButton(button->getToggleState());
        return;
    }
    if (button == &right_) {
        actions_->onRightButton(button->getToggleState());
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


} // namespace
