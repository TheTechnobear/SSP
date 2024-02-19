
#pragma once

#include <assert.h>
#include "PluginProcessor.h"
#include "ssp/controls/VuMeter.h"

using namespace juce;


class StereoChannel : public Component {
public:
    StereoChannel();

    void init(const String &label, TrackData *lData, TrackData *rData) {
        vuMeter_.init(label, true);
        lData_ = lData;
        rData_ = rData;
    }


    void active(bool b) { vuMeter_.active(b); }

    bool active() { return vuMeter_.active(); }

    void enabled(bool b) { vuMeter_.enabled(b); }

    bool enabled() { return vuMeter_.enabled(); }

    void paint(Graphics &) override;
    void resized() override;
private:
    ssp::StereoVuMeter vuMeter_;

    TrackData *lData_;
    TrackData *rData_;

    juce_UseDebuggingNewOperator
};
