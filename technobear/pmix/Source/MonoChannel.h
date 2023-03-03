
#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ssp/controls/VuMeter.h"

class MonoChannel : public Component {
public:
    MonoChannel();

    void init(const String &label, TrackData *data) {
        vuMeter_.init(label, true);
        data_ = data;
    }

    void active(bool b) { vuMeter_.active(b); }

    bool active() { return vuMeter_.active(); }

    void enabled(bool b) { vuMeter_.enabled(b); }

    bool enabled() { return vuMeter_.enabled(); }


    void paint(Graphics &) override;
    void resized() override;
private:
    ssp::MonoVuMeter vuMeter_;
    TrackData *data_;

    juce_UseDebuggingNewOperator
};
