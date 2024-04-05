#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <memory>

namespace ssp {

class FileSelector : public juce::Component {
public:
    explicit FileSelector(const juce::String &defDir);
    ~FileSelector();

    void nextEntry();
    void prevEntry();
    void selectFile();

    juce::String selectedFile();
    juce::String baseDir() { return baseDir_; }
    void setFile(const juce::String &fullname);

private:
    void paint(juce::Graphics &g) override;
    // void resized() override;
    void visibilityChanged() override;
    void drawEntry(juce::Graphics &g, unsigned idx, bool selected, const juce::String &name, bool isDir);
    void scanDir();

    std::map<std::string, juce::File> fileList_;
    int selected_ = -1;
    int offset_ = 0;

    juce::Colour fg_ = juce::Colours::red;
    juce::Colour bg_ = juce::Colours::black;
    static constexpr int nFilePerCol = 15;
    static constexpr int nMaxCols = 2;
    static constexpr int nPageEntries_ = nFilePerCol * nMaxCols;

    juce::String baseDir_ = "~";
};


}  // namespace ssp