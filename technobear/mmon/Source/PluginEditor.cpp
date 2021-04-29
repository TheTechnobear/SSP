
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ssp/BaseParamControl.h"
#include "ssp/ButtonControl.h"

inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

using pcontrol_type = ssp::BarParamControl;
using bcontrol_type = ssp::ButtonControl;

class MyCallback : public MidiInputCallback {
public:
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override {
        Logger::writeToLog("handleIncomingMidiMessage" + String(message.getDescription()));

    }

};


PluginEditor::PluginEditor(PluginProcessor &p)
    : base_type(&p,
                String(JucePlugin_Name) + " : " + String(JucePlugin_Desc),
                JucePlugin_VersionString),
      processor_(p) {

//    addParamPage(
//        std::make_shared<pcontrol_type>(processor_.params_.vca[p][0],0.1f),
//        nullptr,
//        nullptr,
//        nullptr,
//        nullptr
//    );



    auto devlist = MidiInput::getAvailableDevices();
    for(int i=0;i<devlist.size();i++) {
        auto dev = devlist[i];
        Logger::writeToLog("midi device ->  " + dev.identifier + " : "+ dev.name);
        if(dev.name=="MIDIMATE II MIDI 1") {
            MyCallback *cb = new MyCallback;
            midiInput_ = MidiInput::openDevice(dev.identifier, cb);
            midiInput_->start();
            Logger::writeToLog("opening midi device :" + dev.name);
        }

    }


    setSize(1600, 480);
}

void PluginEditor::paint(Graphics &g) {
    base_type::paint(g);
}


void PluginEditor::resized() {
}


