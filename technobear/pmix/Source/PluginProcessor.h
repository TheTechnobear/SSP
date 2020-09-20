#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "Percussa.h"

#include <atomic>
#include <algorithm>


inline float constrain(float v, float vMin, float vMax) {
    return std::max<float>(vMin, std::min<float>(vMax, v));
}

struct TrackData {
    TrackData() {
        init();
    }

    void makeFollow(unsigned f) {
        dummy_ = true;
        follows_ = f;
    }

    void init() {
        level_[MASTER] = 1.0f;
        level_[CUE] = 1.0f;
        for (unsigned i = CUE + 1; i < OUT_TRACKS; i++) level_[i] = 0.0f;
        pan_ = 0.0f;
        gain_ = 1.0f;
        mute_ = false;
        solo_ = false;
        cue_ = false;
        ac_ = false;
        dummy_ = false;
        follows_ = 0;
    }

    static constexpr unsigned OUT_TRACKS = 4;
    static constexpr unsigned MASTER = 0;
    static constexpr unsigned CUE = 1;

    std::atomic<float>  level_[OUT_TRACKS];  // 0==master, 1==cue , 2/3 == aux1/2
    std::atomic<float>  pan_;    // -1 to 1
    std::atomic<float>  gain_;
    std::atomic<bool>   mute_;
    std::atomic<bool>   solo_;
    std::atomic<bool>   cue_;
    std::atomic<bool>   ac_;

    // currently cannot be changed in ui
    bool                dummy_;
    unsigned            follows_; // dummy

    std::atomic<float>  lvl_; // calculated rms/peak level
    // only used by processor
    // RMS
    bool                useRMS_ = false;
    static constexpr    unsigned MAX_RMS = 100 ; // 1000 / (48000/129)  = 1 block = 2.8 mS, we want ~ 300mS
    unsigned            lvlHead_ = 0;
    float               lvlSum_ = 0.0f;
    float               lvlHistory_[MAX_RMS];

    // hpf/dc block
    float               dcX1_ = 0.0f, dcY1_ = 0.0f;
};


class Pmix  : public AudioProcessor
{
public:
    Pmix();
    ~Pmix();


    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    int getNumParameters() override;
    float getParameter (int index) override;
    void setParameter (int index, float newValue) override;

    const String getParameterName (int index) override;
    const String getParameterText (int index) override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    unsigned numInTracks() { return  IN_T_MAX;}
    unsigned numOutTracks() { return  OUT_T_MAX;}

    TrackData& inputTrack(unsigned t)   {  return t < IN_T_MAX ? inTracks_[t] : inTracks_[IN_T_MAX - 1];}
    TrackData& outputTrack(unsigned t)  {  return t < OUT_T_MAX ? outTracks_[t] : outTracks_[OUT_T_MAX - 1];}

    bool isInputEnabled(unsigned idx) { return idx < IN_T_MAX ?  params_[Percussa::sspInEn1 + I_IN_1 + idx] > 0.5f : false ;}
    bool isOutputEnabled(unsigned idx) { return idx < OUT_T_MAX ? params_[Percussa::sspOutEn1 + O_MAIN_L + idx] > 0.5f : false ;}


    enum {
        I_IN_1,
        I_IN_2,
        I_IN_3,
        I_IN_4,
        I_IN_5,
        I_IN_6,
        I_IN_7,
        I_IN_8,
        I_MAX
    };

    enum {
        O_MAIN_L,
        O_MAIN_R,
        O_CUE_L,
        O_CUE_R,
        O_AUX_1_L,
        O_AUX_1_R,
        O_AUX_2_L,
        O_AUX_2_R,
        O_MAX
    };

protected:
    float cv2Pitch(float r) {
        // SSP SDK
        static constexpr float p1 = 0.02325f; // first C note
        static constexpr float p2 = 0.21187f; // second C note
        static constexpr float scale = 12.0f / (p2 - p1);
        float arg = r;
        arg = arg - p1;
        arg = arg * scale;
        return arg;
    }

private:

    void writeToXml(juce::XmlElement& xml);
    void readFromXml(juce::XmlElement& xml);

    void writeTrackXml(TrackData& t, juce::XmlElement& xml);
    void readTrackXml(TrackData& t, juce::XmlElement& xml);


    inline void dcBlock(float x, float& x1 , float&y, float& y1) {
        // y[n] = x[n] - x[n-1] + a * y[n-1]
        // example usage
        // dcBlock(curSample, lastIn, curOut, lastOut)
        static constexpr float a = 0.995f;
        y = x - x1 + a * y1;
        y1 = y;
        x1 = x;
    }

    static constexpr unsigned IN_T_MAX = 8;
    static constexpr unsigned OUT_T_MAX = 8;

    TrackData inTracks_[IN_T_MAX];
    TrackData outTracks_[OUT_T_MAX];
    void initTracks();


    float params_[Percussa::sspLast];

    AudioSampleBuffer inputBuffers_;
    AudioSampleBuffer outputBuffers_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pmix)
};


