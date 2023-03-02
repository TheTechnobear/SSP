#include "../../ssp-sdk/Percussa.h"
#include "ssp/EditorHost.h"
#include "SSPApiEditor.h"

//SSPHASH
#define SSP_IMAGECACHE_HASHCODE 0x53535048415348

SSP_PluginEditorInterface::SSP_PluginEditorInterface(ssp::EditorHost *editor) :
    editor_(editor) {
}

SSP_PluginEditorInterface::~SSP_PluginEditorInterface() {
    if (editor_) delete editor_;
}

void SSP_PluginEditorInterface::frameStart() {
    PluginEditorInterface::frameStart();
    if (editor_) editor_->onSSPTimer();
}

void SSP_PluginEditorInterface::visibilityChanged(bool b) {
    PluginEditorInterface::visibilityChanged(b);
}

void SSP_PluginEditorInterface::renderToImage(unsigned char *buffer, int width, int height) {
    Image img = ImageCache::getFromHashCode(SSP_IMAGECACHE_HASHCODE);
    if (!img.isValid()) {
        // std::cerr << "new render image created" << std::endl;
        Image newimg(Image::ARGB, width, height, true);
        ImageCache::addImageToCache(newimg, SSP_IMAGECACHE_HASHCODE);
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
            case SSP_Left : {
                editor_->onLeftButton(val);
                break;
            }
            case SSP_Right : {
                editor_->onRightButton(val);
                break;
            }
            case SSP_Up : {
                editor_->onUpButton(val);
                break;
            }
            case SSP_Down : {
                editor_->onDownButton(val);
                break;
            }
            case SSP_Shift_L : {
                editor_->onLeftShiftButton(val);
                break;
            }
            case SSP_Shift_R : {
                editor_->onRightShiftButton(val);
                break;
            }
            default : {
                // ignore
            }
        }
    }
}

void SSP_PluginEditorInterface::encoderPressed(int n, bool val) {
    editor_->onEncoderSwitch(n, val);
}

void SSP_PluginEditorInterface::encoderTurned(int n, int val) {
    editor_->onEncoder(n, (float) val);
}
