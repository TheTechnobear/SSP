#include "MatrixView.h"

#include "Matrix.h"
#include "Track.h"

MatrixView::MatrixView(PluginProcessor& p) : ssp::MiniBasicView(&p, nullptr), processor_(p) {
}

MatrixView::~MatrixView() {
}


String MatrixView::channelName(bool inOut, unsigned mod, unsigned ch) {
    auto& track = processor_.track(trackIdx_);
    auto& module = track.modules_[mod];
    if (module.plugin_ && module.descriptor_) {
        String str = module.pluginName_;
        str += "/";
        if (inOut)
            str += String(module.descriptor_->outputChannelNames[ch]);
        else
            str += String(module.descriptor_->inputChannelNames[ch]);
        return str;
    }
    return String("Empty/") + String(ch + 1);
}

String MatrixView::routeAsString(unsigned modIdx, Matrix::Routing& route, bool output) {
    String str;
    switch (route.src_) {
        case Matrix::SRC_INPUT: {
            str += "IN/" + String(route.srcChannel_ + 1);
            break;
        }
        case Matrix::SRC_MOD: {
            str += channelName(true, Track::M_MOD, route.srcChannel_);
            break;
        }
        case Matrix::SRC_WORK: {
            unsigned preMod = modIdx;
            if (!output && preMod) preMod--;

            str += channelName(true, preMod, route.srcChannel_);
            break;
        }
        default: {
            str += "Error/" + String(route.srcChannel_ + 1);
            break;
        }
    }

    str += String(" -> ");

    if (output) {
        str += String("OUT/") + String(route.destChannel_ + 1);
    } else {
        str += channelName(false, modIdx, route.destChannel_);
    }

    return str;
}

void MatrixView::drawView(Graphics& g) {
    int hGap = 15 * COMPACT_UI_SCALE;
    int lGap = 10 * COMPACT_UI_SCALE;
    int txtOffset = 50 * COMPACT_UI_SCALE;
    int colOffset = 130 * COMPACT_UI_SCALE;

    auto& track = processor_.track(trackIdx_);
    auto& matrix = track.matrix_;

    MiniBasicView::drawView(g);
    int x = hGap;
    int y = hGap;

    g.setColour(Colours::yellow);
    g.setFont(Font(Font::getDefaultMonospacedFontName(), 20, Font::plain));

    g.drawSingleLineText("Matrix ( Track " + String(trackIdx_ + 1) + " )", x, y);

    y = hGap * 2;
    g.setColour(Colours::green);
    g.drawSingleLineText("Module Routing", x, y);

    g.setColour(Colours::white);
    unsigned modIdx = 0;
    for (auto& m : matrix.modules_) {
        // String mod;
        // switch (modIdx) {
        //     case 0: mod = "Mod"; break;
        //     case 1: mod = "Pre"; break;
        //     case 2: mod = "Main"; break;
        //     case 3: mod = "Post"; break;
        // }

        // y += lGap;
        // g.drawSingleLineText(mod, x, y);
        // g.drawSingleLineText(String(" : ") + processor_.getLoadedPlugin(trackIdx_, modIdx), x + txtOffset, y);
        for (auto& r : m.routes_) {
            y += lGap;
            String str = routeAsString(modIdx, r, false);
            g.drawSingleLineText(str, x, y);
        }
        modIdx++;
    }


    y = hGap * 2;
    x += colOffset;
    g.setColour(Colours::red);
    g.drawSingleLineText("Output Routing", x, y);

    g.setColour(Colours::white);
    auto& out = matrix.output_;
    for (auto& r : out.routes_) {
        y += lGap;
        String str = routeAsString(Track::M_POST, r, true);
        g.drawSingleLineText(str, x, y);
    }
}
