include_directories("${PROJECT_SOURCE_DIR}/../common")

include_directories("${PROJECT_SOURCE_DIR}/../../external/readerwriterqueue")

set(COMMON_SRC
        ../common/SSP.cpp
        ../common/SSPApiEditor.cpp

        ../common/ssp/BaseView.cpp
        ../common/ssp/BaseProcessor.cpp
        ../common/ssp/SystemEditor.cpp
        ../common/ssp/SystemFullEditor.cpp
        ../common/ssp/SystemMiniEditor.cpp
        ../common/ssp/EditorHost.cpp
        ../common/ssp/SSPUI.cpp
        ../common/ssp/Log.cpp

        ../common/ssp/editors/BaseEditor.cpp
        ../common/ssp/editors/BaseViewEditor.cpp
        ../common/ssp/editors/SingleViewEditor.cpp
        ../common/ssp/editors/MultiViewEditor.cpp
        ../common/ssp/editors/MultiBarEditor.cpp
        ../common/ssp/editors/LineParamEditor.cpp
        ../common/ssp/editors/TableParamEditor.cpp
        ../common/ssp/editors/BarParamEditor.cpp
        ../common/ssp/editors/FileBrowser.cpp
        ../common/ssp/editors/TextEdit.cpp

        ../common/ssp/editors/BaseMiniView.cpp
        ../common/ssp/editors/MiniBasicView.cpp
        ../common/ssp/editors/MiniParamView.cpp
        ../common/ssp/editors/LineMiniEditor.cpp
        ../common/ssp/editors/PageMiniView.cpp

        ../common/ssp/controls/ParamControl.cpp
        ../common/ssp/controls/ParamButton.cpp
        ../common/ssp/controls/ValueControl.cpp
        ../common/ssp/controls/ValueButton.cpp
        ../common/ssp/controls/RmsTrack.cpp
        ../common/ssp/controls/VuMeter.cpp
        ../common/ssp/controls/MiniControl.cpp
        ../common/ssp/controls/ButtonBox.cpp
        ../common/ssp/controls/FileSelector.cpp
        ../common/ssp/controls/TextControl.cpp


)
