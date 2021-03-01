#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/BaseParamControl.h"
#include "ssp/ButtonControl.h"

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ButtonControl;

PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p,
                String(JucePlugin_Name) + " : " + String(JucePlugin_Desc),
                JucePlugin_VersionString),
      processor_(p) {
/*
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.pitch),
        std::make_shared<pcontrol_type>(processor_.params_.harmonics),
        std::make_shared<pcontrol_type>(processor_.params_.timbre),
        std::make_shared<pcontrol_type>(processor_.params_.morph)
    );

    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.model, 1.0f, 1.0f),
        std::make_shared<pcontrol_type>(processor_.params_.freq_mod),
        std::make_shared<pcontrol_type>(processor_.params_.timbre_mod),
        std::make_shared<pcontrol_type>(processor_.params_.morph_mod)
    );
    addParamPage(
        std::make_shared<pcontrol_type>(processor_.params_.lpg),
        std::make_shared<pcontrol_type>(processor_.params_.vca),
        nullptr,
        nullptr);
*/
    setSize(1600, 480);

    for (unsigned i = 0; i < TS_MAX; i++) {
        buttonHeldCount_[i] = 0;
    }
    encMode_ = 0;
    butMode_ = 0;
    activeTracks_ = OUT_14;
    curTracks_ = OUT_14;

    inTracks_[0].init(&params_[0], "In 1", &processor_.inputTrack(0));
    inTracks_[1].init(&params_[1], "In 2", &processor_.inputTrack(1));
    inTracks_[2].init(&params_[2], "In 3", &processor_.inputTrack(2));
    inTracks_[3].init(&params_[3], "In 4", &processor_.inputTrack(3));
    inTracks_[4].init(&params_[0], "In 5", &processor_.inputTrack(4));
    inTracks_[5].init(&params_[1], "In 6", &processor_.inputTrack(5));
    inTracks_[6].init(&params_[2], "In 7", &processor_.inputTrack(6));
    inTracks_[7].init(&params_[3], "In 8", &processor_.inputTrack(7));
    for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
        inTracks_[i].active(false);
        addAndMakeVisible(inTracks_[i]);
    }

    outTracks_[0].init(&params_[0], "Main", &processor_.outputTrack(0), &processor_.outputTrack(1));
    outTracks_[1].init(&params_[1], "Cue", &processor_.outputTrack(2), &processor_.outputTrack(3));
    outTracks_[2].init(&params_[2], "Aux 1", &processor_.outputTrack(4), &processor_.outputTrack(5));
    outTracks_[3].init(&params_[3], "Aux 2", &processor_.outputTrack(6), &processor_.outputTrack(7));


    for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
        outTracks_[i].active(false);
        addAndMakeVisible(outTracks_[i]);
    }


    butMode_ = Channel::BM_SOLOMUTE;
    labelButtons();


    trackSelect(activeTracks_, true);
}


void PluginEditor::timerCallback() {
    base_type::timerCallback();
    for (unsigned i = 0; i < TS_MAX; i++) {
        if (buttonHeldCount_[i] > 0) buttonHeldCount_[i] -= POLL_TIME;
    }
}

void PluginEditor::paint(Graphics &g) {
    base_type::paint(g);
    for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
        inTracks_[i].enabled(processor_.isInputEnabled(i));
    }

    for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
        outTracks_[i].enabled(processor_.isOutputEnabled(i * 2) || processor_.isOutputEnabled(i * 2 + 1));
    }

#if 0
    switch (butMode_) {
        case SSPChannel::BM_SOLOMUTE: {
            switch (curTracks_) {
                case IN_14: {
                    for (unsigned i = 0; i < 4; i++) buttons_[B_A_1 + i].active(processor_.inputTrack(i).solo_);
                    for (unsigned i = 0; i < 4; i++) buttons_[B_B_1 + i].active(processor_.inputTrack(i).mute_);
                    break;
                }
                case IN_58: {
                    for (unsigned i = 0; i < 4; i++) buttons_[B_A_1 + i].active(processor_.inputTrack(4 + i).solo_);
                    for (unsigned i = 0; i < 4; i++) buttons_[B_B_1 + i].active(processor_.inputTrack(4 + i).mute_);
                    break;
                }
                case OUT_14: {
                    for (unsigned i = 0; i < 4; i++) buttons_[B_A_1 + i].active(processor_.outputTrack(i * 2).solo_);
                    for (unsigned i = 0; i < 4; i++) buttons_[B_B_1 + i].active(processor_.outputTrack(i * 2).mute_);
                    break;
                }
                default:
                    ;
            }
        }
            break;
        case SSPChannel::BM_CUEAC: {
            switch (curTracks_) {
                case IN_14: {
                    for (unsigned i = 0; i < 4; i++) buttons_[B_A_1 + i].active(processor_.inputTrack(i).cue_);
                    for (unsigned i = 0; i < 4; i++) buttons_[B_B_1 + i].active(processor_.inputTrack(i).ac_);
                    break;
                }
                case IN_58: {
                    for (unsigned i = 0; i < 4; i++) buttons_[B_A_1 + i].active(processor_.inputTrack(4 + i).cue_);
                    for (unsigned i = 0; i < 4; i++) buttons_[B_B_1 + i].active(processor_.inputTrack(4 + i).ac_);
                    break;
                }
                case OUT_14: {
                    for (unsigned i = 0; i < 4; i++) buttons_[B_A_1 + i].active(processor_.outputTrack(i * 2).cue_);
                    for (unsigned i = 0; i < 4; i++) buttons_[B_B_1 + i].active(processor_.outputTrack(i * 2).ac_);
                    break;
                }
                default:
                    ;
            }
        }
            break;

        default:
            break;
    }
#endif
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
    channelEncoder(0, v > 0.0f ? 1 : -1);
}

void PluginEditor::onEncoderSwitch(unsigned enc, bool v) {
    if (!v) channelEncoderButton(0, v);
}

void PluginEditor::onButton(unsigned btn, bool v) {
    if (!v) channelButton(0, btn, v);
}

void PluginEditor::onLeftButton(bool v) {
    base_type::onLeftButton(v);
    if (!v) {
        encMode_ = encMode_ > 0 ? encMode_ - 1 : (Channel::EM_MAX - 1);
        auto m = static_cast<Channel::EncMode>(encMode_);
        for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
            inTracks_[i].encoderMode(m);
        }
        for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
            outTracks_[i].encoderMode(m);
        }
    }
}

void PluginEditor::onRightButton(bool v) {
    base_type::onRightButton(v);
    if (!v) {
        encMode_ = encMode_ < (Channel::EM_MAX - 1) ? encMode_ + 1 : 0;
        auto m = static_cast<Channel::EncMode>(encMode_);
        for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
            inTracks_[i].encoderMode(m);
        }
        for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
            outTracks_[i].encoderMode(m);
        }
    }
}

void PluginEditor::onUpButton(bool v) {
    base_type::onUpButton(v);
    trackSelect(OUT_14, v);
}

void PluginEditor::onDownButton(bool v) {
    base_type::onDownButton(v);
    if (!v) {
        butMode_ = butMode_ < (Channel::BM_MAX - 1) ? butMode_ + 1 : 0;
        auto m = static_cast<Channel::ButMode>(butMode_);
        for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
            inTracks_[i].buttonMode(m);
        }
        for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
            outTracks_[i].buttonMode(m);
        }
        labelButtons();


    }
}

void PluginEditor::onLeftShiftButton(bool v) {
    trackSelect(IN_14, v);
}

void PluginEditor::onRightShiftButton(bool v) {
    trackSelect(IN_58, v);
}


void PluginEditor::channelEncoder(unsigned c, float v) {
    switch (curTracks_) {
        case IN_14: {
            inTracks_[c].encoder(v);
            break;
        }
        case IN_58: {
            inTracks_[c + 4].encoder(v);
            break;
        }
        case OUT_14: {
            outTracks_[c].encoder(v);
            break;
        }
        default:
            break;
    }
}


void PluginEditor::channelEncoderButton(unsigned c, bool v) {
    switch (curTracks_) {
        case IN_14: {
            inTracks_[c].encbutton(v);
            break;
        }
        case IN_58: {
            inTracks_[c + 4].encbutton(v);
            break;
        }
        case OUT_14: {
            outTracks_[c].encbutton(v);
            break;
        }
        default:
            break;
    }
}


void PluginEditor::channelButton(unsigned c, unsigned i, bool v) {
    switch (curTracks_) {
        case IN_14: {
            inTracks_[c].button(i, v);
            break;
        }
        case IN_58: {
            inTracks_[c + 4].button(i, v);
            break;
        }
        case OUT_14: {
            outTracks_[c].button(i, v);
            break;
        }
        default:
            break;
    }
}

bool PluginEditor::buttonState(unsigned c, unsigned i) {
    switch (curTracks_) {
        case IN_14: {
            return inTracks_[c].button(i);
        }
        case IN_58: {
            return inTracks_[c + 4].button(i);
        }
        case OUT_14: {
            return outTracks_[c].button(i);
        }
        default:
            break;
    }

    return false;
}


void PluginEditor::trackSelect(TrackSelect ts, bool active) {
    if (active) {
        buttonHeldCount_[ts] = BUT_COUNTER;
        curTracks_ = ts;
    } else {
        if (buttonHeldCount_[ts] > 0) {
            activeTracks_ = ts;
        }
        curTracks_ = activeTracks_;
        buttonHeldCount_[ts] = 0;
    }

//	buttons_[B_SHIFTL].active(curTracks_ == IN_14);
//	buttons_[B_UP].active(curTracks_ == OUT_14);
//	buttons_[B_SHIFTR].active(curTracks_ == IN_58);

    for (unsigned i = 0; i < PluginProcessor::IN_T_MAX; i++) {
        inTracks_[i].active((curTracks_ == IN_14 && i < 4) || (curTracks_ == IN_58 && i >= 4));
    }

    for (unsigned i = 0; i < (PluginProcessor::OUT_T_MAX / 2); i++) {
        outTracks_[i].active(curTracks_ == OUT_14);
    }
}


void PluginEditor::labelButtons() {
    /*
    switch (butMode_) {
        case SSPChannel::BM_SOLOMUTE: {
            buttons_[B_A_1].label("Solo");
            buttons_[B_A_2].label("Solo");
            buttons_[B_A_3].label("Solo");
            buttons_[B_A_4].label("Solo");
            buttons_[B_B_1].label("Mute");
            buttons_[B_B_2].label("Mute");
            buttons_[B_B_3].label("Mute");
            buttons_[B_B_4].label("Mute");
        }
            break;
        case SSPChannel::BM_CUEAC: {
            buttons_[B_A_1].label("Cue");
            buttons_[B_A_2].label("Cue");
            buttons_[B_A_3].label("Cue");
            buttons_[B_A_4].label("Cue");
            buttons_[B_B_1].label("AC");
            buttons_[B_B_2].label("AC");
            buttons_[B_B_3].label("AC");
            buttons_[B_B_4].label("AC");
        }
            break;
        default: {
            buttons_[B_A_1].label("");
            buttons_[B_A_2].label("");
            buttons_[B_A_3].label("");
            buttons_[B_A_4].label("");
            buttons_[B_B_1].label("");
            buttons_[B_B_2].label("");
            buttons_[B_B_3].label("");
            buttons_[B_B_4].label("");
        }
    }
     */
}

