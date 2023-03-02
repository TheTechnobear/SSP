#pragma once

#include "SSPExApi.h"
#include "ssp/EditorHost.h"

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

class SSP_PluginEditorInterface : public SSPExtendedApi::PluginEditorInterface {
public:
    explicit SSP_PluginEditorInterface(ssp::EditorHost *editor);
    ~SSP_PluginEditorInterface() override;

    // SSP API
    void frameStart() override;
    void visibilityChanged(bool b) override;
    void renderToImage(unsigned char *buffer, int width, int height) override;

    // other
    void buttonPressed(int n, bool val);
    void encoderPressed(int n, bool val);
    void encoderTurned(int n, int val);
private:
    ssp::EditorHost *editor_;
};
