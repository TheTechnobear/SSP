#include "SystemEditor.h"

#include "BaseProcessor.h"

#include <juce_audio_devices/juce_audio_devices.h>

//#include "SSP.h"

namespace ssp {

//static constexpr unsigned menuTopY = 200 - 1;
static constexpr unsigned btnTopY = 380 - 1;
static constexpr unsigned btnSpaceY = 50;

inline bool isInternalMidi(const String &name) {
    return name.contains("Juce") || name.contains("Midi Through Port");
}

SystemEditor::SystemEditor(BaseProcessor *p) :
    baseProcessor_(p),
      learnBtn_(
          "Learn", [&](bool b) { midiLearn(b); }, 12 * COMPACT_UI_SCALE, Colours::yellow),
      delBtn_(
          "Delete", [&](bool b) { deleteAutomation(b); }, 12 * COMPACT_UI_SCALE, Colours::yellow),
      noteInputBtn_(
          "Note In", [&](bool b) { noteInput(b); }, 12 * COMPACT_UI_SCALE, Colours::lightskyblue),
    midiInCtrl_("Midi IN", [&](float idx, const std::string &str) { midiInCallback(idx, str); }),
    midiOutCtrl_("Midi OUT", [&](float idx, const std::string &str) { midiOutCallback(idx, str); }),
    midiChannelCtrl_("Midi Channel", [&](float idx, const std::string &str) { midiChannelCallback(idx, str); }) {

    learnBtn_.setToggle(true);
    noteInputBtn_.setToggle(true);


    baseProcessor_->midiLearn(false);

    auto in = MidiInput::getAvailableDevices();
    int selIdx = -1;
    midiInStr_.push_back("NONE");
    int idx = 0;
    for (int i = 0; i < in.size(); i++) {
        if (!isInternalMidi(in[i].name)) {
            inDevices_.push_back(in[i]);
            midiInStr_.push_back(std::to_string(idx) + ":" + in[i].name.toStdString());
            if (baseProcessor_->isActiveMidiIn(in[i].name.toStdString())) selIdx = idx + 1; // none
            idx++;
        }
    }
    midiInCtrl_.setValues(midiInStr_, selIdx);

    selIdx = -1;
    idx = 0;
    auto out = MidiOutput::getAvailableDevices();
    midiOutStr_.push_back("NONE");
    for (int i = 0; i < out.size(); i++) {
        if (!isInternalMidi(out[i].name)) {
            outDevices_.push_back(out[i]);
            midiOutStr_.push_back(std::to_string(idx) + ":" + out[i].name.toStdString());
            if (baseProcessor_->isActiveMidiOut(out[i].name.toStdString())) selIdx = idx + 1; //none
            idx++;
        }
    }

    midiOutCtrl_.setValues(midiOutStr_, selIdx);


    midiChStr_.push_back("OMNI");
    for (int i = 0; i < 16; i++) { midiChStr_.push_back(String(i + 1).toStdString()); }
    midiChannelCtrl_.setValues(midiChStr_, baseProcessor_->midiChannel());

    addAndMakeVisible(midiInCtrl_);
    addAndMakeVisible(midiOutCtrl_);
    addAndMakeVisible(midiChannelCtrl_);


    addAndMakeVisible(noteInputBtn_);
    addAndMakeVisible(learnBtn_);
    addAndMakeVisible(delBtn_);

    noteInputBtn_.value(baseProcessor_->noteInput());

    selIdx_ = 0;
    idxOffset_ = 0;
}


void SystemEditor::midiInCallback(float idx, const std::string &dev) {
//    Logger::writeToLog("midiInCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto device = inDevices_[i - 1];
        if (!isInternalMidi(device.name)) {
            baseProcessor_->setMidiIn(device.name.toStdString());
            return;
        }
    }
    baseProcessor_->setMidiIn("");
}

void SystemEditor::midiOutCallback(float idx, const std::string &dev) {
//    Logger::writeToLog("midiOutCallback -> " + String(idx) + " : " + dev);
    unsigned i = idx;
    if (i > 0) { // 0 ==  NONE
        auto device = outDevices_[i - 1];
        if (!isInternalMidi(device.name)) {
            baseProcessor_->setMidiOut(device.name.toStdString());
            return;
        }
    }
    baseProcessor_->setMidiOut("");
}

void SystemEditor::midiChannelCallback(float idx, const std::string &ch) {
    baseProcessor_->midiChannel(idx);
}


SystemEditor::~SystemEditor() {
    baseProcessor_->midiLearn(false);
}

void SystemEditor::midiLearn(bool b) {
    baseProcessor_->midiLearn(b);
}

void SystemEditor::noteInput(bool b) {
    baseProcessor_->noteInput(b);
}

void SystemEditor::deleteAutomation(bool b) {
    if (!b) {
        if (selIdx_ >= 0) {
            auto &am = baseProcessor_->midiAutomation();
            if (am.empty() || selIdx_ >= am.size()) return;

            int idx = 0;
            for (auto ai = am.begin(); ai != am.end(); ai++) {
                auto &a = ai->second;
                if (idx == selIdx_) {
                    am.erase(a.paramIdx_);
                    if (selIdx_ != 0) {
                        selIdx_--;
                        if (selIdx_ < idxOffset_) idxOffset_ = selIdx_;
                    }
                    break;
                }
                idx++;
            }
        }
    }
}

void SystemEditor::onEncoder(unsigned enc, float v) {
    switch (enc) {
        case 0: {
            auto amsize = baseProcessor_->midiAutomation().size();
            if (amsize == 0) return;

            if (v > 0) {
                if (selIdx_ < amsize - 1) {
                    selIdx_++;
                    if (selIdx_ >= idxOffset_ + MAX_SHOWN) idxOffset_ = selIdx_ - (MAX_SHOWN - 1);
                }
            } else {
                if (selIdx_ > 0) {
                    selIdx_--;
                    if (selIdx_ < idxOffset_) idxOffset_ = selIdx_;
                }
            }
            break;
        }
        case 1 : {
            if (v > 0) midiChannelCtrl_.inc(false);
            else midiChannelCtrl_.dec(false);
            break;
        }
        case 2 : {
            if (v > 0) midiInCtrl_.inc(false);
            else midiInCtrl_.dec(false);
            break;
        }
        case 3 : {
            if (v > 0) midiOutCtrl_.inc(false);
            else midiOutCtrl_.dec(false);
            break;
        }
        default: { ;
        }
    }
}

void SystemEditor::onEncoderSwitch(unsigned enc, bool v) {
}

void SystemEditor::onButton(unsigned btn, bool v) {
    switch (btn) {
        case 0: {
            learnBtn_.onButton(v);
            break;
        }
        case 1: {
            noteInputBtn_.onButton(v);
            break;
        }
        case 4: {
            delBtn_.onButton(v);
            break;
        }
        default: break;
    }
}


}
