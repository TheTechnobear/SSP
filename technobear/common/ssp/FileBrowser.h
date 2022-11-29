#pragma once

#include "BaseEditor.h"

namespace ssp {


class FileBrowser : public BaseEditor {

public:
    explicit FileBrowser(BaseProcessor *p, const String& defDir);
    virtual ~FileBrowser();

    void drawView(Graphics &g) override;

    void editorShown() override;
    void editorHidden() override;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;
    void onButton(unsigned btn, bool v) override;
    void onUpButton(bool v) override;
    void onDownButton(bool v) override;
    void onLeftButton(bool v) override;
    void onRightButton(bool v) override;
    // void onLeftShiftButton(bool v) override;
     void onRightShiftButton(bool v) override;

    String selectedFile();
    String baseDir() { return baseDir_;}
    void setFile(String& fullname);

private:

    using base_type = BaseEditor;
    void drawEntry(Graphics &g,unsigned idx, bool selected, const String &name, bool isDir);
    void scanDir();
    void selectFile();

    std::map<std::string,File>  fileList_;
    int selected_ = -1;
    int offset_ = 0;

    Colour fg_=Colours::red;
    Colour bg_=defaultBg_;
    static constexpr int nFilePerCol = 15;
    static constexpr int nMaxCols = 4;
    static constexpr int nPageEntries_ = nFilePerCol * nMaxCols;

    String baseDir_ = "~";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileBrowser)

};

}




