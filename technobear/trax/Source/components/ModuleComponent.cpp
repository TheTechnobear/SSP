#include "ModuleComponent.h"

ModuleComponent::ModuleComponent(PluginProcessor &p, int trackIdx, int slotIdx, juce::Colour fg)
    : processor_(p), trackIdx_(trackIdx), slotIdx_(slotIdx), fg_(fg){
}

void ModuleComponent::paint(juce::Graphics &g) {
    g.setColour(fg_);
    static constexpr unsigned fh = 14 * COMPACT_UI_SCALE;
    g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(), fh, juce::Font::plain));
    String name = processor_.getLoadedPlugin(trackIdx_, slotIdx_);
    g.drawFittedText(String(slotIdx_) + "." + name, getLocalBounds(), juce::Justification::centred, 1);
    g.drawRect(getLocalBounds(), COMPACT_UI_SCALE);
}
