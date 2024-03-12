#include "FileBrowser.h"


namespace ssp {

static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;


FileBrowser::FileBrowser(BaseProcessor *p, const String &defDir) : BaseEditor(p), baseDir_(defDir) {
    setSize(1600, 480);
    int bfh = 48;
    upBtn_.label("^", bfh);
    downBtn_.label("v", bfh);
    leftBtn_.label("<", bfh);
    rightBtn_.label(">", bfh);

    addAndMakeVisible(rightBtn_);
    addAndMakeVisible(leftBtn_);

    leftShiftBtn_.setVisible(false);
    rightShiftBtn_.label("Sel");
    addAndMakeVisible(rightShiftBtn_);}


FileBrowser::~FileBrowser() {
}


void FileBrowser::selectFile() {
    // used for directories
    if (selected_ == -1) return;
    int idx = 0;
    for (auto iter: fileList_) {
        if (idx == selected_) {
            auto file = iter.second;
            if (file.isDirectory()) {
                baseDir_ = file.getFullPathName();
                scanDir();
                selected_ = 0;
                offset_ = 0;
                return;
            }
        }
        idx++;
    }
}


String FileBrowser::selectedFile() {
    if (selected_ == -1) return "";

    int idx = 0;
    for (auto& iter: fileList_) {
        if (idx == selected_) {
            auto file = iter.second;
            if (file.isDirectory()) return "";
            return file.getFullPathName();
        }
        idx++;
    }
    return "";
}



void FileBrowser::setFile(String &fullname) {
    File f(fullname);
    if (!f.exists()) return;

    if (f.isDirectory()) {
        selected_ = -1;
        baseDir_ = f.getFullPathName();
        scanDir();
    } else {
        File fdir(f.getParentDirectory());
        baseDir_ = fdir.getFullPathName();
        scanDir();
        selected_ = -1;
        int idx = 0;
        for (auto& iter: fileList_) {
            if (iter.second == f) {
                selected_ = idx;
            }
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


void FileBrowser::scanDir() {
    selected_ = -1;
    offset_ = 0;
    fileList_.clear();
    fileList_[".."] = File(baseDir_).getParentDirectory();

    for (DirectoryEntry entry: RangedDirectoryIterator(File(baseDir_),
                                                       false, "*",
                                                       File::findFilesAndDirectories)) {
        if (!entry.isHidden()) {
            fileList_[entry.getFile().getFileName().toStdString()] = entry.getFile();
        }
    }
}


void FileBrowser::drawView(Graphics &g) {
    // display 1600x 480
    // x= left/right (0..1599)
    // y= top/bottom (0..479)
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 18, Font::plain));


    int idx = 0;

    for (auto iter: fileList_) {
        String name = iter.first;
        bool isDir = iter.second.isDirectory();
        drawEntry(g, idx, selected_ == idx, name, isDir);
        idx++;
    }
}


void FileBrowser::drawEntry(Graphics &g,
                            unsigned fidx, bool selected,
                            const String &name, bool isDir) {
    float w = 200;
    float h = 25;

    if (fidx < offset_ || fidx > (offset_ + (nFilePerCol * nMaxCols)))
        return;

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

    String fname = name;
    if (fidx < fileList_.size()) {
        if (isDir) {
            fname = "[" + name + "]";
        }
        g.drawText(fname, x + 2, y + 2, w - 4, h - 4, Justification::bottomLeft);
    }

}


void FileBrowser::editorShown() {
    scanDir();
}

void FileBrowser::editorHidden() {
    fileList_.clear();
}


void FileBrowser::onEncoder(unsigned id, float v) {
    base_type::onEncoder(id, v);
    switch (id) {
        case 0 : {
            if (v > 0) {
                if (selected_ == -1) selected_ = 0;
                else {
                    if (selected_ + 1 < fileList_.size()) {
                        selected_++;
                        if ((selected_ - offset_) >= nPageEntries_) {
                            offset_ += nFilePerCol;
                        }
                    }
                }
            } else {
                if (selected_ == -1) selected_ = 0;
                else {
                    if (selected_ > 0) {
                        selected_--;
                        if ((selected_ - offset_) < 0) {
                            offset_ -= nFilePerCol;
                        }
                    }
                }
            }
            break;
        }
        default:;
    }
}

void FileBrowser::onEncoderSwitch(unsigned id, bool v) {
    base_type::onEncoderSwitch(id, v);
    if (v) return;
    switch (id) {
        case 0 : {
            selectFile();
            break;
        }
        default:;
    }
}

void FileBrowser::eventUp(bool v) {
    base_type::eventUp(v);
    if (!v) {
        if (selected_ == -1) selected_ = 0;
        else {
            if (selected_ > 0) {
                selected_--;
            }
            if ((selected_ - offset_) < 0) {
                offset_ -= nFilePerCol;
            }
        }
    }
}

void FileBrowser::eventDown(bool v) {
    base_type::eventDown(v);
    if (!v) {
        if (selected_ == -1) selected_ = 0;
        else {
            if (selected_ + 1 < fileList_.size()) {
                selected_++;
                if ((selected_ - offset_) >= nPageEntries_) {
                    offset_ += nFilePerCol;
                }
            }
        }
    }
}


void FileBrowser::eventLeft(bool v) {
    base_type::eventLeft(v);
    if (!v) {
        if (selected_ == -1) selected_ = 0;
        else {
            if (selected_ - nFilePerCol >= 0) {
                selected_ -= nFilePerCol;
            }
            if (offset_ > 0 && (selected_ - offset_) < 0) {
                offset_ -= nFilePerCol;
            }
        }
    }
}

void FileBrowser::eventRight(bool v) {
    base_type::onRightButton(v);
    if (!v) {
        if (selected_ == -1) selected_ = 0;
        else {
            if (selected_ + nFilePerCol < fileList_.size()) {
                selected_ += nFilePerCol;
            }
            if ((selected_ - offset_) >= nPageEntries_) {
                offset_ += nFilePerCol;
            }
        }
    }
}

 void FileBrowser::eventRightShift(bool v) {
     base_type::eventRightShift(v);
     if(!v) {
         selectFile();
     }
 }



}
