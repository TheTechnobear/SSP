#pragma once

namespace ssp {

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


    
    virtual void eventButton(unsigned btn,bool longPress) = 0;
    virtual void eventUp(bool longPress) = 0;
    virtual void eventDown(bool longPress) = 0;
    virtual void eventLeft(bool longPress) = 0;
    virtual void eventRight(bool longPress) = 0;
    virtual void eventLeftShift(bool longPress) = 0;
    virtual void eventRightShift(bool longPress) = 0;
    virtual void eventButtonCombo(unsigned btn,unsigned comboBtn, bool longPress) = 0;
};


}
