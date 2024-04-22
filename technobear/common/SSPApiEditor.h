#pragma once

#include "SSPExApi.h"
#include "ssp/EditorHost.h"


class SSP_PluginEditorInterface : public SSPExtendedApi::PluginEditorInterface {
public:
    explicit SSP_PluginEditorInterface(ssp::EditorHost *editor);
    ~SSP_PluginEditorInterface() override;

    // SSP API
    void frameStart() override;
    void visibilityChanged(bool b) override;
    void renderToImage(unsigned char *buffer, int width, int height) override;

    juce::Component* editorComponent() override { return editor_->getEditorComponent();}

    // other
    void buttonPressed(int n, bool val);
    void encoderPressed(int n, bool val);
    void encoderTurned(int n, int val);
private:
    ssp::EditorHost *editor_;
    void generateButtenEvents(int n, bool val);
    bool buttonState_[SSP_LastBtn] = { false, false, false, false, false, false, false,
                                       false, false, false, false, false, false, false };
    unsigned buttonCounter_[SSP_LastBtn] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

};
