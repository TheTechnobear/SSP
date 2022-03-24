#pragma once

class SSPActions {
public:
    virtual void onEncoder(unsigned enc, float v) = 0;
    virtual void onEncoderSwitch(unsigned enc, bool v) = 0;
    virtual void onButton(unsigned btn, bool v) = 0;
    virtual void onLeftButton(bool v) = 0;
    virtual void onRightButton(bool v) = 0;
    virtual void onUpButton(bool v) = 0;
    virtual void onDownButton(bool v) = 0;
    virtual void onLeftShiftButton(bool v) = 0;
    virtual void onRightShiftButton(bool v) = 0;

    virtual void onSSPTimer() = 0;
};
