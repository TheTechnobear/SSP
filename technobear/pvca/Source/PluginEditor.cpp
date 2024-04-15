
#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "ssp/controls/ParamButton.h"
#include "ssp/controls/ParamControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ParamButton;

static constexpr unsigned MAX_VIEW = 1;
static constexpr unsigned L_CLRS = 4;

PluginEditor::PluginEditor(PluginProcessor &p) : base_type(&p, MAX_VIEW), processor_(p) {
    static juce::Colour clrs[] = { Colours::red, Colours::blue, Colours::yellow, Colours::green };
    unsigned view = 0;
    unsigned page = 0;

    for (unsigned vid = 0; vid < PluginProcessor::MAX_VCA; vid++) {
        auto &vca = processor_.getVcaParam(vid);

        addParamPage(std::make_shared<pcontrol_type>(vca.gain, 0.1f, 0.01f),
                     std::make_shared<pcontrol_type>(vca.pan, 0.1f, 0.01f), nullptr, nullptr, view, clrs[vid % L_CLRS]);
        page++;
    }

    setSize(1600, 480);
}
