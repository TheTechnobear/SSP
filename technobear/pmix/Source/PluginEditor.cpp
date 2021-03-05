#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/BaseParamControl.h"
#include "ssp/ButtonControl.h"

using pcontrol_type = ssp::LineParamControl;
using bcontrol_type = ssp::ButtonControl;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p,
                String(JucePlugin_Name) + " : " + String(JucePlugin_Desc),
                JucePlugin_VersionString, 3),
      processor_(p) {

    leftShiftBtn_.init("IN14", Colours::grey, Colours::black);
    setButtonBounds(leftShiftBtn_, 0, 4);
    addAndMakeVisible(leftShiftBtn_);
    rightShiftBtn_.init("IN58", Colours::grey, Colours::black);
    setButtonBounds(rightShiftBtn_, 0, 6);
    addAndMakeVisible(rightShiftBtn_);

    leftBtn_.label("EN-");
    rightBtn_.label("EN+");
    upBtn_.label("OUT14");
    downBtn_.label("MODE");


    for (unsigned v = 0; v < 3; v++) {

        std::reference_wrapper<TrackData> t0 = processor_.inputTrack(0);
        std::reference_wrapper<TrackData> t1 = processor_.inputTrack(1);
        std::reference_wrapper<TrackData> t2 = processor_.inputTrack(2);
        std::reference_wrapper<TrackData> t3 = processor_.inputTrack(3);
        if (v == 1) {
            t0 = processor_.inputTrack(4);
            t1 = processor_.inputTrack(5);
            t2 = processor_.inputTrack(6);
            t3 = processor_.inputTrack(7);
        } else if (v == 2) {
            // TODO check this!
            t0 = processor_.outputTrack(0);
            t1 = processor_.outputTrack(2);
            t2 = processor_.outputTrack(4);
            t3 = processor_.outputTrack(6);
        }
        TrackData &td0 = t0;
        TrackData &td1 = t1;
        TrackData &td2 = t2;
        TrackData &td3 = t3;
        float cc = 0.1f, fc = 0.01;
        addParamPage(
            std::make_shared<pcontrol_type>(td0.level[0], cc, fc),
            std::make_shared<pcontrol_type>(td1.level[0], cc, fc),
            std::make_shared<pcontrol_type>(td2.level[0], cc, fc),
            std::make_shared<pcontrol_type>(td3.level[0], cc, fc),
            v
        );
        addParamPage(
            std::make_shared<pcontrol_type>(td0.pan, cc, fc),
            std::make_shared<pcontrol_type>(td1.pan, cc, fc),
            std::make_shared<pcontrol_type>(td2.pan, cc, fc),
            std::make_shared<pcontrol_type>(td3.pan, cc, fc),
            v
        );
        addParamPage(
            std::make_shared<pcontrol_type>(td0.level[1], cc, fc),
            std::make_shared<pcontrol_type>(td1.level[1], cc, fc),
            std::make_shared<pcontrol_type>(td2.level[1], cc, fc),
            std::make_shared<pcontrol_type>(td3.level[1], cc, fc),
            v
        );
        addParamPage(
            std::make_shared<pcontrol_type>(td0.level[2], cc, fc),
            std::make_shared<pcontrol_type>(td1.level[2], cc, fc),
            std::make_shared<pcontrol_type>(td2.level[2], cc, fc),
            std::make_shared<pcontrol_type>(td3.level[2], cc, fc),
            v
        );
        addParamPage(
            std::make_shared<pcontrol_type>(td0.level[3], cc, fc),
            std::make_shared<pcontrol_type>(td1.level[3], cc, fc),
            std::make_shared<pcontrol_type>(td2.level[3], cc, fc),
            std::make_shared<pcontrol_type>(td3.level[3], cc, fc),
            v
        );
        addParamPage(
            std::make_shared<pcontrol_type>(td0.gain, cc, fc),
            std::make_shared<pcontrol_type>(td1.gain, cc, fc),
            std::make_shared<pcontrol_type>(td2.gain, cc, fc),
            std::make_shared<pcontrol_type>(td3.gain, cc, fc),
            v
        );
        unsigned fh = 24;
        auto mc = Colours::lightskyblue;
        addButtonPage(
            std::make_shared<bcontrol_type>(td0.mute, fh, mc),
            std::make_shared<bcontrol_type>(td1.mute, fh, mc),
            std::make_shared<bcontrol_type>(td2.mute, fh, mc),
            std::make_shared<bcontrol_type>(td3.mute, fh, mc),
            std::make_shared<bcontrol_type>(td0.solo, fh, mc),
            std::make_shared<bcontrol_type>(td1.solo, fh, mc),
            std::make_shared<bcontrol_type>(td2.solo, fh, mc),
            std::make_shared<bcontrol_type>(td3.solo, fh, mc),
            v
        );
        addButtonPage(
            std::make_shared<bcontrol_type>(td0.cue, fh, mc),
            std::make_shared<bcontrol_type>(td1.cue, fh, mc),
            std::make_shared<bcontrol_type>(td2.cue, fh, mc),
            std::make_shared<bcontrol_type>(td3.cue, fh, mc),
            std::make_shared<bcontrol_type>(td0.ac, fh, mc),
            std::make_shared<bcontrol_type>(td1.ac, fh, mc),
            std::make_shared<bcontrol_type>(td2.ac, fh, mc),
            std::make_shared<bcontrol_type>(td3.ac, fh, mc),
            v
        );
    }

    viewHeldCount_ = 0;

    inTracks_[0].init("In 1", &processor_.inputTrack(0));
    inTracks_[1].init("In 2", &processor_.inputTrack(1));
    inTracks_[2].init("In 3", &processor_.inputTrack(2));
    inTracks_[3].init("In 4", &processor_.inputTrack(3));
    inTracks_[4].init("In 5", &processor_.inputTrack(4));
    inTracks_[5].init("In 6", &processor_.inputTrack(5));
    inTracks_[6].init("In 7", &processor_.inputTrack(6));
    inTracks_[7].init("In 8", &processor_.inputTrack(7));
    for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
        inTracks_[i].active(false);
        addAndMakeVisible(inTracks_[i]);
    }

    outTracks_[0].init("Main", &processor_.outputTrack(0), &processor_.outputTrack(1));
    outTracks_[1].init("Cue", &processor_.outputTrack(2), &processor_.outputTrack(3));
    outTracks_[2].init("Aux 1", &processor_.outputTrack(4), &processor_.outputTrack(5));
    outTracks_[3].init("Aux 2", &processor_.outputTrack(6), &processor_.outputTrack(7));


    for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
        outTracks_[i].active(false);
        addAndMakeVisible(outTracks_[i]);
    }

    unheldView_ = view_;


    setSize(1600, 480);
}


void PluginEditor::timerCallback() {
    base_type::timerCallback();
    if (viewHeldCount_ > 0) viewHeldCount_ -= POLL_TIME;
}

void PluginEditor::paint(Graphics &g) {
    leftShiftBtn_.active(view_ == 0);
    rightShiftBtn_.active(view_ == 1);
    upBtn_.active(view_ == 2);

    base_type::paint(g);
    for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
        inTracks_[i].enabled(processor_.isInputEnabled(i));
    }

    for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
        outTracks_[i].enabled(processor_.isOutputEnabled(i * 2) || processor_.isOutputEnabled(i * 2 + 1));
    }
}


void PluginEditor::resized() {
    const unsigned space = 5;
    const unsigned inStart = space;
    const unsigned inw = 90;
    const unsigned h = 325;
    const unsigned y = 50;

    const unsigned outStart = 900;
    const unsigned outw = 145;

    for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
        unsigned x = inStart + (i * (space + inw));
        inTracks_[i].setBounds(x, y, inw, h);
    }

    for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
        unsigned x = outStart + (i * (space + outw));
        outTracks_[i].setBounds(x, y, outw, h);
    }
}


void PluginEditor::onEncoder(unsigned enc, float v) {
    base_type::onEncoder(enc, v);
}

void PluginEditor::onEncoderSwitch(unsigned enc, bool v) {
    base_type::onEncoderSwitch(enc, v);
}

void PluginEditor::onButton(unsigned btn, bool v) {
    base_type::onButton(btn, v);
}

void PluginEditor::onLeftButton(bool v) {
    leftBtn_.active(v);
    if (!v) {
        chgParamPage(-1, true);
    }
}

void PluginEditor::onRightButton(bool v) {
    rightBtn_.active(v);
    if (!v) {
        chgParamPage(1, true);
    }
}


void PluginEditor::onDownButton(bool v) {
    downBtn_.active(v);
    if (!v) {
        auto &view = views_[view_];
        unsigned bmode = buttonPage_ + 1 >= view.buttonPages_.size() ? 0 : buttonPage_ + 1;
        setButtonPage(bmode);
    }
}

void PluginEditor::onLeftShiftButton(bool v) {
    unsigned b = 0;
    if (v) {
        unheldView_ = view_;
        setView(b);
        viewHeldCount_ = BUT_COUNTER;
    } else {
        if (viewHeldCount_ == 0) {
            setView(unheldView_);
        }
        viewHeldCount_ = 0;
        unheldView_ = view_;
    }
}

void PluginEditor::onUpButton(bool v) {
    unsigned b = 2;
    if (v) {
        unheldView_ = view_;
        setView(b);
        viewHeldCount_ = BUT_COUNTER;
    } else {
        if (viewHeldCount_ == 0) {
            setView(unheldView_);
        }
        viewHeldCount_ = 0;
        unheldView_ = view_;
    }
}

void PluginEditor::onRightShiftButton(bool v) {
    unsigned b = 1;
    if (v) {
        unheldView_ = view_;
        setView(b);
        viewHeldCount_ = BUT_COUNTER;
    } else {
        if (viewHeldCount_ == 0) {
            setView(unheldView_);
        }
        viewHeldCount_ = 0;
        unheldView_ = view_;
    }
}









