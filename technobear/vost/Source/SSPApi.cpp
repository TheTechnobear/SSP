
#include "Percussa.h"

#include "PluginProcessor.h"
#include "ssp/EditorHost.h"

// this is a common file in ALL my plugin projects
// it is copied between them, so do not alter it without propagating to others.

// there are two reasons for it to be duplicated
// - it has to reference the specific PluginProcessor
// - there are static functions , and we may want different variations

// note: if a plugin needs a variation , the COPY the file and rename it
// and ref new file name in cmake, this way f I propagate a changed version it will NOT
// overwrite the custom version !
// this is HIGHLY likely to occur !!!!

enum SSPButtons {
    SSP_Soft_1,
    SSP_Soft_2,
    SSP_Soft_3,
    SSP_Soft_4,
    SSP_Soft_5,
    SSP_Soft_6,
    SSP_Soft_7,
    SSP_Soft_8,
    SSP_Left,
    SSP_Right,
    SSP_Up,
    SSP_Down,
    SSP_Shift_L,
    SSP_Shift_R,
    SSP_LastBtn
};


class SSP_PluginEditorInterface : public Percussa::SSP::PluginEditorInterface {
public:
    SSP_PluginEditorInterface(ssp::EditorHost *editor) :
        editor_(editor) {
    }

    SSP_PluginEditorInterface() {
        if (img_) delete img_;
//    if (editor_) delete editor_;
    }

    void renderToImage(unsigned char *buffer, int width, int height) override {
        if (img_ == nullptr) {
            img_ = new Image(Image::ARGB, width, height, false);
            editor_->setBounds(Rectangle<int>(0, 0, width, height));
            editor_->setOpaque(true);
            editor_->setVisible(true);
        }

        // draw editor component onto image.
        Graphics g(*img_);
        editor_->paintEntireComponent(g, true);
        Image::BitmapData bitmap(*img_, Image::BitmapData::readOnly);
        memcpy(buffer, bitmap.data, width * height * 4);
    }

    void buttonPressed(int n, bool val) {
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

    void encoderPressed(int n, bool val) {
//        std::cerr << "encoderPressed " << n <<  " : " << val << std::endl;
        editor_->onEncoderSwitch(n, val);
    }

    void encoderTurned(int n, int val) {
//        std::cerr << "encoderTurned " << n <<  " : " << val << std::endl;
        editor_->onEncoder(n, (float) val);
    }

private:
    ssp::EditorHost *editor_;
    Image *img_ = nullptr;
};

class SSP_PluginInterface : public Percussa::SSP::PluginInterface {
public:
    SSP_PluginInterface(PluginProcessor *p) : processor_(p), editor_(nullptr) {
    }

    ~SSP_PluginInterface() {
//        if(editor_) delete editor_;
//        if(processor_) delete processor_;
    }

    Percussa::SSP::PluginEditorInterface *getEditor() override {
        if (editor_ == nullptr) {
            ssp::EditorHost *pluginEditor = static_cast<ssp::EditorHost *>( processor_->createEditor());
            editor_ = new SSP_PluginEditorInterface(pluginEditor);
        }
        return editor_;
    }

    void buttonPressed(int n, bool val) override {
        if (editor_) editor_->buttonPressed(n, val);
    }

    void encoderPressed(int n, bool val) override {
        if (editor_) editor_->encoderPressed(n, val);
    }

    void encoderTurned(int n, int val) override {
        if (editor_) editor_->encoderTurned(n, val);
    }

    void inputEnabled(int n, bool val) override {
        processor_->onInputChanged(n, val);
    }

    void outputEnabled(int n, bool val) override {
        processor_->onOutputChanged(n, val);
    }

    void getState(void **buffer, size_t *size) override {
        MemoryBlock state;
        processor_->getStateInformation(state);
        *size = state.getSize();
        *buffer = new char[*size];
        state.copyTo(*buffer, 0, *size);
    }

    void setState(void *buffer, size_t size) override {
        processor_->setStateInformation(buffer, size);
    }

    void setParam(int paramNr, float val) override {
//        processor_->setParameter(paramNr, val);
    }

    void prepare(double sampleRate, int samplesPerBlock) override {
        unsigned numIn = processor_->getBusCount(true);
        unsigned numOut = processor_->getBusCount(false);
        processor_->setPlayConfigDetails(
            numIn,
            numOut,
            sampleRate,
            samplesPerBlock);

        processor_->prepareToPlay(sampleRate, samplesPerBlock);
    }

    void process(float **channelData, int numChannels, int numSamples) override {
        MidiBuffer midiBuffer;
        AudioSampleBuffer buffer(channelData, numChannels, numSamples);
        processor_->processBlock(buffer, midiBuffer);
    }

private:
    SSP_PluginEditorInterface *editor_ = nullptr;
    PluginProcessor *processor_ = nullptr;
};


extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginDescriptor *createDescriptor() {
    return PluginProcessor::createDescriptor();
}

extern "C" __attribute__ ((visibility("default")))
Percussa::SSP::PluginInterface *createInstance() {
    return new SSP_PluginInterface(new PluginProcessor());

}

