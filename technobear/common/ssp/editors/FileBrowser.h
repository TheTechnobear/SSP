#pragma once

#include "BaseEditor.h"

#include <juce_gui_basics/juce_gui_basics.h>
using namespace juce;

namespace ssp {


class FileBrowser : public BaseEditor {

public:
    explicit FileBrowser(BaseProcessor *p, const String& defDir);
    virtual ~FileBrowser();

    void drawView(juce::Graphics &g) override;

    void editorShown() override;
    void editorHidden() override;

    void onEncoder(unsigned enc, float v) override;
    void onEncoderSwitch(unsigned enc, bool v) override;

    void eventUp(bool v) override;
    void eventDown(bool v) override;
    void eventRight(bool v) override;
    void eventLeft(bool v) override;
    void eventRightShift(bool v) override;

    juce::String selectedFile();
    juce::String baseDir() { return baseDir_;}
    void setFile(String& fullname);

private:

    using base_type = BaseEditor;
    void drawEntry(juce::Graphics &g,unsigned idx, bool selected, const juce::String &name, bool isDir);
    void scanDir();
    void selectFile();

    std::map<std::string,File>  fileList_;
    int selected_ = -1;
    int offset_ = 0;

    juce::Colour fg_=Colours::red;
    juce::Colour bg_=defaultBg_;
    static constexpr int nFilePerCol = 15;
    static constexpr int nMaxCols = 4;
    static constexpr int nPageEntries_ = nFilePerCol * nMaxCols;

    juce::String baseDir_ = "~";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileBrowser)

};

}




