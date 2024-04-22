#pragma once


#include "SSP.h"
#include "SSP.h"
#include "controls/BaseParameter.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>

namespace ssp {

class BaseProcessor :
    public juce::AudioProcessor,
    public juce::MidiInputCallback,
//                      public juce::AudioProcessorListener {
    public juce::AudioProcessorListener,
    private juce::ValueTree::Listener {

public:

    explicit BaseProcessor(const AudioProcessor::BusesProperties &ioLayouts,
                           juce::AudioProcessorValueTreeState::ParameterLayout pl);
    ~BaseProcessor() override;

    virtual void init();

    bool acceptsMidi() const override { return false; }

    bool producesMidi() const override { return false; }

    bool silenceInProducesSilenceOut() const override { return false; }

    double getTailLengthSeconds() const override { return 0.0f; }

    int getNumPrograms() override { return 1; }

    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int index) override {}

    const juce::String getProgramName(int index) override { return ""; }

    void changeProgramName(int index, const juce::String &newName) override {}

    void prepareToPlay(double newSampleRate, int estimatedSamplesPerBlock) override;
    void releaseResources() override;

    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;


    juce::RangedAudioParameter *getParameter(juce::StringRef n) { return apvts.getParameter(n); }

    juce::AudioProcessorValueTreeState &vts() { return apvts; }

    virtual void onInputChanged(unsigned i, bool b);
    virtual void onOutputChanged(unsigned i, bool b);

    bool isOutputEnabled(unsigned i) { return i < numOut && outputEnabled[i]; }

    bool isInputEnabled(unsigned i) { return i < numIn && inputEnabled[i]; }

    void setMidiIn(const std::string &name);
    void setMidiOut(const std::string &name);


    bool isActiveMidiIn(const std::string &name) { return midiInDeviceName_ == name; }

    bool isActiveMidiOut(const std::string &name) { return midiOutDeviceName_ == name; }

    void midiLearn(bool b);

    virtual void midiNoteInput(unsigned note, unsigned velocity) { ; }

    void noteInput(bool b) { noteInput_ = b; }
    bool noteInput() const { return noteInput_; }   

    void midiChannel(unsigned ch) { midiChannel_ = ch; }

    int midiChannel() const { return midiChannel_; }

    void useCompactUI(bool b) { compactEditor_ = b; }
    bool useCompactUI() const { return compactEditor_; }




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

        bool valid() const { return paramIdx_ >= 0 && midi_.type_ != Midi::T_MAX; }

        void store(juce::XmlElement *);
        void recall(juce::XmlElement *);
    };



protected:
    friend class BaseEditor;

    friend class SystemEditor;

    //FIXME : or at least clean up!
    static constexpr unsigned numIn = 24;
    static constexpr unsigned numOut = 24;

    juce::AudioProcessorValueTreeState apvts;
    bool inputEnabled[numIn];
    bool outputEnabled[numOut];

    virtual void midiFromXml(juce::XmlElement *);
    virtual void midiToXml(juce::XmlElement *);
    virtual void customFromXml(juce::XmlElement *);
    virtual void customToXml(juce::XmlElement *);


#if __APPLE__
    virtual void testFromXml(juce::XmlElement *);
    virtual void testToXml(juce::XmlElement *);
#endif

    void addBaseParameters(juce::AudioProcessorValueTreeState::ParameterLayout &);


    // midi automation
    // AudioProcessorListener
    void audioProcessorParameterChanged(juce::AudioProcessor *p, int parameterIndex, float newValue) override;

    void audioProcessorChanged(juce::AudioProcessor *processor, const AudioProcessorListener::ChangeDetails &details) override { ; }

    // MidiInputCallback
    void handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message) override;

    void handleMidi(const juce::MidiMessage &message);

    void automateParam(int idx, const MidiAutomation &a, const juce::MidiMessage &msg);

    std::map<int, MidiAutomation> midiAutomation_;

    std::string midiInDeviceName_;
    std::string midiOutDeviceName_;
    std::unique_ptr<juce::MidiInput> midiInDevice_;
    std::unique_ptr<juce::MidiOutput> midiOutDevice_;
    int midiChannel_ = 0;
    bool midiLearn_ = false;
    bool noteInput_ = false;

    MidiAutomation lastLearn_;

    bool compactEditor_ = false;

    struct MidiMsg {

    };

//    moodycamel::ReaderWriterQueue<MidiMsg> messageQueue_;

public:
    std::map<int, MidiAutomation> &midiAutomation() { return midiAutomation_; }

private:
    std::string getMidiInputDeviceId(const std::string &name);
    std::string getMidiOutputDeviceId(const std::string &name);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseProcessor)
};

}
