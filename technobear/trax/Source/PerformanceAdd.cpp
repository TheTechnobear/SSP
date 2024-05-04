#include "PerformanceAdd.h"

#include "Module.h"
#include "Track.h"

PerformanceAdd::PerformanceAdd(PluginProcessor &p) : base_type(&p), processor_(p) {
    addAndMakeVisible(trackList_);
    addAndMakeVisible(moduleList_);
    addAndMakeVisible(paramList_);
}

PerformanceAdd::~PerformanceAdd() {
}


void PerformanceAdd::drawView(juce::Graphics &g) {
    base_type::drawView(g);
    g.setColour(juce::Colours::yellow);
    g.setFont(fh);
    g.drawText("Add Performance Parameter", canvasX(), 0, canvasWidth(), fh, juce::Justification::centredLeft);
}


void PerformanceAdd::resized() {
    base_type::resized();
    auto h = canvasHeight() - fh;
    auto w = canvasWidth();
    auto y = canvasY();
    auto x = canvasX() + fh;

    auto gap = 5 * COMPACT_UI_SCALE;
    auto gw = w / 4;
    trackList_.setBounds(x, y, gw - gap, h);
    moduleList_.setBounds(x + gw, y, gw - gap, h);
    paramList_.setBounds(x + gw * 2, y, (gw * 3), h);
}

void PerformanceAdd::refreshTrackList() {
    trackList_.clear();
    for (int trackIdx = 0; trackIdx < PluginProcessor::MAX_TRACKS; trackIdx++) {
        trackList_.addItem("Track " + std::to_string(trackIdx + 1));
    }
    curTrackIdx_ = 0xff;
    trackList_.idx(0);
    refreshModuleList();
}

void PerformanceAdd::refreshModuleList() {
    if (curTrackIdx_ == trackList_.idx()) { return; }
    curTrackIdx_ = trackList_.idx();

    moduleList_.clear();

    for (int moduleIdx = 0; moduleIdx < Track::MAX_MODULES; moduleIdx++) {
        auto name = processor_.getLoadedPlugin(curTrackIdx_, moduleIdx);
        moduleList_.addItem(std::to_string(moduleIdx) + "." + name);
    }

    curModuleIdx_ = 0xff;
    moduleList_.idx(0);
    refreshParamList();
}
void PerformanceAdd::refreshParamList() {
    if (curModuleIdx_ == moduleList_.idx()) { return; }

    paramList_.clear();
    curModuleIdx_ = moduleList_.idx();
    auto plugin = processor_.getPlugin(curTrackIdx_, curModuleIdx_);
    if (!plugin) return;
    int nParams = plugin->numberOfParameters();
    for (int paramIdx = 0; paramIdx < nParams; paramIdx++) {
        SSPExtendedApi::PluginInterface::ParameterDesc desc;
        if (plugin->parameterDesc(paramIdx, desc)) { paramList_.addItem(desc.name_); }
    }
    paramList_.idx(0);
}


void PerformanceAdd::editorShown() {
    refreshTrackList();
    base_type::editorShown();
}

void PerformanceAdd::onEncoder(unsigned id, float v) {
    if (id == 0) {
        if (v > 0) {
            trackList_.nextItem();
        } else {
            trackList_.prevItem();
        }
        refreshModuleList();
    } else if (id == 1) {
        if (v > 0) {
            moduleList_.nextItem();
        } else {
            moduleList_.prevItem();
        }
        refreshParamList();
    } else if (id == 2) {
        if (v > 0) {
            paramList_.nextItem();
        } else {
            paramList_.prevItem();
        }
    }
}

void PerformanceAdd::onEncoderSwitch(unsigned id, bool v) {
    if (v) return;

    if (id == 0) {
    } else if (id == 1) {
    } else if (id == 2) {
        auto pid = paramList_.idx();
        auto pluginName = processor_.getLoadedPlugin(curTrackIdx_, curModuleIdx_);
        auto plugin = processor_.getPlugin(curTrackIdx_, curModuleIdx_);
        if (!plugin) return;


        SSPExtendedApi::PluginInterface::ParameterDesc desc;
        if (plugin->parameterDesc(pid, desc)) {
            PluginProcessor::PerformanceParam param(curTrackIdx_, curModuleIdx_, pid, pluginName, desc.name_, desc.min_,
                                                    desc.max_, desc.def_, desc.numSteps_, desc.isDescrete_);
            processor_.addPerformanceParam(param);
        }
    }
}
