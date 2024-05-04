#include "PerfParamComponent.h"


PerfParamComponent::PerfParamComponent(PluginProcessor &p, PluginProcessor::PerformanceParam& param) : processor_(p), param_(param) {
}

void PerfParamComponent::paint(juce::Graphics &g) {
    auto r = getLocalBounds();
    g.setColour(juce::Colours::yellow);
    static constexpr unsigned fh = 14 * COMPACT_UI_SCALE;
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain));
    
    std::string name = std::string("Track " + std::to_string(param_.trackIdx() + 1) + ":" + param_.pluginName() + ":" +
                                param_.paramName());
    g.drawFittedText(name, r.removeFromTop(fh), juce::Justification::centred, 1);

    g.setColour(juce::Colours::white);
    std::string value = std::to_string(param_.value());
    g.drawFittedText(value, r.removeFromTop(fh), juce::Justification::centred, 1);
}


void PerfParamComponent::onSSPTimer() {
    auto plugin = processor_.getPlugin(param_.trackIdx(), param_.moduleIdx());
    if (plugin == nullptr) {
        return;
    }
    auto value = plugin->parameterValue(param_.paramIdx());
    if (value != param_.value()) {
        param_.value(value);
        repaint();
    }   

    repaint();
}

void PerfParamComponent::onEncoder(int enc,float v) {
    auto plugin = processor_.getPlugin(param_.trackIdx(), param_.moduleIdx());
    if (plugin == nullptr) {
        return;
    }
    auto value = plugin->parameterValue(param_.paramIdx());
    value += v / 10;
    plugin->parameterValue(param_.paramIdx(), value);
    param_.value(value);
    repaint();
}

void PerfParamComponent::onEncoderSwitch(int enc, bool v) {
    // float defValue = 0.0;
    // auto plugin = processor_.getPlugin(param_.trackIdx(), param_.moduleIdx());
    // if (plugin == nullptr) {
    //     return;
    // }
    // plugin->parameterValue(param_.paramIdx(), defValue);
    // param_.value(defValue);
    // repaint();
}
