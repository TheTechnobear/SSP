#include "PerformanceEdit.h"

PerformanceEdit::PerformanceEdit(PluginProcessor& p) : base_type(&p), processor_(p) {
    auto addBtn = std::make_shared<ssp::ValueButton>("Add", [this](bool b) {});
    auto delBtn = std::make_shared<ssp::ValueButton>("Delete", [this](bool b) {
        if (!b) deleteParam();
    });
    addButton(0, addBtn);
    addButton(1, delBtn);
    addAndMakeVisible(paramList_);
}


void PerformanceEdit::drawView(juce::Graphics& g) {
    base_type::drawView(g);
    g.setColour(juce::Colours::yellow);
    g.setFont(fh);
    g.drawText("Performance Parameters", canvasX(), 0, canvasWidth(), fh, juce::Justification::centredLeft);
}

PerformanceEdit::~PerformanceEdit() {
}

void PerformanceEdit::resized() {
    base_type::resized();
    auto r = getLocalBounds();
    paramList_.setBounds(r.removeFromTop(fh));
}

void PerformanceEdit::editorShown() {
    refreshParamList();
    base_type::editorShown();
}

void PerformanceEdit::onEncoder(unsigned id, float v) {
    if (id == 0) {
        if (v > 0) {
            paramList_.nextItem();
        } else {
            paramList_.prevItem();
        }
    }
}

void PerformanceEdit::onEncoderSwitch(unsigned id, bool v) {
}

void PerformanceEdit::refreshParamList() {
    paramList_.clear();
    for (auto& param : processor_.performanceParams()) {
        std::string p = std::string("Track " + std::to_string(param.trackIdx() + 1) + ":" + param.pluginName() + ":" +
                                    param.paramName());
        paramList_.addItem(p);
    }
}

void PerformanceEdit::deleteParam() {
    auto idx = paramList_.idx();
    auto param = processor_.performanceParams()[idx];
    processor_.removePerformanceParam(param);
    refreshParamList();
}