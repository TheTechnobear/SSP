#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>

using namespace juce;

#include "SSP.h"

#include "BaseParameter.h"

namespace ssp {

class BaseProcessor : public AudioProcessor,
                      public MidiInputCallback,
                      public AudioProcessorListener {
//                      public AudioProcessorListener,
//                      private ValueTree::Listener {
public:

    explicit BaseProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                           juce::AudioProcessorValueTreeState::ParameterLayout pl);
    virtual ~BaseProcessor();

    virtual void init();

    bool acceptsMidi() const override { return false; }

    bool producesMidi() const override { return false; }

    bool silenceInProducesSilenceOut() const override { return false; }

    double getTailLengthSeconds() const override { return 0.0f; }

    int getNumPrograms() override { return 1; }

    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int index) override {}

    const String getProgramName(int index) override { return ""; }

    void changeProgramName(int index, const juce::String &newName) override {}

    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    struct sspParams {
        static constexpr unsigned numEnc = Percussa::sspEnc4 - Percussa::sspEnc1 + 1;
        static constexpr unsigned numIn = Percussa::sspInEn24 - Percussa::sspInEn1 + 1;
        static constexpr unsigned numOut = Percussa::sspOutEn24 - Percussa::sspOutEn1 + 1;
        static constexpr unsigned numSw = Percussa::sspSw8 - Percussa::sspSw1 + 1;
        static constexpr unsigned numCtrlSw = Percussa::sspSwShiftR - Percussa::sspSwLeft + 1;

        static const char *getId(unsigned id) {
            if (id < Percussa::sspLast) return percussaParamsName[id];
            return "unknown";
        };
    };

    RangedAudioParameter *getParameter(StringRef n) { return apvts.getParameter(n); }

    AudioProcessorValueTreeState &vts() { return apvts; }

    virtual void onInputChanged(unsigned i, bool b);
    virtual void onOutputChanged(unsigned i, bool b);

    bool isOutputEnabled(unsigned i) { return i < sspParams::numOut && outputEnabled[i]; }

    bool isInputEnabled(unsigned i) { return i < sspParams::numIn && inputEnabled[i]; }

    void setMidiIn(std::string id);
    void setMidiOut(std::string id);

    bool isActiveMidiIn(const std::string &id) { return midiInDeviceId_ == id; }

    bool isActiveMidiOut(const std::string &id) { return midiOutDeviceId_ == id; }

    void midiLearn(bool b);

protected:
    friend class BaseEditor;
    friend class SystemEditor;

    AudioProcessorValueTreeState apvts;
    bool inputEnabled[sspParams::numIn];
    bool outputEnabled[sspParams::numOut];

    virtual void midiFromXml(juce::XmlElement *);
    virtual void midiToXml(juce::XmlElement *);
    virtual void customFromXml(juce::XmlElement *);
    virtual void customToXml(juce::XmlElement *);

    void addBaseParameters(AudioProcessorValueTreeState::ParameterLayout &);


    // midi automation
    // AudioProcessorListener
    void audioProcessorParameterChanged(AudioProcessor *p, int parameterIndex, float newValue) override;

    void audioProcessorChanged(AudioProcessor *processor) override { ; }

    // MidiInputCallback
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override;

    void handleMidi(const MidiMessage &message);
    void automateParam(int idx, float v);

    struct MidiAutomation {
        int paramIdx_ = -1;
        float scale_ = 1.0f;
        float offset_ = 0.0f;
        struct Midi {
            int channel_ = 0;
            int num_ = 0; // note num, cc , if applicable
            enum Type {
                T_CC,
                T_PRESSURE,
                T_NOTE,
                T_MAX
            } type_ = T_MAX;
        } midi_;


        void reset() {
            paramIdx_ = -1;
            scale_ = 1.0f;
            offset_ = 0.0f;
            midi_.channel_ = 0;
            midi_.num_ = 0;
            midi_.type_ = Midi::T_MAX;
        }

        bool valid() { return paramIdx_ >= 0 && midi_.type_ != Midi::T_MAX; }

        void store(XmlElement *);
        void recall(XmlElement *);
    };

    std::vector<MidiAutomation> midiAutomation_;

    std::string midiInDeviceId_;
    std::string midiOutDeviceId_;
    std::unique_ptr<MidiInput> midiInDevice_;
    std::unique_ptr<MidiOutput> midiOutDevice_;
    int midiChannel_ = 0;
    bool midiLearn_ = false;

    MidiAutomation lastLearn_;

public:
    std::vector<MidiAutomation> &midiAutomation() { return midiAutomation_; }

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseProcessor)
};

}
