#include "FileSelector.h"


namespace ssp {

FileSelector::FileSelector(const juce::String &defDir) : baseDir_(defDir) {
}


FileSelector::~FileSelector() {
}


void FileSelector::nextEntry() {
    if (selected_ == -1)
        selected_ = 0;
    else {
        if (selected_ + 1 < fileList_.size()) {
            selected_++;
            if ((selected_ - offset_) >= nPageEntries_) { offset_ += nFilePerCol; }
        }
    }
    repaint();
}

void FileSelector::prevEntry() {
    if (selected_ == -1)
        selected_ = 0;
    else {
        if (selected_ > 0) { selected_--; }
        if ((selected_ - offset_) < 0) { offset_ -= nFilePerCol; }
    }
    repaint();
}


void FileSelector::selectFile() {
    // used for directories
    if (selected_ == -1) return;
    int idx = 0;
    for (auto iter : fileList_) {
        if (idx == selected_) {
            auto file = iter.second;
            if (file.isDirectory()) {
                baseDir_ = file.getFullPathName();
                scanDir();
                selected_ = 0;
                offset_ = 0;
                repaint();
                return;
            }
        }
        idx++;
    }
}


juce::String FileSelector::selectedFile() {
    if (selected_ == -1) return "";

    int idx = 0;
    for (auto &iter : fileList_) {
        if (idx == selected_) {
            auto file = iter.second;
            if (file.isDirectory()) return "";
            return file.getFullPathName();
        }
        idx++;
    }
    return "";
}


void FileSelector::setFile(const juce::String &fullname) {
    juce::File f(fullname);
    if (!f.exists()) return;

    if (f.isDirectory()) {
        selected_ = -1;
        baseDir_ = f.getFullPathName();
        scanDir();
    } else {
        juce::File fdir(f.getParentDirectory());
        baseDir_ = fdir.getFullPathName();
        scanDir();
        selected_ = -1;
        int idx = 0;
        for (auto &iter : fileList_) {
            if (iter.second == f) { selected_ = idx; }
            idx++;
        }

        offset_ = 0;
        if (selected_ > nPageEntries_) {
            int page = selected_ / nPageEntries_;
            int col = (page * nMaxCols) - 1;
            offset_ = col * nFilePerCol;
        }
    }
}


void FileSelector::scanDir() {
    selected_ = -1;
    offset_ = 0;
    fileList_.clear();
    fileList_[".."] = juce::File(baseDir_).getParentDirectory();

    for (juce::DirectoryEntry entry :
         juce::RangedDirectoryIterator(juce::File(baseDir_), false, "*", juce::File::findFilesAndDirectories)) {
        if (!entry.isHidden()) { fileList_[entry.getFile().getFileName().toStdString()] = entry.getFile(); }
    }
}


void FileSelector::paint(juce::Graphics &g) {
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), 9 * COMPACT_UI_SCALE, juce::Font::plain));
    int idx = 0;
    for (auto iter : fileList_) {
        juce::String name = iter.first;
        bool isDir = iter.second.isDirectory();
        drawEntry(g, idx, selected_ == idx, name, isDir);
        idx++;
    }
}


void FileSelector::drawEntry(juce::Graphics &g, unsigned fidx, bool selected, const juce::String &name, bool isDir) {
    float w = 200;
    float h = 25;

    if (fidx < offset_ || fidx > (offset_ + (nFilePerCol * nMaxCols))) return;

    int idx = fidx - offset_;
    int r = idx % nFilePerCol;
    int c = idx / nFilePerCol;


    if (c >= nMaxCols) return;

    float y = (r * h) + 50;
    float x = (c * w) + 20;
    if (!selected) {
        g.setColour(bg_);
        g.fillRect(x, y, w, h);
        g.setColour(fg_);
    } else {
        g.setColour(fg_);
        g.fillRect(x, y, w, h);
        g.setColour(bg_);
    }

    juce::String fname = name;
    if (fidx < fileList_.size()) {
        if (isDir) { fname = "[" + name + "]"; }
        g.drawText(fname, x + 2, y + 2, w - 4, h - 4, juce::Justification::bottomLeft);
    }
}


void FileSelector::visibilityChanged() {
    if (isVisible()) {
        scanDir();
    } else {
        fileList_.clear();
    }
}


}  // namespace ssp
