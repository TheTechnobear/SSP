#include "PerfParamComponent.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

inline juce::String toString(float v) {
    if (v > -0.0001f && v < 0.0001f) { return juce::String(0.0f, 2, false); }
    return juce::String(v, 2, false);
}

PerfParamComponent::PerfParamComponent(PluginProcessor &p, PluginProcessor::PerformanceParam &param, juce::Colour fg)
    : processor_(p), param_(param), fg_(fg) {
    if (param_.isDescrete()) {
        coarseInc_ = (param_.max() - param_.min()) / (param_.numSteps() - 1);
        fineInc_ = coarseInc_;
    } else {
        float range = param_.max() - param_.min();
        coarseInc_ = range / 10.f;
        fineInc_ = coarseInc_ / 10.f;
    }
}

void PerfParamComponent::onSSPTimer() {
    auto plugin = processor_.getPlugin(param_.trackIdx(), param_.moduleIdx());
    if (plugin == nullptr) { return; }
    auto value = plugin->parameterValue(param_.paramIdx());
    param_.value(value);
    repaint();
}

void PerfParamComponent::sendToPlugin(float v) {
    auto plugin = processor_.getPlugin(param_.trackIdx(), param_.moduleIdx());
    if (plugin == nullptr) { return; }
    plugin->parameterValue(param_.paramIdx(), v);
}

void PerfParamComponent::onEncoder(float v) {
    if (v > 0.f) {
        inc(encState_);
    } else {
        dec(encState_);
    }
    encReset = false;
}

void PerfParamComponent::onEncoderSwitch(bool v) {
    encState_ = v;
    if (v) {
        encReset = true;
    } else {
        if (encReset) { reset(); }
        encReset = false;
    }
}

void PerfParamComponent::inc(bool fine) {
    float inc = fine ? fineInc_ : coarseInc_;
    float nv = param_.value() + (inc != 0 ? inc : 0.01f);
    set(nv);
}

void PerfParamComponent::dec(bool fine) {
    float inc = fine ? fineInc_ : coarseInc_;
    float nv = param_.value() - (inc != 0 ? inc : 0.01f);
    set(nv);
}

void PerfParamComponent::reset() {
    set(param_.def());
}


void PerfParamComponent::set(float v) {
    float nv = constrain(v, param_.min(), param_.max());
    if (nv != param_.value()) {
        param_.value(nv);
        sendToPlugin(param_.value());
    }
}


void PerfParamComponent::paint(juce::Graphics &g) {
    static constexpr unsigned fh = 16 * COMPACT_UI_SCALE;
    int h = getHeight();
    int w = getWidth();

    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain)));

    g.setColour(active() ? fg_ : juce::Colours::grey);
    g.drawText(param_.paramName(), 0, 0, w, fh, juce::Justification::centred);

    g.setColour(active() ? juce::Colours::darkcyan : juce::Colours::grey);
    float range = param_.max() - param_.min();
    float nval = (param_.value() - param_.min()) / range;

    int be = (w - 4) * nval;
    bool biPolar = param_.min() < 0.f && param_.max() > 0.f;
    if (!biPolar) {
        g.fillRect(2, h / 2 + 2, be, fh - 4);
    } else {
        float v = nval - 0.5f;
        int bl = (w - 4) * v;
        int bs = (bl > 0.0f ? 0.5f : 0.5 + v) * (w - 4);
        g.fillRect(bs, int(h / 2) + 2, (bl > 0 ? bl : -bl), fh - 4);
    }

    g.setColour(juce::Colours::white);
    g.drawVerticalLine(be + 2, int(h / 2) + 2, int(h / 2) + 2 + fh - 4);


    if (active()) {
        g.setColour(active() ? juce::Colours::white : juce::Colours::lightgrey);
        auto val = !param_.isDescrete() ? toString(param_.value()) : juce::String(param_.value(), 0, false);
        g.drawText(val, 1, (h / 2) + 1, w - 2, fh - 2, juce::Justification::centred);
    }
}