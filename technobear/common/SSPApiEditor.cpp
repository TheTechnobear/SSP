#include "SSPApiEditor.h"

#include "../../ssp-sdk/Percussa.h"
#include "ssp/EditorHost.h"

// SSPHASH
#define SSP_FULL_IMAGECACHE_HASHCODE 0x53535048415348
#define SSP_COMPACT_IMAGECACHE_HASHCODE 0x53535048415349

SSP_PluginEditorInterface::SSP_PluginEditorInterface(ssp::EditorHost *editor) : editor_(editor) {
    for (int i = 0; i < SSP_LastBtn; i++) {
        buttonCounter_[i] = 0;
        buttonState_[i] = false;
    }
}

SSP_PluginEditorInterface::~SSP_PluginEditorInterface() {
    if (editor_) delete editor_;
}

void SSP_PluginEditorInterface::frameStart() {
    PluginEditorInterface::frameStart();
    if (editor_) editor_->onSSPTimer();
    for (int i = 0; i < SSP_LastBtn; i++) { buttonCounter_[i] -= (buttonCounter_[i] > 0); }
}

void SSP_PluginEditorInterface::visibilityChanged(bool b) {
    PluginEditorInterface::visibilityChanged(b);
}

void SSP_PluginEditorInterface::renderToImage(unsigned char *buffer, int width, int height) {
    auto hashcode = editor_->isCompactUI() ? SSP_COMPACT_IMAGECACHE_HASHCODE : SSP_FULL_IMAGECACHE_HASHCODE;
    Image img = ImageCache::getFromHashCode(hashcode);
    if (!img.isValid()) {
        // std::cerr << "new render image created" << std::endl;
        Image newimg(Image::ARGB, width, height, true);
        ImageCache::addImageToCache(newimg, hashcode);
        img = newimg;
    }

    if (!editor_->isVisible()) {
        editor_->setBounds(Rectangle<int>(0, 0, width, height));
        editor_->setOpaque(true);
        editor_->setVisible(true);
    }

    Graphics g(img);
    editor_->paintEntireComponent(g, true);
    Image::BitmapData bitmap(img, Image::BitmapData::readOnly);

    memcpy(buffer, bitmap.data, width * height * 4);
}

void SSP_PluginEditorInterface::buttonPressed(int n, bool val) {
    //        std::cerr << "buttonPressed " << n <<  " : " << val << std::endl;
    if (n <= SSP_Soft_8) {
        editor_->onButton(n, val);
    } else {
        switch (n) {
            case SSP_Left: {
                editor_->onLeftButton(val);
                break;
            }
            case SSP_Right: {
                editor_->onRightButton(val);
                break;
            }
            case SSP_Up: {
                editor_->onUpButton(val);
                break;
            }
            case SSP_Down: {
                editor_->onDownButton(val);
                break;
            }
            case SSP_Shift_L: {
                editor_->onLeftShiftButton(val);
                break;
            }
            case SSP_Shift_R: {
                editor_->onRightShiftButton(val);
                break;
            }
            default: {
                // ignore
            }
        }
    }
    generateButtenEvents(n, val);
}

void SSP_PluginEditorInterface::encoderPressed(int n, bool val) {
    editor_->onEncoderSwitch(n, val);
}

void SSP_PluginEditorInterface::encoderTurned(int n, int val) {
    editor_->onEncoder(n, (float)val);
}

void SSP_PluginEditorInterface::generateButtenEvents(int n, bool val) {
    static constexpr unsigned LONG_PRESS_COUNT = 5;

    if (buttonState_[n] == val) return;
    // only look at transitions

    auto &actions_ = editor_;
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
