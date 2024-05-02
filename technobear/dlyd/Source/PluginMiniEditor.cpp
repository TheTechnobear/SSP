#include "PluginMiniEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/MiniControl.h"

using pcontrol_type = ssp::MiniControl;
using bcontrol_type = ssp::ParamButton;


PluginMiniEditor::PluginMiniEditor(PluginProcessor &p) : base_type(&p), processor_(p) {
    static constexpr unsigned L_CLRS = 5;
    static constexpr unsigned MAX_TAPS = PluginProcessor::MAX_TAPS;
    static juce::Colour clrs[L_CLRS] = { Colours::red, Colours::blue, Colours::yellow, Colours::green, Colours::cyan };

    static constexpr float defCoarse = 10.0f;
    static constexpr float defFine = 0.1f;

    addButtonPage(
        std::make_shared<bcontrol_type>(processor_.params_.freeze, 12 * COMPACT_UI_SCALE, Colours::lightskyblue),
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    addParamPage(std::make_shared<pcontrol_type>(processor_.params_.size, defCoarse, defFine),
                 std::make_shared<pcontrol_type>(processor_.params_.mix, defCoarse, defFine),
                 std::make_shared<pcontrol_type>(processor_.params_.in_level, defCoarse, defFine),
                 std::make_shared<pcontrol_type>(processor_.params_.out_level, defCoarse, defFine), Colours::orange);

    for (unsigned tap = 0; tap < MAX_TAPS; tap++) {
        auto &t = processor_.getTap(tap);
        auto clr = clrs[tap % L_CLRS];

        addParamPage(std::make_shared<pcontrol_type>(t.time, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(t.level, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(t.feedback, defCoarse, defFine),
                     std::make_shared<pcontrol_type>(t.pan, 0.1, 0.01), clr);
        addParamPage(std::make_shared<pcontrol_type>(t.lpf, 100, 5), std::make_shared<pcontrol_type>(t.hpf, 100, 5),
                     std::make_shared<pcontrol_type>(t.noise, 1, 0.01), nullptr, clr);

    }
}
